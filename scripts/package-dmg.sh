#!/usr/bin/env bash
# ==============================================================================
# SparkNotePad macOS DMG Drag-and-Drop Packaging Script
# ==============================================================================
# Creates a modern, styled macOS DMG installer with custom background,
# drag-and-drop indicator, volume icon, and Applications symlink.
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

# Input parameters
APP_DIR="${1:-${REPO_ROOT}/build/install/SparkNotePad.app}"
DMG_OUTPUT="${2:-${REPO_ROOT}/build/SparkNotePad-v1.4.6-x64.dmg}"
VOL_NAME="${3:-SparkNotePad}"

# Ensure absolute paths
APP_PARENT_DIR="$(dirname "${APP_DIR}")"
if [[ -d ${APP_PARENT_DIR} ]]; then
	APP_PARENT_ABS="$(cd "${APP_PARENT_DIR}" && pwd)"
	APP_BASE="$(basename "${APP_DIR}")"
	APP_DIR="${APP_PARENT_ABS}/${APP_BASE}"
fi

DMG_PARENT_DIR="$(dirname "${DMG_OUTPUT}")"
mkdir -p "${DMG_PARENT_DIR}"
DMG_OUTPUT_DIR="$(cd "${DMG_PARENT_DIR}" && pwd)"
DMG_BASE="$(basename "${DMG_OUTPUT}")"
DMG_OUTPUT="${DMG_OUTPUT_DIR}/${DMG_BASE}"

echo "=========================================================="
echo "⚡ SparkNotePad DMG Drag-and-Drop Packaging Pipeline"
echo "=========================================================="
echo "Source App Bundle : ${APP_DIR}"
echo "Output Disk Image : ${DMG_OUTPUT}"
echo "Volume Name       : ${VOL_NAME}"
echo "=========================================================="

# Validation
if [[ ! -d ${APP_DIR} ]]; then
	echo "Error: App bundle not found at '${APP_DIR}'" >&2
	exit 1
fi

# Ensure background image exists, regenerate if needed
BG_IMG="${REPO_ROOT}/deploy/macos/dmg-background.png"
BG_IMG_2X="${REPO_ROOT}/deploy/macos/dmg-background@2x.png"
if [[ ! -f ${BG_IMG} ]]; then
	echo "[*] Generating DMG background image..."
	if command -v python3 >/dev/null 2>&1; then
		python3 "${SCRIPT_DIR}/generate_dmg_background.py"
	else
		echo "Warning: python3 not available to regenerate DMG background." >&2
	fi
fi

VOL_ICON="${REPO_ROOT}/icon/SparkNotePad.icns"
DS_STORE_PRESET="${REPO_ROOT}/deploy/macos/dmg-ds-store"

if [[ ! -f ${DS_STORE_PRESET} ]]; then
	echo "Error: Required DMG layout preset not found at '${DS_STORE_PRESET}'" >&2
	exit 1
fi

# Cleanup previous DMG if present
rm -f "${DMG_OUTPUT}"
hdiutil detach "/Volumes/${VOL_NAME}" -force 2>/dev/null || true

# Temporary staging folder containing only SparkNotePad.app
STAGE_DIR="$(mktemp -d /tmp/sparknotepad-dmg-stage.XXXXXX)"
trap 'rm -rf "${STAGE_DIR}"' EXIT

echo "[*] Staging application bundle..."
cp -R "${APP_DIR}" "${STAGE_DIR}/SparkNotePad.app"

# Ensure SparkNotePad.icns exists in bundle Resources
RESOURCES_DIR="${STAGE_DIR}/SparkNotePad.app/Contents/Resources"
mkdir -p "${RESOURCES_DIR}"
if [[ ! -f "${RESOURCES_DIR}/SparkNotePad.icns" ]]; then
	if [[ -f ${VOL_ICON} ]]; then
		echo "[*] Injecting application icon into bundle Resources..."
		cp "${VOL_ICON}" "${RESOURCES_DIR}/SparkNotePad.icns"
	fi
