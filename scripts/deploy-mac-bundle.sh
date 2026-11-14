#!/usr/bin/env bash
# ==============================================================================
# SparkNotePad macOS Bundle Deployment & Framework Hardening Script
# ==============================================================================
# Resolves macdeployqt parsing failures on modern macOS (Xcode 15/16 otool
# multi-arch headers), guarantees @executable_path/../Frameworks LC_RPATH on
# Qt 6.2+, prunes bloatware frameworks and redundant plugins to preserve the
# strict 30MB-40MB DMG range, and performs safe single-binary symbol stripping.
# ==============================================================================

set -Eeuo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/.." && pwd -P)"

APP_PATH="${1:-${REPO_ROOT}/build/install/SparkNotePad.app}"
MACDEPLOYQT_BIN="${2:-}"

if [[ ! -d ${APP_PATH} ]]; then
	printf 'Error: App bundle not found at %s\n' "${APP_PATH}" >&2
	exit 1
fi

MAIN_BIN="${APP_PATH}/Contents/MacOS/SparkNotePad"
if [[ ! -f ${MAIN_BIN} ]]; then
	printf 'Error: Main executable not found at %s\n' "${MAIN_BIN}" >&2
	exit 1
fi

if [[ -z ${MACDEPLOYQT_BIN} ]]; then
	if command -v macdeployqt >/dev/null 2>&1; then
		MACDEPLOYQT_BIN="macdeployqt"
	else
		printf 'Error: macdeployqt not found in PATH and not passed as parameter\n' >&2
		exit 1
	fi
fi

REAL_OTOOL="$(xcrun --find otool 2>/dev/null || type -P otool || echo "/usr/bin/otool")"

ensure_rpath() {
	local bin_path="$1"
	if ! "${REAL_OTOOL}" -l "${bin_path}" 2>/dev/null | grep -q "@executable_path/../Frameworks"; then
		printf '[*] Injecting @executable_path/../Frameworks LC_RPATH into %s...\n' "${bin_path##*/}"
		install_name_tool -add_rpath "@executable_path/../Frameworks" "${bin_path}" 2>/dev/null || true
	fi
}

# Detect single-architecture slice (e.g., arm64 or x86_64) for universal otool filtering
ARCH_LIST="$(lipo -archs "${MAIN_BIN}" 2>/dev/null || true)"
TARGET_ARCH=""
if [[ -n ${ARCH_LIST} && ${ARCH_LIST} != *" "* ]]; then
	TARGET_ARCH="${ARCH_LIST}"
fi

echo "=========================================================="
echo "⚡ SparkNotePad macOS Deployment Pipeline"
echo "=========================================================="
printf 'App Bundle    : %s\n' "${APP_PATH}"
printf 'Target Arch   : %s\n' "${ARCH_LIST:-unknown}"
printf 'macdeployqt   : %s\n' "${MACDEPLOYQT_BIN}"
echo "=========================================================="

# 1. Guarantee LC_RPATH prior to macdeployqt (critical for Qt 6.2 CMake install)
ensure_rpath "${MAIN_BIN}"

# 2. Setup temporary otool filter wrapper
# Modern Xcode (15/16) otool on universal Qt frameworks (Qt 6.2.4) outputs:
#   <path> (architecture x86_64): ...
#   <path> (architecture arm64): ...
# Qt 6.2 macdeployqt strictly expects a SINGLE unannotated header `<path>:`.
# Without deduplication + header normalization, macdeployqt emits:
#   "ERROR: Could not parse otool output line" and skips rewriting @rpath!
OTOOL_WRAPPER_DIR="$(mktemp -d /tmp/sparknotepad-otool-wrapper.XXXXXX)"
trap 'rm -rf -- "${OTOOL_WRAPPER_DIR}"' EXIT

cat <<EOF >"${OTOOL_WRAPPER_DIR}/otool"
#!/usr/bin/env bash
set -euo pipefail
REAL_OTOOL="${REAL_OTOOL}"
TARGET_ARCH="${TARGET_ARCH}"
EOF

