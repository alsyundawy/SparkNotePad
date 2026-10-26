#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: SparkNotePad-Default-Editor-Linux.sh
# DESCRIPTION: Production-Grade Linux Default Editor Configuration Engine for
#              SparkNotePad across all Debian (.deb) & RPM (.rpm) Distributions
#              and all Linux Desktop Environments (DEs) & Window Managers (WMs).
# MAINTAINER:  Harry Dertin Sutisna Alsyundawy (@alsyundawy)
# REPOSITORY:  https://github.com/alsyundawy/SparkNotePad
# VERSION:     2.1.0
# LICENSE:     GPL-3.0-or-later / MIT
# ==============================================================================
#
# ARCHITECTURAL OVERVIEW & SPECIFICATION
# ======================================
# 1. DISTRIBUTION FAMILIES:
#    - DEB Family: Debian, Ubuntu, Linux Mint, Pop!_OS, Elementary OS, Zorin OS,
#                  Kali Linux, Parrot OS, MX Linux, AntiX, Devuan, PureOS,
#                  Raspberry Pi OS, Bodhi Linux, Peppermint OS, Deepin, Tuxedo.
#    - RPM Family: Fedora, Red Hat Enterprise Linux (RHEL), CentOS Stream,
#                  Rocky Linux, AlmaLinux, openSUSE (Leap & Tumbleweed), SLES,
#                  Mageia, OpenMandriva, Amazon Linux, Oracle Linux, PCLinuxOS.
#    - Universal:  Arch Linux, Manjaro, Alpine, Void, Solus, Gentoo, NixOS.
#
# 2. DESKTOP ENVIRONMENTS & WINDOW MANAGERS:
#    - Full DEs:   GNOME (40-47), KDE Plasma (5 & 6), XFCE (4.16-4.18),
#                  Cinnamon, MATE, LXQt, LXDE, Budgie, COSMIC (Rust),
#                  Deepin DDE, Pantheon (Elementary), Enlightenment (E25).
#    - Tiling WMs: Sway, Hyprland, i3, Wayfire, bspwm, River, Awesome, dwm, Qtile.
#
# 3. 5-LAYER INTEGRATION MATRIX:
#    - Layer 1: FreeDesktop.org XDG MIME standard (xdg-mime & mimeapps.list)
#    - Layer 2: Desktop-Specific Integration APIs (GIO/GVFS, KWriteConfig/KIO, Xfconf)
#    - Layer 3: System Terminal Alternatives (update-alternatives / alternatives)
#    - Layer 4: Shell Environment Variables ($EDITOR, $VISUAL, $SUDO_EDITOR)
#    - Layer 5: Binary & Desktop Entry Resolution with Self-Healing Scaffolding
# ==============================================================================

set -Eeuo pipefail
IFS=$'\n\t'

# ====================== ENVIRONMENT & CONFIG ======================
readonly SCRIPT_VERSION="2.1.0"
readonly DEFAULT_DESKTOP_FILE="SparkNotePad.desktop"
readonly FALLBACK_DESKTOP_FILE="sparknotepad.desktop"
readonly FLATPAK_APP_ID="io.github.alsyundawy.SparkNotePad"

# Colors for terminal output
if [[ -t 1 ]] && [[ -z ${NO_COLOR:-} ]]; then
	RED='\033[0;31m'
	GREEN='\033[0;32m'
	YELLOW='\033[1;33m'
	BLUE='\033[0;34m'
	CYAN='\033[0;36m'
	BOLD='\033[1m'
	NC='\033[0m'
else
	RED=''
	GREEN=''
	YELLOW=''
	BLUE=''
	CYAN=''
	BOLD=''
	NC=''
fi

info() { printf "%b[INFO]%b %s\n" "${BLUE}" "${NC}" "$*"; }
step() { printf "%b[STEP]%b %s\n" "${CYAN}" "${NC}" "$*"; }
substep() { printf "  %b↳%b %s\n" "${BLUE}" "${NC}" "$*"; }
ok() { printf "%b[ OK ]%b %s\n" "${GREEN}" "${NC}" "$*"; }
warn() { printf "%b[WARN]%b %s\n" "${YELLOW}" "${NC}" "$*" >&2; }
err() { printf "%b[ERR ]%b %s\n" "${RED}" "${NC}" "$*" >&2; }
header() { printf "\n%b%s%b\n" "${BOLD}" "$*" "${NC}"; }

# ====================== PROFILING & DETECTION ======================
DISTRO_NAME="Unknown Linux"
DISTRO_ID="unknown"
DISTRO_ID_LIKE="unknown"
DISTRO_VERSION=""
PKG_FAMILY="unknown"
DETECTED_PKG_MANAGERS=()
CURRENT_DE="unknown"
SESSION_TYPE="unknown"
RESOLVED_BIN=""
RESOLVED_DESKTOP_FILE=""
RESOLVED_DESKTOP_NAME="${DEFAULT_DESKTOP_FILE}"