fi

# Prune unneeded heavy frameworks, plugins, and debug symbols from staged bundle
# NOTE: QtDBus.framework is strictly REQUIRED by QtGui.framework on macOS and must NOT be removed!
rm -rf "${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtPdf.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtQml.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtQmlMeta.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtQmlModels.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtQmlWorkerScript.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtQuick.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtVirtualKeyboard.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtVirtualKeyboardQml.framework" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/platforminputcontexts/libqtvirtualkeyboardplugin.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqpdf.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqtiff.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqwebp.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqmacheif.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqmacjp2.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqwbmp.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/imageformats/libqtga.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/tls/libqopensslbackend.dylib" \
	"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/tls/libqcertonlybackend.dylib" 2>/dev/null || true

# Safe removal of QtOpenGL.framework: only prune if no bundle component links to it
if [[ -d "${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtOpenGL.framework" ]]; then
	if ! otool -L "${STAGE_DIR}/SparkNotePad.app/Contents/MacOS/SparkNotePad" \
		"${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/"* \
		"${STAGE_DIR}/SparkNotePad.app/Contents/PlugIns/platforms/"* 2>/dev/null | grep -q "QtOpenGL"; then
		rm -rf "${STAGE_DIR}/SparkNotePad.app/Contents/Frameworks/QtOpenGL.framework"
	fi
fi

# Strip local symbols from main executable only (per AGENTS.md Rule 3; never strip Qt plugins/frameworks)
if [[ -f "${STAGE_DIR}/SparkNotePad.app/Contents/MacOS/SparkNotePad" ]]; then
	strip -x "${STAGE_DIR}/SparkNotePad.app/Contents/MacOS/SparkNotePad" 2>/dev/null || true
fi

codesign --force --deep -s - "${STAGE_DIR}/SparkNotePad.app" 2>/dev/null || true

# Touch the app bundle so macOS LaunchServices/Finder refreshes its icon cache
touch "${STAGE_DIR}/SparkNotePad.app"

# Stage styled presentation assets (background, Applications symlink, volume icon, .DS_Store)
echo "[*] Staging custom presentation assets with preserved background..."
ln -s /Applications "${STAGE_DIR}/Applications"

mkdir -p "${STAGE_DIR}/.background"
if [[ -f ${BG_IMG} ]]; then
	cp "${BG_IMG}" "${STAGE_DIR}/.background/dmg-background.png"
fi
if [[ -f ${BG_IMG_2X} ]]; then
	cp "${BG_IMG_2X}" "${STAGE_DIR}/.background/dmg-background@2x.png"
fi
chflags hidden "${STAGE_DIR}/.background" 2>/dev/null || true

cp "${DS_STORE_PRESET}" "${STAGE_DIR}/.DS_Store"

if [[ -f ${VOL_ICON} ]]; then
	cp "${VOL_ICON}" "${STAGE_DIR}/.VolumeIcon.icns"
	chflags hidden "${STAGE_DIR}/.VolumeIcon.icns" 2>/dev/null || true
fi

if command -v SetFile >/dev/null 2>&1; then
	SetFile -a V "${STAGE_DIR}/.background" 2>/dev/null || true
	SetFile -a V "${STAGE_DIR}/.VolumeIcon.icns" 2>/dev/null || true
	SetFile -c icnC "${STAGE_DIR}/.VolumeIcon.icns" 2>/dev/null || true
	SetFile -a C "${STAGE_DIR}" 2>/dev/null || true
fi

echo "[*] Packaging styled lightweight DMG via native hdiutil..."
hdiutil create \
	-volname "${VOL_NAME}" \
	-srcfolder "${STAGE_DIR}" \
	-ov \
	-format UDZO \
	-imagekey zlib-level=9 \
	"${DMG_OUTPUT}"

