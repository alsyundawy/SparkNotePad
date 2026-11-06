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
DMG_OUTPUT="${2:-${REPO_ROOT}/build/SparkNotePad-v1.3.8-x64.dmg}"
VOL_NAME="${3:-SparkNotePad}"

# Ensure absolute paths
APP_PARENT_DIR="$(dirname "${APP_DIR}")"
if [[ -d "${APP_PARENT_DIR}" ]]; then
  APP_DIR="$(cd "${APP_PARENT_DIR}" && pwd)/$(basename "${APP_DIR}")"
fi

DMG_PARENT_DIR="$(dirname "${DMG_OUTPUT}")"
mkdir -p "${DMG_PARENT_DIR}"
DMG_OUTPUT_DIR="$(cd "${DMG_PARENT_DIR}" && pwd)"
DMG_OUTPUT="${DMG_OUTPUT_DIR}/$(basename "${DMG_OUTPUT}")"

echo "=========================================================="
echo "⚡ SparkNotePad DMG Drag-and-Drop Packaging Pipeline"
echo "=========================================================="
echo "Source App Bundle : ${APP_DIR}"
echo "Output Disk Image : ${DMG_OUTPUT}"
echo "Volume Name       : ${VOL_NAME}"
echo "=========================================================="

# Validation
if [[ ! -d "${APP_DIR}" ]]; then
  echo "Error: App bundle not found at '${APP_DIR}'" >&2
  exit 1
fi

# Ensure background image exists, regenerate if needed
BG_IMG="${REPO_ROOT}/deploy/macos/dmg-background.png"
if [[ ! -f "${BG_IMG}" ]]; then
  echo "[*] Generating DMG background image..."
  if command -v python3 >/dev/null 2>&1; then
    python3 "${SCRIPT_DIR}/generate_dmg_background.py"
  else
    echo "Warning: python3 not available to regenerate DMG background." >&2
  fi
fi

VOL_ICON="${REPO_ROOT}/icon/SparkNotePad.icns"

# Ensure create-dmg is installed
if ! command -v create-dmg >/dev/null 2>&1; then
  echo "[*] create-dmg not found in PATH. Checking Homebrew..."
  if command -v brew >/dev/null 2>&1; then
    echo "[*] Installing create-dmg via Homebrew..."
    brew install create-dmg || true
  fi
fi

# Cleanup previous DMG if present
rm -f "${DMG_OUTPUT}"
hdiutil detach "/Volumes/${VOL_NAME}" -force 2>/dev/null || true

# Temporary staging folder containing only SparkNotePad.app
STAGE_DIR="$(mktemp -d /tmp/sparknotepad-dmg-stage.XXXXXX)"
trap 'rm -rf "${STAGE_DIR}"' EXIT

echo "[*] Staging application bundle..."
cp -R "${APP_DIR}" "${STAGE_DIR}/SparkNotePad.app"

# Attempt DMG creation using create-dmg if available
CREATED=0
if command -v create-dmg >/dev/null 2>&1; then
  echo "[*] Packaging styled DMG using create-dmg..."
  CREATE_DMG_ARGS=(
    --volname "${VOL_NAME}"
    --window-pos 200 120
    --window-size 680 440
    --icon-size 128
    --text-size 12
    --icon "SparkNotePad.app" 170 245
    --hide-extension "SparkNotePad.app"
    --app-drop-link 510 245
    --format UDZO
    --hdiutil-retries 5
    --overwrite
  )

  if [[ -f "${VOL_ICON}" ]]; then
    CREATE_DMG_ARGS+=(--volicon "${VOL_ICON}")
  fi

  if [[ -f "${BG_IMG}" ]]; then
    CREATE_DMG_ARGS+=(--background "${BG_IMG}")
  fi

  # In CI / headless runner environments (no WindowServer/Finder UI), skip Finder AppleScript directly
  if [[ -n "${CI:-}" || -n "${GITHUB_ACTIONS:-}" ]]; then
    echo "[*] Detected CI/headless environment. Passing --skip-jenkins to create-dmg..."
    if create-dmg "${CREATE_DMG_ARGS[@]}" --skip-jenkins "${DMG_OUTPUT}" "${STAGE_DIR}"; then
      CREATED=1
      echo "[*] Styled DMG successfully created with create-dmg (CI headless mode)."
    fi
  else
    if create-dmg "${CREATE_DMG_ARGS[@]}" "${DMG_OUTPUT}" "${STAGE_DIR}"; then
      CREATED=1
      echo "[*] Styled DMG successfully created with create-dmg."
    else
      echo "Warning: Standard create-dmg failed. Retrying with --skip-jenkins (headless mode)..." >&2
      if create-dmg "${CREATE_DMG_ARGS[@]}" --skip-jenkins "${DMG_OUTPUT}" "${STAGE_DIR}"; then
        CREATED=1
        echo "[*] Styled DMG successfully created (headless mode)."
      fi
    fi
  fi
fi

# Fallback to native hdiutil if create-dmg is unavailable or failed
if [[ ${CREATED} -eq 0 ]]; then
  echo "[*] Falling back to native hdiutil packaging..."
  ln -s /Applications "${STAGE_DIR}/Applications"
  
  if [[ -f "${BG_IMG}" ]]; then
    mkdir -p "${STAGE_DIR}/.background"
    cp "${BG_IMG}" "${STAGE_DIR}/.background/dmg-background.png"
  fi

  hdiutil create \
    -volname "${VOL_NAME}" \
    -srcfolder "${STAGE_DIR}" \
    -ov \
    -format UDZO \
    -imagekey zlib-level=9 \
    "${DMG_OUTPUT}"
fi

# Verification
if [[ ! -f "${DMG_OUTPUT}" ]]; then
  echo "Error: Failed to create disk image '${DMG_OUTPUT}'" >&2
  exit 1
fi

echo "[*] Verifying DMG checksum and structure..."
hdiutil verify "${DMG_OUTPUT}"

DMG_SIZE="$(du -h "${DMG_OUTPUT}" | cut -f1)"
echo "=========================================================="
echo "✔ SparkNotePad DMG packaging complete: ${DMG_OUTPUT} (${DMG_SIZE})"
echo "=========================================================="