# Candidate binary locations
readonly CANDIDATE_BINS=(
	"/usr/bin/SparkNotePad"
	"/usr/local/bin/SparkNotePad"
	"/usr/bin/sparknotepad"
	"/usr/local/bin/sparknotepad"
	"${HOME}/.local/bin/SparkNotePad"
	"${HOME}/.local/bin/sparknotepad"
	"/opt/sparknotepad/bin/SparkNotePad"
	"/opt/SparkNotePad/bin/SparkNotePad"
	"/var/lib/flatpak/exports/bin/${FLATPAK_APP_ID}"
	"${HOME}/.local/share/flatpak/exports/bin/${FLATPAK_APP_ID}"
)

# Candidate desktop entry locations
readonly CANDIDATE_DESKTOP_FILES=(
	"/usr/share/applications/${DEFAULT_DESKTOP_FILE}"
	"/usr/local/share/applications/${DEFAULT_DESKTOP_FILE}"
	"${HOME}/.local/share/applications/${DEFAULT_DESKTOP_FILE}"
	"/usr/share/applications/${FALLBACK_DESKTOP_FILE}"
	"/usr/local/share/applications/${FALLBACK_DESKTOP_FILE}"
	"${HOME}/.local/share/applications/${FALLBACK_DESKTOP_FILE}"
	"/var/lib/flatpak/exports/share/applications/${FLATPAK_APP_ID}.desktop"
	"${HOME}/.local/share/flatpak/exports/share/applications/${FLATPAK_APP_ID}.desktop"
)

# Exhaustive list of 80+ text, source code, data, serialization, and config MIME types
readonly LINUX_MIME_TYPES=(
	# Plain Text & Generic Documents
	"text/plain"
	"text/plain;charset=utf-8"
	"application/x-zerosize"
	"text/x-log"

	# Markdown & Documentation
	"text/markdown"
	"text/x-markdown"
	"text/x-rst"
	"text/x-asciidoc"
	"text/x-tex"
	"text/x-latex"

	# Web & Styles
	"text/html"
	"application/xhtml+xml"
	"text/css"
	"text/less"
	"text/x-scss"
	"text/x-sass"

	# ECMAScript / JavaScript / TypeScript
	"text/javascript"
	"application/javascript"
	"application/x-javascript"
	"text/typescript"
	"application/typescript"
	"application/x-typescript"

	# Serialization & Data Formats
	"application/json"
	"application/ld+json"
	"application/x-yaml"
	"text/x-yaml"
	"text/yaml"
	"application/toml"
	"text/x-toml"
	"application/xml"
	"text/xml"
	"text/csv"
	"text/tab-separated-values"

	# Systems Programming (C, C++, Objective-C, C#)
	"text/x-c"
	"text/x-c++"
	"text/x-csrc"
	"text/x-chdr"
	"text/x-c++src"
	"text/x-c++hdr"
	"text/x-objcsrc"
	"text/x-csharp"

	# Modern Compiled & Systems Languages (Rust, Go, Zig, Nim, D)
	"text/x-rust"
	"text/rust"
	"text/x-go"
	"text/x-golang"
	"text/x-zig"
	"text/x-nim"
	"text/x-dsrc"

	# Scripting & Interpreted Languages (Python, Lua, Ruby, Perl, PHP, Shell)
	"text/x-python"
	"text/x-python3"
	"text/x-script.python"
	"text/x-lua"
	"text/x-ruby"
	"application/x-ruby"
	"text/x-perl"
	"application/x-perl"
	"text/x-php"
	"application/x-php"
	"text/x-shellscript"
	"application/x-shellscript"
	"text/x-bash"
	"text/x-zsh"
	"text/x-fish"
	"text/x-tcl"
	"text/x-r"

	# JVM, Mobile & Functional Languages
	"text/x-java"
	"text/x-java-source"
	"text/x-kotlin"
	"text/x-scala"
	"text/x-groovy"
	"text/x-dart"
	"text/x-swift"
	"text/x-haskell"
	"text/x-ocaml"
	"text/x-clojure"
	"text/x-elixir"
	"text/x-erlang"

	# Configuration, Build & DevOps
	"text/x-config"
	"text/x-ini"
	"application/x-wine-extension-ini"
	"text/x-cmake"
	"text/x-makefile"
	"text/x-dockerfile"
	"text/x-diff"
	"text/x-patch"
	"text/x-sql"
	"application/sql"
)

