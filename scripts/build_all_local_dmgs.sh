#!/usr/bin/env bash
set -Eeuo pipefail

# Scripts directory and repo root
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/.." && pwd -P)"
cd "${REPO_ROOT}"

echo "=== Starting Automated Local DMG Build Pipeline (SparkNotePad 1.3.7 down to 1.0.0) ==="

QT_DIR="/Users/alsyundawy/Qt/6.12.0/macos"
BRANCHES=("1.3.7" "1.3.6" "1.3.5" "1.3.0" "1.2.0" "1.1.0" "1.0.0")

mkdir -p "${REPO_ROOT}/build-dmg"

for BRANCH in "${BRANCHES[@]}"; do
    echo "=================================================="
    echo "Processing Branch: ${BRANCH}"
    echo "=================================================="

    # 1. Switch to branch
    git switch "${BRANCH}"

    # 2. Check and apply OptionSet.h fix if cstdlib is missing
    if [[ -f thirdparty/lexilla/lexlib/OptionSet.h ]] && ! grep -q "cstdlib" thirdparty/lexilla/lexlib/OptionSet.h; then
        echo "[*] Adding missing <cstdlib> include to OptionSet.h"
        python3 -c '
path = "thirdparty/lexilla/lexlib/OptionSet.h"
with open(path, "r", encoding="utf-8") as f:
    content = f.read()
target = "#ifndef OPTIONSET_H\n#define OPTIONSET_H\n"
replacement = "#ifndef OPTIONSET_H\n#define OPTIONSET_H\n\n#include <cstdlib>\n#include <string>\n"
if target in content and "#include <cstdlib>" not in content:
    content = content.replace(target, replacement, 1)
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)
    print("Successfully patched OptionSet.h")
'
    fi

    # 3. Configure & Build DMG
    echo "[*] Configuring and building DMG target for ${BRANCH}..."
    rm -rf build/install build/*.dmg
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DAPP_DISTRIBUTION=dmg \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=15.0 \
        -DCMAKE_OSX_ARCHITECTURES=x86_64 \
        -DMAC_ARCH_SUFFIX=x64 \
        -DCMAKE_PREFIX_PATH="${QT_DIR}" \
        -DCMAKE_C_COMPILER=/usr/bin/clang \
        -DCMAKE_CXX_COMPILER=/usr/bin/clang++

    cmake --build build --target dmg

    # Locate generated DMG file
    DMG_PATH=$(find build -maxdepth 1 -name "*.dmg" | head -n 1)
    if [[ -z "${DMG_PATH}" || ! -f "${DMG_PATH}" ]]; then
        echo "Error: DMG not found in build/ for branch ${BRANCH}" >&2
        exit 1
    fi

    echo "[*] Verifying DMG: ${DMG_PATH}"
    hdiutil verify "${DMG_PATH}"

    TARGET_DMG_NAME="$(basename "${DMG_PATH}")"
    echo "[*] Copying verified DMG to build-dmg/${TARGET_DMG_NAME}"
    cp -f "${DMG_PATH}" "${REPO_ROOT}/build-dmg/${TARGET_DMG_NAME}"

    # 4. Commit and push fixes if there are modifications
    if [[ -n "$(git status --porcelain)" ]]; then
        echo "[*] Staging and committing changes on branch ${BRANCH}"
        git add -u
        git commit -m "fix(build): ensure compatibility for local macOS DMG packaging" || true
        echo "[*] Pushing branch ${BRANCH} to origin"
        git push origin "refs/heads/${BRANCH}:refs/heads/${BRANCH}"
    else
        echo "[*] No file changes needed on branch ${BRANCH}"
    fi

    echo "=== Branch ${BRANCH} Completed Successfully! ==="
done

# Switch back to main
echo "=================================================="
echo "Returning to main"
echo "=================================================="
git switch main

# Update SHA256SUMS
echo "[*] Regenerating SHA256SUMS for all DMGs in build-dmg/"
cd "${REPO_ROOT}/build-dmg"
shasum -a 256 ./*.dmg > SHA256SUMS
cat SHA256SUMS
cd "${REPO_ROOT}"

echo "=== ALL LOCAL DMGS BUILT, VERIFIED, AND SYNCED SUCCESSFULLY! ==="
