#!/usr/bin/env bash
# ==============================================================================
# SparkNotePad macOS Bundle Deployment & Framework Hardening Script
# ==============================================================================
# Resolves macdeployqt parsing failures on modern macOS (Xcode 15/16 otool headers),
# prunes bloatware frameworks, preserves required graphics/DBus libraries,
# and performs safe single-binary symbol stripping.
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

APP_PATH="${1:-${REPO_ROOT}/build/install/SparkNotePad.app}"
MACDEPLOYQT_BIN="${2:-}"

if [[ ! -d "${APP_PATH}" ]]; then
	echo "Error: App bundle not found at '${APP_PATH}'" >&2
	exit 1
fi

if [[ -z "${MACDEPLOYQT_BIN}" ]]; then
	if command -v macdeployqt >/dev/null 2>&1; then
		MACDEPLOYQT_BIN="macdeployqt"
	else
		echo "Error: macdeployqt not found in PATH and not passed as parameter" >&2
		exit 1
	fi
fi

echo "=========================================================="
echo "⚡ SparkNotePad macOS Deployment Pipeline"
echo "=========================================================="
echo "App Bundle    : ${APP_PATH}"
echo "macdeployqt   : ${MACDEPLOYQT_BIN}"
echo "=========================================================="

# 1. Setup temporary otool filter wrapper
# Modern Xcode (15/16) otool prints: "<path> (architecture arm64):"
# Older macdeployqt (Qt 6.2.x) expects: "<path>:"
# Without this filter, macdeployqt fails on every library line with:
# "ERROR: Could not parse otool output line" and skips rewriting @rpath links!
OTOOL_WRAPPER_DIR="$(mktemp -d /tmp/sparknotepad-otool-wrapper.XXXXXX)"
trap 'rm -rf "${OTOOL_WRAPPER_DIR}"' EXIT

cat << EOF > "${OTOOL_WRAPPER_DIR}/otool"
#!/usr/bin/env bash
REAL_OTOOL="\$(xcrun --find otool 2>/dev/null || echo "/usr/bin/otool")"
exec "\${REAL_OTOOL}" "\$@" | sed -E 's/^(.*) \(architecture [^)]+\):$/\1:/'
EOF
chmod +x "${OTOOL_WRAPPER_DIR}/otool"

OLD_PATH="${PATH}"
export PATH="${OTOOL_WRAPPER_DIR}:${PATH}"

echo "[*] Executing macdeployqt with universal otool compatibility wrapper..."
"${MACDEPLOYQT_BIN}" "${APP_PATH}"

# Restore original PATH and remove wrapper
export PATH="${OLD_PATH}"
rm -rf "${OTOOL_WRAPPER_DIR}"

# 2. Prune unused heavy bloatware frameworks and virtual keyboards
# Preserves standard image formats (libqjpeg, libqgif, libqwebp) and QtDBus
echo "[*] Pruning unused heavy bloatware frameworks and plugins..."
rm -rf \
	"${APP_PATH}/Contents/Frameworks/QtPdf.framework" \
	"${APP_PATH}/Contents/Frameworks/QtQml.framework" \
	"${APP_PATH}/Contents/Frameworks/QtQmlMeta.framework" \
	"${APP_PATH}/Contents/Frameworks/QtQmlModels.framework" \
	"${APP_PATH}/Contents/Frameworks/QtQmlWorkerScript.framework" \
	"${APP_PATH}/Contents/Frameworks/QtQuick.framework" \
	"${APP_PATH}/Contents/Frameworks/QtVirtualKeyboard.framework" \
	"${APP_PATH}/Contents/Frameworks/QtVirtualKeyboardQml.framework" \
	"${APP_PATH}/Contents/PlugIns/platforminputcontexts/libqtvirtualkeyboardplugin.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqpdf.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqtiff.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqwebp.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqmacheif.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqmacjp2.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqwbmp.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqtga.dylib" \
	"${APP_PATH}/Contents/PlugIns/tls/libqopensslbackend.dylib" \
	"${APP_PATH}/Contents/PlugIns/tls/libqcertonlybackend.dylib" 2>/dev/null || true

# Safe removal of QtOpenGL.framework: only prune if no bundle component links to it
if [[ -d "${APP_PATH}/Contents/Frameworks/QtOpenGL.framework" ]]; then
	if ! otool -L "${APP_PATH}/Contents/MacOS/SparkNotePad" \
		"${APP_PATH}/Contents/Frameworks/"* \
		"${APP_PATH}/Contents/PlugIns/platforms/"* 2>/dev/null | grep -q "QtOpenGL"; then
		echo "[*] QtOpenGL.framework is not linked by any component. Pruning..."
		rm -rf "${APP_PATH}/Contents/Frameworks/QtOpenGL.framework"
	else
		echo "[*] QtOpenGL.framework is referenced by Qt libraries. Retaining for runtime integrity."
	fi
fi

# 3. Strip local binary symbols from the main application binary ONLY (per AGENTS.md Rule 3)
# Note: Never strip pre-compiled Qt framework dylibs or plugins (e.g. libqcocoa.dylib),
# as stripping plugins corrupts Qt metadata query symbols and causes 'Abort trap: 6'!
echo "[*] Stripping local symbols from main executable..."
if [[ -f "${APP_PATH}/Contents/MacOS/SparkNotePad" ]]; then
	strip -x "${APP_PATH}/Contents/MacOS/SparkNotePad" 2>/dev/null || true
fi

# 4. Ad-hoc codesign app bundle
echo "[*] Ad-hoc codesigning app bundle..."
codesign --force --deep -s - "${APP_PATH}" 2>/dev/null || true

echo "✔ macOS bundle deployment complete for ${APP_PATH}"