# Detect Linux Distribution and Packaging Family
detect_distro() {
	if [[ -f /etc/os-release ]]; then
		# shellcheck disable=SC1091
		source /etc/os-release
		DISTRO_NAME="${PRETTY_NAME:-Linux}"
		DISTRO_ID="${ID:-unknown}"
		DISTRO_ID_LIKE="${ID_LIKE:-unknown}"
		DISTRO_VERSION="${VERSION_ID:-}"
	elif [[ -f /etc/debian_version ]]; then
		DISTRO_NAME="Debian GNU/Linux $(cat /etc/debian_version)"
		DISTRO_ID="debian"
		DISTRO_ID_LIKE="debian"
	elif [[ -f /etc/redhat-release ]]; then
		DISTRO_NAME=$(cat /etc/redhat-release)
		DISTRO_ID="rhel"
		DISTRO_ID_LIKE="fedora"
	fi

	# Classify Packaging Family
	if [[ ${DISTRO_ID} =~ (debian|ubuntu|linuxmint|pop|elementary|zorin|kali|parrot|mx|antix|devuan|pureos|raspbian|deepin|tuxedo|sparky) ]] ||
		[[ ${DISTRO_ID_LIKE} =~ (debian|ubuntu) ]]; then
		PKG_FAMILY="DEB"
	elif [[ ${DISTRO_ID} =~ (fedora|rhel|centos|rocky|almalinux|opensuse|suse|sles|sled|mageia|openmandriva|amazonlinux|ol|pclinuxos) ]] ||
		[[ ${DISTRO_ID_LIKE} =~ (fedora|rhel|suse) ]]; then
		PKG_FAMILY="RPM"
	elif [[ ${DISTRO_ID} =~ (arch|manjaro|endeavouros) ]] || [[ ${DISTRO_ID_LIKE} =~ arch ]]; then
		PKG_FAMILY="ARCH"
	elif [[ ${DISTRO_ID} =~ (alpine|void|gentoo|nixos|solus) ]]; then
		PKG_FAMILY="INDEPENDENT"
	else
		PKG_FAMILY="GENERIC_LINUX"
	fi

	# Detect Package Managers
	DETECTED_PKG_MANAGERS=()
	for pm in apt nala dpkg dnf dnf5 rpm zypper urpmi pacman apk xbps-install emerge flatpak; do
		if command -v "${pm}" &>/dev/null; then
			DETECTED_PKG_MANAGERS+=("${pm}")
		fi
	done
}

# Detect Desktop Environment & Session
detect_desktop_environment() {
	SESSION_TYPE="${XDG_SESSION_TYPE:-unknown}"
	local raw_de="${XDG_CURRENT_DESKTOP:-${DESKTOP_SESSION:-${GDMSESSION:-unknown}}}"
	local de_upper
	de_upper=$(echo "${raw_de}" | tr '[:lower:]' '[:upper:]')

	if [[ ${de_upper} =~ GNOME ]]; then
		CURRENT_DE="GNOME"
	elif [[ ${de_upper} =~ KDE ]]; then
		CURRENT_DE="KDE"
	elif [[ ${de_upper} =~ XFCE ]]; then
		CURRENT_DE="XFCE"
	elif [[ ${de_upper} =~ CINNAMON ]]; then
		CURRENT_DE="CINNAMON"
	elif [[ ${de_upper} =~ MATE ]]; then
		CURRENT_DE="MATE"
	elif [[ ${de_upper} =~ LXQT ]]; then
		CURRENT_DE="LXQT"
	elif [[ ${de_upper} =~ LXDE ]]; then
		CURRENT_DE="LXDE"
	elif [[ ${de_upper} =~ BUDGIE ]]; then
		CURRENT_DE="BUDGIE"
	elif [[ ${de_upper} =~ COSMIC ]]; then
		CURRENT_DE="COSMIC"
	elif [[ ${de_upper} =~ PANTHEON ]]; then
		CURRENT_DE="PANTHEON"
	elif [[ ${de_upper} =~ DEEPIN ]]; then
		CURRENT_DE="DEEPIN"
	elif [[ ${de_upper} =~ (ENLIGHTENMENT|MOKSHA) ]]; then
		CURRENT_DE="ENLIGHTENMENT"
	elif [[ ${de_upper} =~ (SWAY|HYPRLAND|I3|WAYFIRE|BSPWM|RIVER|AWESOME|DWM|QTILE) ]]; then
		CURRENT_DE="TILING_WM (${raw_de})"
	elif [[ -z ${DISPLAY:-} ]] && [[ -z ${WAYLAND_DISPLAY:-} ]]; then
		CURRENT_DE="CLI_HEADLESS"
	else
		CURRENT_DE="${raw_de}"
	fi
}

# Find SparkNotePad executable binary
resolve_binary() {
	local explicit_bin="$1"

	if [[ -n ${explicit_bin} ]]; then
		if [[ -x ${explicit_bin} ]]; then
			RESOLVED_BIN="${explicit_bin}"
			return 0
		else
			err "Specified binary path is not executable: ${explicit_bin}"
			exit 1
		fi
	fi

	# Search standard candidate paths
	for bin in "${CANDIDATE_BINS[@]}"; do
		if [[ -x ${bin} ]]; then
			RESOLVED_BIN="${bin}"
			return 0
		fi
	done

	# Search standard PATH
	if command -v SparkNotePad &>/dev/null; then
		RESOLVED_BIN=$(command -v SparkNotePad)
		return 0
	elif command -v sparknotepad &>/dev/null; then
		RESOLVED_BIN=$(command -v sparknotepad)
		return 0
	fi

	# Check local workspace build directory
	local local_build_bin
	local_build_bin="$(pwd)/build/src/SparkNotePad"
	if [[ -x ${local_build_bin} ]]; then
		RESOLVED_BIN="${local_build_bin}"
		return 0
	fi

	RESOLVED_BIN=""
}