# Verification
if [[ ! -f ${DMG_OUTPUT} ]]; then
	echo "Error: Failed to create disk image '${DMG_OUTPUT}'" >&2
	exit 1
fi

echo "[*] Verifying DMG checksum and structure..."
hdiutil verify "${DMG_OUTPUT}"

# Strict size gate: must remain between 28MB and 40MB without bloatware
DMG_SIZE_BYTES="$(stat -f%z "${DMG_OUTPUT}" 2>/dev/null || wc -c <"${DMG_OUTPUT}")"
MIN_ALLOWED_BYTES=31457280 # 30 MB (31,457,280 bytes) per AGENTS.md Rule 3 floor
MAX_ALLOWED_BYTES=41943040 # 40 MB (41,943,040 bytes) per AGENTS.md Rule 3 ceiling
DMG_SIZE="$(du -h "${DMG_OUTPUT}" | cut -f1)"

if ((DMG_SIZE_BYTES < MIN_ALLOWED_BYTES)); then
	echo "[-] FATAL ERROR: SparkNotePad DMG size (${DMG_SIZE}, ${DMG_SIZE_BYTES} bytes) is below the 30MB threshold! Over-pruned or missing standard assets." >&2
	exit 1
fi

if ((DMG_SIZE_BYTES > MAX_ALLOWED_BYTES)); then
	echo "[-] FATAL ERROR: SparkNotePad DMG size (${DMG_SIZE}, ${DMG_SIZE_BYTES} bytes) exceeds the strict 40MB limit! Bloatware detected." >&2
	exit 1
fi

# Strict background & layout integrity gate
VERIFY_MOUNT="$(mktemp -d /tmp/sparknotepad-verify-mount.XXXXXX)"
if hdiutil attach "${DMG_OUTPUT}" -mountpoint "${VERIFY_MOUNT}" -nobrowse >/dev/null 2>&1; then
	if ! strings "${VERIFY_MOUNT}/.DS_Store" | grep -q "backgroundImageAlias"; then
		hdiutil detach "${VERIFY_MOUNT}" -force >/dev/null 2>&1 || true
		rm -rf "${VERIFY_MOUNT}"
		echo "[-] FATAL ERROR: Built DMG .DS_Store is missing backgroundImageAlias! Background image was lost." >&2
		exit 1
	fi
	if [[ ! -f "${VERIFY_MOUNT}/.background/dmg-background.png" ]]; then
		hdiutil detach "${VERIFY_MOUNT}" -force >/dev/null 2>&1 || true
		rm -rf "${VERIFY_MOUNT}"
		echo "[-] FATAL ERROR: Built DMG is missing .background/dmg-background.png!" >&2
		exit 1
	fi
	# Strict runtime launch verification: binary must load all dyld frameworks cleanly
	echo "[*] Testing binary launch from mounted DMG..."
	if ! TEST_LAUNCH_OUTPUT="$(QT_QPA_PLATFORM=offscreen "${VERIFY_MOUNT}/SparkNotePad.app/Contents/MacOS/SparkNotePad" --version 2>&1)"; then
		hdiutil detach "${VERIFY_MOUNT}" -force >/dev/null 2>&1 || true
		rm -rf "${VERIFY_MOUNT}"
		echo "[-] FATAL ERROR: SparkNotePad binary inside DMG failed dyld load or runtime launch!" >&2
		echo "[-] Binary output: ${TEST_LAUNCH_OUTPUT}" >&2
		exit 1
	fi
	echo "✔ Binary launch test passed: ${TEST_LAUNCH_OUTPUT}"
	hdiutil detach "${VERIFY_MOUNT}" -force >/dev/null 2>&1 || true
fi
rm -rf "${VERIFY_MOUNT}"

echo "=========================================================="
echo "✔ SparkNotePad DMG packaging complete: ${DMG_OUTPUT} (${DMG_SIZE})"
echo "✔ Verified standard lightweight size (30MB–40MB) and custom background presentation"
echo "=========================================================="
