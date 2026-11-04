#!/usr/bin/env bash
set -Eeuo pipefail

QT_DIR="${QT_DIR:-/Users/alsyundawy/Qt/6.12.0/macos}"
MAX_PARALLEL="${MAX_PARALLEL:-1}"
NICE_LEVEL="${NICE_LEVEL:-12}"
ARCH="${ARCH:-auto}"

[[ "$(uname -s)" != "Darwin" ]] && {
  echo "Hanya untuk macOS"
  exit 1
}
[[ -d "$QT_DIR" ]] || {
  echo "QT_DIR tidak ada: $QT_DIR"
  exit 1
}

CPUS=$(sysctl -n hw.ncpu)
BUILD_JOBS=$((CPUS > 4 ? CPUS / 2 : CPUS - 1))
[[ $BUILD_JOBS -lt 1 ]] && BUILD_JOBS=1

if [[ "$ARCH" == "auto" ]]; then
  HOST_ARCH=$(uname -m)
  [[ "$HOST_ARCH" == "arm64" ]] && ARCH="arm64" || ARCH="x86_64"
fi
case "$ARCH" in
  x86_64 | arm64) CMAKE_ARCH="$ARCH" ;;
  universal) CMAKE_ARCH="arm64;x86_64" ;;
  *)
    echo "ARCH salah: $ARCH"
    exit 1
    ;;
esac

echo "=== CPU:$CPUS | Jobs:$BUILD_JOBS | Parallel:$MAX_PARALLEL | Arch:$ARCH | LOCAL ONLY ==="

REPO_ROOT=$(git rev-parse --show-toplevel)
cd "$REPO_ROOT"

if [[ $# -gt 0 ]]; then
  BRANCHES=("$@")
else
  BRANCHES=("main" "1.3.8")
fi

CENTRAL_BUILD="$REPO_ROOT/build"
mkdir -p "$CENTRAL_BUILD"
WORKTREE_BASE="$REPO_ROOT/.worktrees"
mkdir -p "$WORKTREE_BASE"

PIDS=()
cleanup() {
  echo
  echo "[*] Cleaning up..."
  [[ ${#PIDS[@]} -gt 0 ]] && kill "${PIDS[@]}" 2>/dev/null || true
  wait "${PIDS[@]}" 2>/dev/null || true
  for wt in "$WORKTREE_BASE"/*; do
    [[ -d "$wt" ]] && git worktree remove --force "$wt" 2>/dev/null || true
  done
  rm -rf "$WORKTREE_BASE"
}
trap cleanup EXIT INT TERM

is_local_branch() { git show-ref --verify --quiet "refs/heads/$1"; }

build_branch() {
  local BRANCH="$1"
  local SAFE_NAME
  SAFE_NAME=$(echo "$BRANCH" | tr '/:' '--')
  local WT="$WORKTREE_BASE/$SAFE_NAME"
  local LOG="$CENTRAL_BUILD/${SAFE_NAME}.log"
  local START_TS
  START_TS=$(date +%s)

  local RC=0
  if ! {
    echo "=================================================="
    echo "[$(date '+%H:%M:%S')] Building LOCAL: $BRANCH"
    echo "=================================================="
    cd "$WT"

    git checkout main -- scripts/build_all_local_dmgs.sh 2>/dev/null || true

    # Patch sederhana
    local opt="thirdparty/lexilla/lexlib/OptionSet.h"
    if [[ -f "$opt" ]] && ! grep -q cstdlib "$opt"; then
      python3 -c '
p="thirdparty/lexilla/lexlib/OptionSet.h"
c=open(p).read()
t="#ifndef OPTIONSET_H\n#define OPTIONSET_H\n"
r=t+"\n#include <cstdlib>\n#include <string>\n"
if t in c and "#include <cstdlib>" not in c:
    open(p,"w").write(c.replace(t,r,1))
    print("Patched OptionSet.h")
'
    fi

    rm -rf build
    cmake -S . -B build \
      -DCMAKE_BUILD_TYPE=Release \
      -DAPP_DISTRIBUTION=dmg \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=15.0 \
      -DCMAKE_OSX_ARCHITECTURES="$CMAKE_ARCH" \
      -DMAC_ARCH_SUFFIX="$ARCH" \
      -DCMAKE_PREFIX_PATH="$QT_DIR" \
      -DCMAKE_C_COMPILER=/usr/bin/clang \
      -DCMAKE_CXX_COMPILER=/usr/bin/clang++

    nice -n "$NICE_LEVEL" cmake --build build --target dmg --parallel "$BUILD_JOBS"

    DMG_PATH=$(find build -maxdepth 1 -name "*.dmg" -print -quit 2>/dev/null || true)
    [[ -z "$DMG_PATH" || ! -f "$DMG_PATH" ]] && {
      echo "ERROR: DMG tidak ketemu"
      exit 1
    }

    hdiutil verify "$DMG_PATH"
    cp -f "$DMG_PATH" "$CENTRAL_BUILD/${SAFE_NAME}-$(basename "$DMG_PATH")"

    if [[ -n $(git status --porcelain -uno) ]]; then
      git add -u
      git commit -m "fix(build): local macOS DMG packaging" || true
    fi

    echo "[$(date '+%H:%M:%S')] Selesai dalam $(($(date +%s) - START_TS))s"
  } >"$LOG" 2>&1; then
    RC=1
  fi

  [[ $RC -ne 0 ]] && echo "[ERROR] $BRANCH gagal → $LOG" >&2
  return $RC
}

echo "=== LOCAL ONLY Build ==="
for BRANCH in "${BRANCHES[@]}"; do
  if ! is_local_branch "$BRANCH"; then
    echo "[SKIP] Branch lokal '$BRANCH' tidak ada"
    continue
  fi

  SHA=$(git rev-parse --short "$BRANCH")
  echo "[*] Branch lokal '$BRANCH' @ $SHA"

  SAFE_NAME=$(echo "$BRANCH" | tr '/:' '--')
  WT="$WORKTREE_BASE/$SAFE_NAME"

  while [[ $(jobs -r | wc -l) -ge $MAX_PARALLEL ]]; do sleep 2; done

  echo "[*] Creating worktree (local only)..."
  git worktree add --force "$WT" "$BRANCH" || {
    echo "Gagal worktree $BRANCH"
    continue
  }

  build_branch "$BRANCH" &
  PIDS+=($!)
done

echo
echo "[*] Menunggu selesai..."
FAIL=0
for PID in "${PIDS[@]}"; do
  wait "$PID" || FAIL=1
done

cd "$CENTRAL_BUILD"
if ls ./*.dmg >/dev/null 2>&1; then
  shasum -a 256 ./*.dmg >SHA256SUMS
  cat SHA256SUMS
else
  echo "Tidak ada DMG"
  FAIL=1
fi

if [[ $FAIL -eq 0 ]]; then
  echo "=== SELESAI (LOCAL ONLY) ==="
else
  echo "=== ADA ERROR ==="
  exit 1
fi