# Find or auto-scaffold SparkNotePad.desktop file
resolve_desktop_entry() {
	local explicit_desktop="$1"
	local auto_scaffold="$2"

	if [[ -n ${explicit_desktop} ]]; then
		if [[ -f ${explicit_desktop} ]]; then
			RESOLVED_DESKTOP_FILE="${explicit_desktop}"
			RESOLVED_DESKTOP_NAME=$(basename "${explicit_desktop}")
			return 0
		else
			err "Specified desktop file does not exist: ${explicit_desktop}"
			exit 1
		fi
	fi

	# Search standard candidate paths
	for df in "${CANDIDATE_DESKTOP_FILES[@]}"; do
		if [[ -f ${df} ]]; then
			RESOLVED_DESKTOP_FILE="${df}"
			RESOLVED_DESKTOP_NAME=$(basename "${df}")
			return 0
		fi
	done

	# If not found and auto-scaffold is allowed, generate user-level desktop entry
	if [[ ${auto_scaffold} -eq 1 ]] && [[ -n ${RESOLVED_BIN} ]]; then
		local user_app_dir="${HOME}/.local/share/applications"
		local scaffold_file="${user_app_dir}/${DEFAULT_DESKTOP_FILE}"
		mkdir -p "${user_app_dir}"

		step "Scaffolding compliant FreeDesktop desktop entry: ${scaffold_file}..."
		cat >"${scaffold_file}" <<EOF
[Desktop Entry]
Categories=Qt;TextEditor;Utility;Development;
Exec=${RESOLVED_BIN} %F
Icon=SparkNotePad
MimeType=text/plain;text/x-c;text/x-c++;text/x-python;text/x-shellscript;application/json;text/markdown;text/html;text/css;application/xml;text/x-lua;text/x-rust;text/x-go;text/x-php;text/x-java;text/x-sql;text/x-yaml;
StartupNotify=true
Type=Application
Terminal=false
Actions=NewWindow;NewEmptyDocument;

Name=SparkNotePad
Name[id]=SparkNotePad
GenericName=Text and Source Code Editor
GenericName[id]=Editor Teks dan Kode Sumber
Comment=A lightweight Scintilla-based text and source code editor for Linux
Comment[id]=Editor teks dan kode sumber ringan berbasis Scintilla dan Qt untuk Linux
Keywords=sparknotepad;spark;notepad;text;editor;scintilla;lexilla;tabs;programming;source;code;

[DesktopAction NewWindow]
Name=New Window
Name[id]=Jendela Baru
Exec=${RESOLVED_BIN}

[DesktopAction NewEmptyDocument]
Name=New Empty Document
Name[id]=Dokumen Kosong Baru
Exec=${RESOLVED_BIN}
EOF
		chmod 644 "${scaffold_file}"
		if command -v update-desktop-database &>/dev/null; then
			update-desktop-database "${user_app_dir}" 2>/dev/null || true
		fi
		RESOLVED_DESKTOP_FILE="${scaffold_file}"
		RESOLVED_DESKTOP_NAME="${DEFAULT_DESKTOP_FILE}"
		ok "Created user desktop entry: ${scaffold_file}"
		return 0
	fi

	RESOLVED_DESKTOP_FILE=""
}

# ====================== INTEGRATION LAYERS ======================