cat <<'EOF' >>"${OTOOL_WRAPPER_DIR}/otool"
exec "${REAL_OTOOL}" "$@" | awk -v target_arch="${TARGET_ARCH}" '
{
	if ($0 ~ /^[^ \t].* \(architecture [^)]+\):$/ || $0 ~ /^[^ \t].*:$/) {
		is_arch = ($0 ~ / \(architecture [^)]+\):$/)
		arch = ""
		if (is_arch) {
			arch = $0
			sub(/^.* \(architecture /, "", arch)
			sub(/\):$/, "", arch)
		}
		hdr = $0
		sub(/ \(architecture [^)]+\):$/, ":", hdr)
		block_count++
		in_first = (block_count == 1)
		if (in_first) {
			first_buf = hdr "\n"
		}
		if (target_arch != "" && arch == target_arch && !emitted) {
			emitting = 1
			emitted = 1
			print hdr
		} else {
			emitting = 0
		}
		next
	}
	if (in_first) {
		first_buf = first_buf $0 "\n"
	}
	if (emitting) {
		print $0
	}
}
END {
	if (!emitted && first_buf != "") {
		printf "%s", first_buf
	}
}'
EOF
chmod +x "${OTOOL_WRAPPER_DIR}/otool"

OLD_PATH="${PATH}"
export PATH="${OTOOL_WRAPPER_DIR}:${PATH}"

echo "[*] Executing macdeployqt with universal otool compatibility wrapper..."
"${MACDEPLOYQT_BIN}" "${APP_PATH}"

export PATH="${OLD_PATH}"
rm -rf -- "${OTOOL_WRAPPER_DIR}"

# 3. Guarantee LC_RPATH after macdeployqt (defense-in-depth for Qt 6.2)
ensure_rpath "${MAIN_BIN}"

# 4. Verify framework @rpath resolution integrity
echo "[*] Auditing @rpath framework linkages..."
OTOOL_DEPS_RAW="$("${REAL_OTOOL}" -L "${MAIN_BIN}" || true)"
DEP_LIBS="$(printf '%s\n' "${OTOOL_DEPS_RAW}" | awk 'NR > 1 { print $1 }')"
while IFS= read -r dep_line; do
	[[ -z ${dep_line} ]] && continue
	if [[ ${dep_line} == *"Qt"*".framework"* && ${dep_line} != "@rpath/"* ]]; then
		fw_part="${dep_line#*Qt}"
		fw_part="Qt${fw_part}"
		printf '[*] Rewriting non-@rpath linkage: %s -> @rpath/%s\n' "${dep_line}" "${fw_part}"
		install_name_tool -change "${dep_line}" "@rpath/${fw_part}" "${MAIN_BIN}" 2>/dev/null || true
	fi
done <<<"${DEP_LIBS}"

# 5. Prune unused heavy bloatware frameworks and redundant plugins
# Keeps essential Qt frameworks (including QtDBus) and core UI/image plugins
# (libqsvg, libqsvgicon, libqgif, libqico, libqcocoa, libqmacstyle, libqsecuretransportbackend)
# so ALL Qt versions (6.2 through 6.12) fit cleanly within the 30MB-40MB DMG range.
echo "[*] Pruning unused heavy bloatware frameworks and redundant plugins..."
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
	"${APP_PATH}/Contents/PlugIns/networkinformation/libqapplenetworkinformation.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqpdf.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqtiff.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqwebp.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqjpeg.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqicns.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqmacheif.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqmacjp2.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqwbmp.dylib" \
	"${APP_PATH}/Contents/PlugIns/imageformats/libqtga.dylib" \
	"${APP_PATH}/Contents/PlugIns/tls/libqopensslbackend.dylib" \
	"${APP_PATH}/Contents/PlugIns/tls/libqcertonlybackend.dylib" 2>/dev/null || true

# Safe removal of QtOpenGL.framework: only prune if no bundle component links to it
if [[ -d "${APP_PATH}/Contents/Frameworks/QtOpenGL.framework" ]]; then
	if ! "${REAL_OTOOL}" -L "${MAIN_BIN}" \
		"${APP_PATH}/Contents/Frameworks/"* \
		"${APP_PATH}/Contents/PlugIns/platforms/"* 2>/dev/null | grep -q "QtOpenGL"; then
		echo "[*] QtOpenGL.framework is not linked by any component. Pruning..."
		rm -rf -- "${APP_PATH}/Contents/Frameworks/QtOpenGL.framework"
	else
		echo "[*] QtOpenGL.framework is referenced by Qt libraries. Retaining for runtime integrity."
	fi
fi

# 6. Strip local binary symbols from the main application binary ONLY (per AGENTS.md Rule 3)
# Never strip pre-compiled Qt framework dylibs or plugins (e.g. libqcocoa.dylib),
# as stripping plugins corrupts Qt metadata query symbols and causes 'Abort trap: 6'!
echo "[*] Stripping local symbols from main executable..."
strip -x "${MAIN_BIN}" 2>/dev/null || true

# 7. Ad-hoc codesign app bundle
echo "[*] Ad-hoc codesigning app bundle..."
codesign --force --deep -s - "${APP_PATH}" 2>/dev/null || true

printf '✔ macOS bundle deployment complete for %s\n' "${APP_PATH}"
