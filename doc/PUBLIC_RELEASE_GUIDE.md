# SparkNotePad Public Release & Update Pipeline Guide

## 1. Overview

SparkNotePad's public release pipeline provides automated builds, codesigning, packaging, checksum verification, and in-app update checks across macOS and Linux.

---

## 2. Release Management Tools

### 2.1 Automated Release Preparation

The maintainer utility `scripts/prepare-release.sh` coordinates end-to-end packaging:

```bash
# Prepare release for a specific version tag:
./scripts/prepare-release.sh v1.3.7

# Dry-run validation:
./scripts/prepare-release.sh v1.3.7 --dry-run
```

Outputs produced:

- `SparkNotePad-v1.3.7-arm64.dmg` (macOS Apple Silicon M1/M2/M3/M4)
- `SparkNotePad-v1.3.7-x64.dmg` (macOS Intel x86_64)
- `sparknotepad_1.3.7_amd64.deb` (Debian / Ubuntu)
- `sparknotepad-1.3.7-1.x86_64.rpm` (Fedora / RHEL)
- `SparkNotePad-Default-Editor.sh` (File association integration helper)

### 2.2 Native In-App Update Engine (ReleaseManager)

Unlike legacy Windows-only updater implementations, `ReleaseManager`:

- Directly checks the official GitHub Releases API (`https://api.github.com/repos/alsyundawy/SparkNotePad/releases/latest`) using native `QNetworkAccessManager` with HTTP/2 safely disabled.
- Compares semantic versions (handling major, minor, patch, and pre-release `-alpha`/`-beta` suffixes).
- Prompts the user with release notes and a direct action button to open the release download page.
- Available via `Help > Check for Updates...` (`Ctrl+Shift+U` / `Cmd+Shift+U`).

---

## 3. GitHub Actions CI/CD Release Flow

When a new version tag is pushed (e.g., `git push origin v1.3.7`):

1. `.github/workflows/release.yml` triggers automatically (or can be dispatched manually).
2. Matrix compiles SparkNotePad on macOS and Linux across Qt 6 versions.
3. macOS binaries are signed with Hardened Runtime entitlements and notarized if Apple Developer ID credentials are present.
4. Linux packages (`.deb` and `.rpm`) are generated via CPack.
5. All production assets are uploaded directly to the GitHub Release.