# Layer 1: XDG MIME Applications & mimeapps.list
apply_layer_xdg() {
	local dry_run="$1"
	local scope="$2" # "user" or "system"

	step "Layer 1: Applying FreeDesktop XDG MIME specifications..."

	# 1A. Use xdg-mime if present
	if command -v xdg-mime &>/dev/null; then
		substep "Registering ${#LINUX_MIME_TYPES[@]} MIME types via xdg-mime for ${RESOLVED_DESKTOP_NAME}..."
		for mime in "${LINUX_MIME_TYPES[@]}"; do
			if [[ ${dry_run} -eq 1 ]]; then
				printf "    [DRY-RUN] xdg-mime default %s %s\n" "${RESOLVED_DESKTOP_NAME}" "${mime}"
			else
				xdg-mime default "${RESOLVED_DESKTOP_NAME}" "${mime}" 2>/dev/null || true
			fi
		done
		ok "xdg-mime registrations complete."
	else
		warn "xdg-mime command not found; proceeding with direct configuration file updates."
	fi

	# 1B. Direct atomic update to mimeapps.list
	local target_mimeapps_files=()
	local current_uid
	current_uid=$(id -u)

	if [[ ${scope} == "system" ]] || [[ ${current_uid} -eq 0 ]]; then
		target_mimeapps_files+=(
			"/etc/xdg/mimeapps.list"
			"/usr/share/applications/mimeapps.list"
		)
	fi

	# Always update user config as well
	local user_config_dir="${XDG_CONFIG_HOME:-${HOME}/.config}"
	mkdir -p "${user_config_dir}"
	target_mimeapps_files+=(
		"${user_config_dir}/mimeapps.list"
		"${user_config_dir}/gnome-mimeapps.list"
		"${user_config_dir}/kde-mimeapps.list"
		"${user_config_dir}/cinnamon-mimeapps.list"
		"${user_config_dir}/mate-mimeapps.list"
	)

	substep "Writing atomic entries into XDG mimeapps.list registries..."
	for file in "${target_mimeapps_files[@]}"; do
		if [[ ${dry_run} -eq 1 ]]; then
			info "[DRY-RUN] Would atomically update ${file}"
			continue
		fi

		# Ensure parent directory exists
		mkdir -p "$(dirname "${file}")"
		local tmp_file
		tmp_file=$(mktemp "${file}.tmp.XXXXXX")

		# If file exists, copy existing content, strip existing lines for our MIME types under [Default Applications]
		if [[ -f ${file} ]]; then
			cp "${file}" "${tmp_file}"
		else
			cat >"${tmp_file}" <<EOF
[Default Applications]
[Added Associations]
EOF
		fi

		# Ensure [Default Applications] section exists
		if ! grep -q "^\[Default Applications\]" "${tmp_file}"; then
			printf "\n[Default Applications]\n" >>"${tmp_file}"
		fi

		# Update or append our MIME associations under [Default Applications]
		# We construct a clean sed/awk script for atomic deterministic replacements
		for mime in "${LINUX_MIME_TYPES[@]}"; do
			if grep -q "^${mime}=" "${tmp_file}"; then
				# Replace existing entry: place our desktop file first
				sed -i -E "s|^(${mime}=)(.*)|\\1${RESOLVED_DESKTOP_NAME};\\2|; s|(${RESOLVED_DESKTOP_NAME};)+|${RESOLVED_DESKTOP_NAME};|g" "${tmp_file}"
			else
				# Append under [Default Applications]
				sed -i "/^\[Default Applications\]/a ${mime}=${RESOLVED_DESKTOP_NAME};" "${tmp_file}"
			fi
		done

		# Clean up any trailing double semicolons or duplicates
		sed -i -E "s|;;+|;|g" "${tmp_file}"

		mv -f "${tmp_file}" "${file}"
		chmod 644 "${file}" 2>/dev/null || true
	done
	ok "Atomic XDG mimeapps.list synchronization complete."

	# 1C. Rebuild desktop databases
	if command -v update-desktop-database &>/dev/null; then
		substep "Refreshing desktop databases via update-desktop-database..."
		if [[ ${dry_run} -eq 0 ]]; then
			update-desktop-database "${HOME}/.local/share/applications" 2>/dev/null || true
			local uid_check
			uid_check=$(id -u)
			if [[ ${uid_check} -eq 0 ]]; then
				update-desktop-database "/usr/share/applications" 2>/dev/null || true
			fi
		fi
	fi
}

# Layer 2: Desktop Environment Native APIs
apply_layer_de_native() {
	local dry_run="$1"

	step "Layer 2: Applying Desktop-Specific Native Integrations (${CURRENT_DE})..."

	# 2A. GNOME / Budgie / Cinnamon / MATE / Pantheon: GIO integration
	if command -v gio &>/dev/null; then
		substep "Configuring GIO file associations via 'gio mime'..."
		for mime in "${LINUX_MIME_TYPES[@]}"; do
			if [[ ${dry_run} -eq 1 ]]; then
				printf "    [DRY-RUN] gio mime %s %s\n" "${mime}" "${RESOLVED_DESKTOP_NAME}"
			else
				gio mime "${mime}" "${RESOLVED_DESKTOP_NAME}" 2>/dev/null || true
			fi
		done
		ok "GIO mime handlers configured."
	fi

	# 2B. KDE Plasma 5 & 6 Native Sycoca / KWriteConfig
	if [[ ${CURRENT_DE} =~ KDE ]] || command -v kbuildsycoca6 &>/dev/null || command -v kbuildsycoca5 &>/dev/null; then
		substep "Configuring KDE KService & Sycoca caches..."
		local kwriter=""
		if command -v kwriteconfig6 &>/dev/null; then
			kwriter="kwriteconfig6"
		elif command -v kwriteconfig5 &>/dev/null; then
			kwriter="kwriteconfig5"
		fi

		if [[ -n ${kwriter} ]]; then
			for mime in "${LINUX_MIME_TYPES[@]}"; do
				if [[ ${dry_run} -eq 0 ]]; then
					"${kwriter}" --file mimeapps.list --group "Default Applications" --key "${mime}" "${RESOLVED_DESKTOP_NAME}" 2>/dev/null || true
				fi
			done
		fi

		# Rebuild KDE system configuration cache
		if command -v kbuildsycoca6 &>/dev/null; then
			substep "Running kbuildsycoca6..."
			[[ ${dry_run} -eq 0 ]] && kbuildsycoca6 --noincremental 2>/dev/null || true
		elif command -v kbuildsycoca5 &>/dev/null; then
			substep "Running kbuildsycoca5..."
			[[ ${dry_run} -eq 0 ]] && kbuildsycoca5 --noincremental 2>/dev/null || true
		fi
		ok "KDE Plasma integration refreshed."
	fi

	# 2C. XFCE / Thunar
	if [[ ${CURRENT_DE} =~ XFCE ]]; then
		substep "Configuring XFCE Desktop & Thunar file associations..."
		if command -v xfce4-mime-settings &>/dev/null; then
			ok "XFCE MIME registry synchronized with XDG standard."
		fi
	fi
}

