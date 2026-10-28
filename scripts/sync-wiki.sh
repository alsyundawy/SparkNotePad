#!/usr/bin/env bash
# ==============================================================================
# SparkNotePad Wiki Synchronization Script
# Synchronizes the local staging folder `wiki/` with the GitHub Wiki repository:
# https://github.com/alsyundawy/SparkNotePad.wiki.git
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
WIKI_SRC="${REPO_ROOT}/wiki"

WIKI_REMOTE_SSH="git@github.com:alsyundawy/SparkNotePad.wiki.git"
WIKI_REMOTE_HTTPS="https://github.com/alsyundawy/SparkNotePad.wiki.git"

# Allow overriding target remote via environment variable
TARGET_REMOTE="${WIKI_REMOTE:-${WIKI_REMOTE_SSH}}"

echo "=== SparkNotePad GitHub Wiki Synchronization ==="
echo "Source directory : ${WIKI_SRC}"
echo "Target remote    : ${TARGET_REMOTE}"

if [[ ! -d ${WIKI_SRC} ]]; then
	echo "[-] Error: Source directory '${WIKI_SRC}' does not exist." >&2
	exit 1
fi

# Step 1: Probe remote connectivity
echo "[*] Checking if GitHub Wiki remote is initialized..."
if ! git ls-remote "${TARGET_REMOTE}" &>/dev/null; then
	# Try HTTPS fallback if SSH failed
	if git ls-remote "${WIKI_REMOTE_HTTPS}" &>/dev/null; then
		TARGET_REMOTE="${WIKI_REMOTE_HTTPS}"
		echo "[+] Connected via HTTPS remote: ${TARGET_REMOTE}"
	else
		echo ""
		echo "[!] NOTICE: The GitHub Wiki repository has not been initialized yet by GitHub."
		echo "    GitHub requires creating the initial wiki page via the Web UI before the git repository is created."
		echo ""
		echo "    To initialize your Wiki:"
		echo "    1. Navigate to: https://github.com/alsyundawy/SparkNotePad/wiki"
		echo "    2. Click the green 'Create the first page' button."
		echo "    3. Click 'Save Page' (any placeholder content is fine)."
		echo "    4. Re-run this script: ./scripts/sync-wiki.sh"
		echo ""
		exit 0
	fi
fi

# Step 2: Clone wiki into temporary directory
TMP_DIR="$(mktemp -d /tmp/sparknotepad-wiki-sync-XXXXXX)"
cleanup() {
	rm -rf "${TMP_DIR}"
}
trap cleanup EXIT INT TERM

echo "[*] Cloning wiki repository into temporary workspace..."
git clone "${TARGET_REMOTE}" "${TMP_DIR}"

# Step 3: Copy all files from staging directory
echo "[*] Syncing markdown documents and assets..."
cp -R "${WIKI_SRC}/"* "${TMP_DIR}/"

# Step 4: Commit and push changes
cd "${TMP_DIR}"
git add -A

if git diff-index --quiet HEAD --; then
	echo "[+] Wiki is already up to date. No changes to commit."
else
	COMMIT_HASH="$(git -C "${REPO_ROOT}" rev-parse --short HEAD 2>/dev/null || echo "manual")"
	COMMIT_MSG="docs(wiki): sync documentation from main repository (${COMMIT_HASH})"

	# Configure author if not set
	CONFIG_USER_NAME="$(git -C "${REPO_ROOT}" config user.name 2>/dev/null || true)"
	CONFIG_USER_EMAIL="$(git -C "${REPO_ROOT}" config user.email 2>/dev/null || true)"
	git config user.name "${CONFIG_USER_NAME:-Harry Dertin Sutisna Alsyundawy}"
	git config user.email "${CONFIG_USER_EMAIL:-alsyundawy@users.noreply.github.com}"

	echo "[*] Committing changes..."
	git commit -m "${COMMIT_MSG}"

	echo "[*] Pushing to GitHub Wiki..."
	git push origin master || git push origin main
	echo "[+] GitHub Wiki successfully synchronized!"
fi
