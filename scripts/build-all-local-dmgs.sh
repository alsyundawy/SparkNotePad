#!/usr/bin/env bash
# ==============================================================================
# SparkNotePad Multi-Version Local DMG Automated Build Runner
# ==============================================================================
# Builds macOS DMG installers for historical and target versions (1.3.5 - 1.3.9)
# using isolated git worktrees, standard CMake Unix Makefiles, and -j2 concurrency
# to prevent CPU thermal spikes.
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
DIST_DIR="${REPO_ROOT}/dist"
CACHE_DIR="${REPO_ROOT}/.cpm-cache"

mkdir -p "${DIST_DIR}"

VERSIONS=("1.3.9" "1.3.8" "1.3.7" "1.3.6" "1.3.5")

echo "=========================================================="
echo "⚡ Starting Local Multi-Version DMG Build Pipeline"
echo "Target Versions : ${VERSIONS[*]}"
echo "Destination     : ${DIST_DIR}"
echo "=========================================================="

for ver in "${VERSIONS[@]}"; do
  TAG_NAME="v${ver}"
  TARGET_DMG="${DIST_DIR}/SparkNotePad-v${ver}-x64.dmg"

  if [[ -f "${TARGET_DMG}" ]]; then
    echo "[*] Version ${ver} DMG already exists at ${TARGET_DMG}. Verifying..."
    if hdiutil verify "${TARGET_DMG}" >/dev/null 2>&1; then
      echo "[✔] Existing DMG for v${ver} is valid. Skipping build."
      continue
    else
      echo "[!] Existing DMG is corrupted. Rebuilding..."
      rm -f "${TARGET_DMG}"
    fi
  fi

  echo "----------------------------------------------------------"
  echo ">>> Building local DMG for version: ${ver} (Tag: ${TAG_NAME})"
  echo "----------------------------------------------------------"

  WT_DIR="/tmp/sparknotepad-wt-${ver}"
  rm -rf "${WT_DIR}"
  git worktree prune || true

  echo "[*] Creating isolated worktree at ${WT_DIR} for tag ${TAG_NAME}..."
  git worktree add "${WT_DIR}" "${TAG_NAME}"

  pushd "${WT_DIR}" >/dev/null

  echo "[*] Configuring CMake (Unix Makefiles, No Ninja, Release)..."
  cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCPM_SOURCE_CACHE="${CACHE_DIR}" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

  echo "[*] Compiling application and building DMG target (-j2)..."
  cmake --build build --target dmg -j2

  # Locate output DMG
  BUILT_DMG="$(find build -maxdepth 2 -name "SparkNotePad*.dmg" -not -name "rw.*" | head -n 1 || true)"
  if [[ -z "${BUILT_DMG}" || ! -f "${BUILT_DMG}" ]]; then
    echo "[-] Error: Failed to find built DMG for version ${ver}" >&2
    popd >/dev/null
    git worktree remove --force "${WT_DIR}" || true
    continue
  fi

  echo "[*] Verifying built DMG: ${BUILT_DMG}..."
  hdiutil verify "${BUILT_DMG}"

  echo "[*] Copying verified DMG to ${TARGET_DMG}..."
  cp -v "${BUILT_DMG}" "${TARGET_DMG}"

  popd >/dev/null

  echo "[*] Cleaning up worktree ${WT_DIR}..."
  git worktree remove --force "${WT_DIR}" || true

  echo "[✔] Version ${ver} DMG build and verification complete!"
done

echo "=========================================================="
echo "🎉 All local DMG builds completed successfully!"
ls -lh "${DIST_DIR}"/SparkNotePad-v*.dmg
echo "=========================================================="