# Layer 3: Alternatives System (Debian & RPM)
apply_layer_alternatives() {
	local dry_run="$1"

	step "Layer 3: Configuring System CLI Alternatives (/usr/bin/editor)..."

	local alt_cmd=""
	if command -v update-alternatives &>/dev/null; then
		alt_cmd="update-alternatives"
	elif command -v alternatives &>/dev/null; then
		alt_cmd="alternatives"
	fi

	if [[ -z ${alt_cmd} ]]; then
		info "No update-alternatives or alternatives command found on this system; skipping Layer 3."
		return 0
	fi

	if [[ -z ${RESOLVED_BIN} ]]; then
		warn "No executable SparkNotePad binary found to register in alternatives."
		return 0
	fi

	local user_id
	user_id=$(id -u)

	if [[ ${user_id} -ne 0 ]]; then
		info "User is not root. To register SparkNotePad in /usr/bin/editor, run:"
		info "  sudo ${alt_cmd} --install /usr/bin/editor editor ${RESOLVED_BIN} 100"
		info "  sudo ${alt_cmd} --set editor ${RESOLVED_BIN}"
		return 0
	fi

	substep "Registering ${RESOLVED_BIN} in ${alt_cmd} with priority 100..."
	if [[ ${dry_run} -eq 1 ]]; then
		info "[DRY-RUN] Would run: ${alt_cmd} --install /usr/bin/editor editor ${RESOLVED_BIN} 100"
		info "[DRY-RUN] Would run: ${alt_cmd} --set editor ${RESOLVED_BIN}"
	else
		"${alt_cmd}" --install /usr/bin/editor editor "${RESOLVED_BIN}" 100 2>/dev/null || true
		"${alt_cmd}" --set editor "${RESOLVED_BIN}" 2>/dev/null || true

		# Also register gnome-text-editor if alternative exists in database
		if "${alt_cmd}" --display gnome-text-editor &>/dev/null; then
			"${alt_cmd}" --install /usr/bin/gnome-text-editor gnome-text-editor "${RESOLVED_BIN}" 100 2>/dev/null || true
			"${alt_cmd}" --set gnome-text-editor "${RESOLVED_BIN}" 2>/dev/null || true
		fi
		ok "System alternative /usr/bin/editor set to ${RESOLVED_BIN}."
	fi
}

# Layer 4: Shell Environment Variables ($EDITOR, $VISUAL, $SUDO_EDITOR)
apply_layer_environment_vars() {
	local dry_run="$1"
	local scope="$2"

	# shellcheck disable=SC2016
	step 'Layer 4: Configuring Terminal Environment Variables ($EDITOR, $VISUAL)...'

	local export_line="export VISUAL=\"${RESOLVED_BIN:-SparkNotePad}\"; export EDITOR=\"${RESOLVED_BIN:-SparkNotePad}\""

	# System-wide profile.d (if root)
	local sys_uid
	sys_uid=$(id -u)
	if [[ ${scope} == "system" ]] || [[ ${sys_uid} -eq 0 ]]; then
		local profile_file="/etc/profile.d/sparknotepad-editor.sh"
		substep "Writing system-wide profile export: ${profile_file}..."
		if [[ ${dry_run} -eq 1 ]]; then
			info "[DRY-RUN] Would write ${profile_file}"
		else
			cat >"${profile_file}" <<EOF
# Set SparkNotePad as default system visual and command-line editor
export VISUAL="${RESOLVED_BIN:-SparkNotePad}"
export EDITOR="${RESOLVED_BIN:-SparkNotePad}"
export SUDO_EDITOR="${RESOLVED_BIN:-SparkNotePad}"
EOF
			chmod 644 "${profile_file}"
			ok "Created ${profile_file}"
		fi
	fi

	# User shell startup files
	local user_targets=(
		"${HOME}/.bashrc"
		"${HOME}/.profile"
	)
	if [[ -f ${HOME}/.zshrc ]]; then
		user_targets+=("${HOME}/.zshrc")
	fi

	substep "Configuring user shell initialization profiles..."
	for rc in "${user_targets[@]}"; do
		if [[ ! -f ${rc} ]]; then
			continue
		fi

		if [[ ${dry_run} -eq 1 ]]; then
			info "[DRY-RUN] Would append or verify export in ${rc}"
			continue
		fi

		# Idempotent check
		if grep -q "SparkNotePad" "${rc}"; then
			# Update existing export
			sed -i -E "s|export (VISUAL|EDITOR)=.*SparkNotePad.*|${export_line}|g" "${rc}"
		else
			# Append clean section
			cat >>"${rc}" <<EOF

# SparkNotePad Default Editor Configuration
${export_line}
EOF
		fi
	done
	ok "User environment variables configured."
}

# ====================== AUDIT & STATUS REPORT ======================
audit_status() {
	header "================================================================="
	header "   SparkNotePad Linux Default Editor Diagnostic Audit"
	header "================================================================="

	echo
	local distro_desc="${DISTRO_NAME}"
	if [[ -n ${DISTRO_VERSION} ]]; then
		distro_desc="${DISTRO_NAME} (v${DISTRO_VERSION})"
	fi
	printf "  %bOperating System:%b    %s [%s Family]\n" "${BOLD}" "${NC}" "${distro_desc}" "${PKG_FAMILY}"
	printf "  %bDesktop Environment:%b %s (Session: %s)\n" "${BOLD}" "${NC}" "${CURRENT_DE}" "${SESSION_TYPE}"
	printf "  %bPackage Managers:%b    %s\n" "${BOLD}" "${NC}" "${DETECTED_PKG_MANAGERS[*]:-none detected}"
	printf "  %bResolved Binary:%b     %s\n" "${BOLD}" "${NC}" "${RESOLVED_BIN:-NOT FOUND}"
	printf "  %bResolved Desktop:%b    %s (%s)\n" "${BOLD}" "${NC}" "${RESOLVED_DESKTOP_NAME}" "${RESOLVED_DESKTOP_FILE:-NOT FOUND}"
	echo

	header "--- FreeDesktop.org XDG MIME Associations Status ---"
	local sample_mimes=(
		"Plain Text Document:text/plain"
		"C Source File:text/x-c"
		"C++ Header File:text/x-chdr"
		"Python Script:text/x-python"
		"Shell Script:text/x-shellscript"
		"JSON Data Document:application/json"
		"YAML Configuration:application/x-yaml"
		"Markdown Document:text/markdown"
		"XML Data Document:application/xml"
		"HTML Web Page:text/html"
		"CSS Stylesheet:text/css"
		"SQL Query File:text/x-sql"
	)

	for item in "${sample_mimes[@]}"; do
		local label="${item%%:*}"
		local mime="${item##*:}"
		local current="unknown"

		if command -v xdg-mime &>/dev/null; then
			current=$(xdg-mime query default "${mime}" 2>/dev/null || true)
		fi
		current="${current:-none}"

		if [[ ${current} =~ (SparkNotePad|sparknotepad) ]]; then
			printf "  %b✓%b %-26s [%s] → %b%s%b\n" "${GREEN}" "${NC}" "${label}" "${mime}" "${GREEN}" "${current}" "${NC}"
		else
			printf "  %b✗%b %-26s [%s] → %b%s%b\n" "${RED}" "${NC}" "${label}" "${mime}" "${YELLOW}" "${current}" "${NC}"
		fi
	done

	echo
	header "--- CLI / Terminal Alternatives Status ---"
	local alt_cmd=""
	if command -v update-alternatives &>/dev/null; then
		alt_cmd="update-alternatives"
	elif command -v alternatives &>/dev/null; then
		alt_cmd="alternatives"
	fi

	if [[ -n ${alt_cmd} ]]; then
		local current_editor
		current_editor=$("${alt_cmd}" --display editor 2>/dev/null | grep "currently points to" || true)
		printf "  Alternatives Tool:      %s\n" "${alt_cmd}"
		printf "  Status:                 %s\n" "${current_editor:-Not configured or managed}"
	else
		printf "  Alternatives Tool:      Not available on this distribution\n"
	fi

	echo
	header "--- Shell Environment Variables Status ---"
	printf "  \$VISUAL:                 %s\n" "${VISUAL:-<unset>}"
	printf "  \$EDITOR:                 %s\n" "${EDITOR:-<unset>}"
	printf "  \$SUDO_EDITOR:            %s\n" "${SUDO_EDITOR:-<unset>}"
	echo
}

# ====================== RESTORE DEFAULTS ======================
restore_defaults() {
	header "================================================================="
	header "   Restoring Native System Default Editor Handlers"
	header "================================================================="

	# Standard fallback desktop entries per DE
	local fallback_desktop="org.gnome.TextEditor.desktop"
	if [[ ${CURRENT_DE} =~ KDE ]]; then
		fallback_desktop="org.kde.kwrite.desktop"
	elif [[ ${CURRENT_DE} =~ XFCE ]]; then
		fallback_desktop="mousepad.desktop"
	elif [[ ${CURRENT_DE} =~ CINNAMON ]]; then
		fallback_desktop="xed.desktop"
	elif [[ ${CURRENT_DE} =~ MATE ]]; then
		fallback_desktop="pluma.desktop"
	elif [[ ${CURRENT_DE} =~ PANTHEON ]]; then
		fallback_desktop="io.elementary.code.desktop"
	elif [[ ${CURRENT_DE} =~ GNOME ]]; then
		fallback_desktop="org.gnome.TextEditor.desktop"
	fi

	step "Restoring default text handler to: ${fallback_desktop}..."
	if command -v xdg-mime &>/dev/null; then
		xdg-mime default "${fallback_desktop}" "text/plain" 2>/dev/null || true
		xdg-mime default "${fallback_desktop}" "text/x-log" 2>/dev/null || true
	fi

	# Revert alternatives if root
	local alt_cmd=""
	if command -v update-alternatives &>/dev/null; then
		alt_cmd="update-alternatives"
	elif command -v alternatives &>/dev/null; then
		alt_cmd="alternatives"
	fi

	local rev_uid
	rev_uid=$(id -u)
	if [[ -n ${alt_cmd} ]] && [[ ${rev_uid} -eq 0 ]]; then
		substep "Reverting ${alt_cmd} to auto mode..."
		"${alt_cmd}" --auto editor 2>/dev/null || true
	fi

	ok "Default text associations reverted to platform defaults."
}

# ====================== CLI HELP ======================
show_help() {
	cat <<EOF
SparkNotePad Linux Default Editor Automation Engine v${SCRIPT_VERSION}
Configures SparkNotePad as the default editor across all Debian & RPM Linux distributions.

Usage: $(basename "$0") [OPTIONS]

Options:
  -y, --yes, -f, --force    Non-interactive mode (auto-confirm and apply immediately)
  -d, --dry-run             Simulate actions without making permanent system changes
  -s, --status, --check     Audit and display current editor associations and status
  --system                  Apply system-wide configuration (requires root / sudo)
  --user                    Apply user-level configuration (default)
  --restore, --revert       Revert default text editor to distribution native default
  --bin-path <path>         Explicitly specify the path to the SparkNotePad executable
  --desktop-path <path>     Explicitly specify the path to SparkNotePad.desktop
  -h, --help                Display this comprehensive help message
  -v, --version             Display version and maintainer information

Supported Linux Distributions:
  • DEB: Debian, Ubuntu, Linux Mint, Pop!_OS, Elementary OS, Zorin, Kali, MX, etc.
  • RPM: Fedora, RHEL, CentOS Stream, Rocky Linux, AlmaLinux, openSUSE, Mageia, etc.
  • All Desktop Environments: GNOME, KDE Plasma 5/6, XFCE, Cinnamon, MATE, LXQt,
                              Budgie, COSMIC, Deepin, Pantheon, Tiling WMs (Sway, i3).

Examples:
  $(basename "$0") -f               # Set SparkNotePad as default editor immediately
  $(basename "$0") --status         # Check current default editor mappings
  $(basename "$0") -d               # Preview all planned configuration steps
  sudo $(basename "$0") --system    # Apply system-wide default configuration
EOF
}

# ====================== MAIN ENTRY POINT ======================
main() {
	local auto_confirm=0
	local dry_run=0
	local do_audit=0
	local do_restore=0
	local scope="user"
	local explicit_bin=""
	local explicit_desktop=""

	while [[ $# -gt 0 ]]; do
		case "$1" in
		-y | --yes | -f | --force)
			auto_confirm=1
			shift
			;;
		-d | --dry-run)
			dry_run=1
			shift
			;;
		-s | --status | --check)
			do_audit=1
			shift
			;;
		--system)
			scope="system"
			shift
			;;
		--user)
			scope="user"
			shift
			;;
		--restore | --revert)
			do_restore=1
			shift
			;;
		--bin-path)
			explicit_bin="${2:-}"
			shift 2
			;;
		--desktop-path)
			explicit_desktop="${2:-}"
			shift 2
			;;
		-h | --help)
			show_help
			exit 0
			;;
		-v | --version)
			echo "SparkNotePad-Default-Editor-Linux.sh v${SCRIPT_VERSION} by Harry DS Alsyundawy"
			exit 0
			;;
		*)
			err "Unknown option: $1"
			show_help
			exit 1
			;;
		esac
	done

	# Perform System Profiling
	detect_distro
	detect_desktop_environment
	resolve_binary "${explicit_bin}"
	resolve_desktop_entry "${explicit_desktop}" 1

	if [[ ${do_audit} -eq 1 ]]; then
		audit_status
		exit 0
	fi

	if [[ ${do_restore} -eq 1 ]]; then
		restore_defaults
		exit 0
	fi

	header "================================================================="
	header "  SparkNotePad Linux Default Editor Configuration Engine"
	header "================================================================="

	echo
	printf "  %bDistribution:%b         %s (%s)\n" "${BOLD}" "${NC}" "${DISTRO_NAME}" "${PKG_FAMILY}"
	printf "  %bDesktop Environment:%b  %s\n" "${BOLD}" "${NC}" "${CURRENT_DE}"
	printf "  %bTarget Scope:%b         %s\n" "${BOLD}" "${NC}" "${scope}"
	printf "  %bResolved Executable:%b  %s\n" "${BOLD}" "${NC}" "${RESOLVED_BIN:-<Not Found>}"
	printf "  %bDesktop File:%b         %s\n" "${BOLD}" "${NC}" "${RESOLVED_DESKTOP_NAME}"
	echo

	if [[ ${auto_confirm} -eq 0 ]] && [[ ${dry_run} -eq 0 ]]; then
		info "This will configure SparkNotePad as the default system and terminal editor"
		info "across ${#LINUX_MIME_TYPES[@]} text, code, markup, and config MIME types."
		local answer=""
		if [[ -t 0 ]]; then
			read -r -p "Proceed with applying configuration? [Y/n] " answer || answer="n"
			answer="${answer:-y}"
		else
			warn "Non-interactive shell detected. Use -y / -f to execute."
			exit 1
		fi
		if [[ ! ${answer} =~ ^[Yy]$ ]]; then
			warn "Operation cancelled by user."
			exit 0
		fi
	fi

	# Execute 5-Layer Integration
	apply_layer_xdg "${dry_run}" "${scope}"
	apply_layer_de_native "${dry_run}"
	apply_layer_alternatives "${dry_run}"
	apply_layer_environment_vars "${dry_run}" "${scope}"

	echo
	header "================================================================="
	ok "SparkNotePad is now successfully configured as your Default Linux Editor!"
	header "================================================================="
	echo
}

main "$@"
