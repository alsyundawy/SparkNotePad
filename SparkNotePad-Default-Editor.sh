#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: SparkNotePad-Default-Editor.sh
# DESCRIPTION: Production-grade cross-platform script to set SparkNotePad as the
#              default system handler for all text, code, scripts, configs,
#              markup, dotfiles, and extensionless documents on macOS & Linux.
# AUTHOR:      Harry DS Alsyundawy (SparkNotePad Maintainer)
# VERSION:     2.0.0
# LICENSE:     GPL-3.0-or-later / MIT
# ==============================================================================
#
# DOCNOTE & TECHNICAL ARCHITECTURE
# ================================
#
# 1. OVERVIEW
# -----------
# On macOS, LaunchServices manages default file handlers. By default, Apple sets
# TextEdit as fallback for plain text, developer source code, and unknown/extensionless
# files (public.data). On Linux, FreeDesktop xdg-mime and update-alternatives
# govern desktop and terminal editor assignments.
#
# This script executes a complete, deterministic takeover for SparkNotePad:
# - macOS: com.alsyundawy.SparkNotePad (SparkNotePad.app)
#   Applies batch LaunchServices registrations via duti, unblocks Gatekeeper
#   quarantine flags, preserves macOS 13+ ExtensionKit System Settings plugins,
#   rebuilds user LaunchServices cache, and refreshes Finder and Dock.
# - Linux: SparkNotePad.desktop
#   Configures xdg-mime associations for 40+ programming & text MIME types,
#   and configures update-alternatives for /usr/bin/editor if running with root.
#
# 2. KEY COMMAND-LINE OPTIONS
# ---------------------------
#   -y, --yes, -f, --force   Non-interactive mode (auto-confirm & force replace)
#   -d, --dry-run            Simulate operations without modifying system settings
#   -s, --status, --check    Audit and report current default editor associations
#   --restore, --revert      Restore system defaults (TextEdit on macOS)
#   --app-path <path>        Explicit path to SparkNotePad.app
#   --no-rebuild-cache       Skip rebuilding LaunchServices database cache (macOS)
#   --no-restart             Skip restarting Finder and Dock at completion (macOS)
#   -h, --help               Display help and usage information
#   -v, --version            Display version information
# ==============================================================================

set -Eeuo pipefail
IFS=$'\n\t'

# ====================== ENVIRONMENT & PATH ======================
export PATH="/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:${HOME}/Library/FlyEnv/env/node/bin:${PATH:-}"

# ====================== CONFIGURATION ======================
readonly SCRIPT_VERSION="2.0.0"
readonly BUNDLE_ID="io.github.alsyundawy.SparkNotePad"
readonly APP_NAME="SparkNotePad.app"
readonly DESKTOP_FILE="SparkNotePad.desktop"

# Candidate search paths for macOS app bundle
readonly CANDIDATE_APP_PATHS=(
	"/Applications/${APP_NAME}"
	"${HOME}/Applications/${APP_NAME}"
	"/Applications/SparkNotePad-MacOS.app"
	"${HOME}/Applications/SparkNotePad-MacOS.app"
	"$(pwd)/build/${APP_NAME}"
	"$(pwd)/${APP_NAME}"
)

APP_RESOLVED_PATH=""
ACTIVE_BUNDLE_ID="${BUNDLE_ID}"
TMP_BATCH_CONFIG=""
PLATFORM="unknown"

# Colors
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
ok() { printf "%b[ OK ]%b %s\n" "${GREEN}" "${NC}" "$*"; }
warn() { printf "%b[WARN]%b %s\n" "${YELLOW}" "${NC}" "$*" >&2; }
err() { printf "%b[ERR ]%b %s\n" "${RED}" "${NC}" "$*" >&2; }
header() { printf "%b%s%b\n" "${BOLD}" "$*" "${NC}"; }

detect_platform() {
	local uname_s
	uname_s=$(uname -s)
	case "${uname_s}" in
	Darwin) PLATFORM="macos" ;;
	Linux) PLATFORM="linux" ;;
	*) PLATFORM="unsupported" ;;
	esac
}

cleanup() {
	if [[ -n ${TMP_BATCH_CONFIG} ]] && [[ -f ${TMP_BATCH_CONFIG} ]]; then
		rm -f "${TMP_BATCH_CONFIG}"
	fi
}
trap cleanup EXIT INT TERM HUP

# Comprehensive UTIs targeted for SparkNotePad on macOS
readonly UTIS=(
	"public.data"
	"public.plain-text"
	"public.text"
	"public.utf8-plain-text"
	"public.utf16-plain-text"
	"public.utf16-external-plain-text"
	"public.delimited-values-text"
	"public.comma-separated-values-text"
	"public.tab-separated-values-text"
	"public.rtf"
	"com.apple.traditional-mac-plain-text"
	"public.source-code"
	"public.script"
	"public.shell-script"
	"public.bash-script"
	"public.zsh-script"
	"public.csh-script"
	"com.apple.applescript.text"
	"public.c-source"
	"public.c-header"
	"public.c-plus-plus-source"
	"public.c-plus-plus-header"
	"public.objective-c-source"
	"public.objective-c-plus-plus-source"
	"public.swift-source"
	"public.assembly-source"
	"public.python-script"
	"public.perl-script"
	"public.ruby-script"
	"public.php-script"
	"com.netscape.javascript-source"
	"com.sun.java-source"
	"public.json"
	"public.xml"
	"public.css"
	"public.html"
	"public.yaml"
	"org.rust-lang.rust-source"
	"org.golang.go-source"
)

# Comprehensive file extensions
readonly EXTENSIONS=(
	# Plain Text & Notes
	"txt" "text" "log" "out" "err" "nfo" "diz" "me" "1st" "ans" "asc" "ascii"
	"readme" "changelog" "license" "authors" "todo" "notes"

	# Markdown & Documentation
	"md" "markdown" "mdown" "mkd" "mkdn" "mdwn" "mdtxt" "mdtext" "rst" "adoc"
	"asciidoc" "tex" "latex" "ltx" "org" "wiki" "textile"

	# Web & Front-End
	"html" "htm" "xhtml" "shtml" "shtm" "phtml" "css" "scss" "sass" "less"
	"styl" "stylus" "js" "mjs" "cjs" "jsx" "ts" "mts" "cts" "tsx" "vue"
	"svelte" "astro" "json" "json5" "jsonc" "jsonl" "ndjson" "map" "svg"

	# Config, Serialization & Data
	"yaml" "yml" "toml" "ini" "cfg" "conf" "config" "cnf" "properties" "props"
	"xml" "plist" "env" "editorconfig" "gitignore" "gitattributes" "gitmodules"
	"dockerignore" "npmignore" "slugignore" "csv" "tsv" "tab" "sql" "cql"

	# Systems Programming (C, C++, Rust, Zig, Go)
	"c" "h" "cpp" "hpp" "cc" "hh" "cxx" "hxx" "c++" "h++" "m" "mm"
	"rs" "rlib" "zig" "go" "mod" "sum" "s" "asm"

	# JVM & .NET Languages
	"java" "class" "kt" "kts" "scala" "sc" "groovy" "gvy" "gradle" "clj" "cljs"
	"cljc" "edn" "cs" "csx" "fs" "fsi" "fsx" "vb"

	# Scripting & Dynamic Languages
	"py" "pyw" "pyi" "pyx" "pxd" "rb" "rbw" "rake" "gemspec" "php" "phtml"
	"pl" "pm" "t" "pod" "lua" "tcl" "r" "rmd" "jl" "dart" "ex" "exs" "erl" "hrl"

	# Shell & Automation
	"sh" "bash" "zsh" "fish" "ksh" "csh" "tcsh" "awk" "sed" "bat" "cmd" "ps1" "psm1"
	"make" "makefile" "mk" "cmake" "dockerfile" "vagrantfile" "gemfile" "procfile"
)

# Linux MIME types
readonly LINUX_MIME_TYPES=(
	"text/plain"
	"text/x-c"
	"text/x-c++"
	"text/x-csrc"
	"text/x-chdr"
	"text/x-c++src"
	"text/x-c++hdr"
	"text/x-python"
	"text/x-shellscript"
	"application/json"
	"text/markdown"
	"text/html"
	"text/css"
	"application/xml"
	"text/x-lua"
	"text/x-rust"
	"text/x-go"
	"text/x-php"
	"text/x-java"
	"text/x-sql"
	"text/x-yaml"
	"text/javascript"
	"application/javascript"
	"application/x-shellscript"
	"application/x-yaml"
	"application/toml"
)

# Resolve application bundle path on macOS
resolve_app() {
	if [[ -n ${APP_RESOLVED_PATH} ]]; then
		if [[ -d ${APP_RESOLVED_PATH} ]]; then
			info "Using user-specified application path: ${APP_RESOLVED_PATH}"
			return 0
		else
			err "Specified application path does not exist: ${APP_RESOLVED_PATH}"
			exit 1
		fi
	fi

	for p in "${CANDIDATE_APP_PATHS[@]}"; do
		if [[ -d ${p} ]]; then
			APP_RESOLVED_PATH="${p}"
			ACTIVE_BUNDLE_ID="${BUNDLE_ID}"
			ok "Resolved application: ${APP_RESOLVED_PATH} (${ACTIVE_BUNDLE_ID})"
			return 0
		fi
	done

	err "SparkNotePad.app was not found in candidate paths."
	err "Please build or install SparkNotePad.app, or pass --app-path /path/to/SparkNotePad.app"
	exit 1
}

# Unquarantine app on macOS
unquarantine_app() {
	local app_path="$1"
	local dry_run="$2"

	if [[ ${dry_run} -eq 1 ]]; then
		info "[DRY-RUN] Would remove com.apple.quarantine from: ${app_path}"
		return 0
	fi

	if xattr -p com.apple.quarantine "${app_path}" &>/dev/null; then
		step "Removing com.apple.quarantine attribute from: ${app_path}"
		xattr -dr com.apple.quarantine "${app_path}" 2>/dev/null || true
		ok "Quarantine attribute stripped successfully."
	fi
}

get_lsregister_path() {
	local candidates=(
		"/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister"
		"/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister"
	)
	for p in "${candidates[@]}"; do
		if [[ -x ${p} ]]; then
			echo "${p}"
			return 0
		fi
	done
	err "lsregister binary not found in standard macOS system paths."
	exit 1
}

ensure_duti() {
	if command -v duti &>/dev/null; then
		return 0
	fi

	warn "duti is not installed. Attempting to install via Homebrew..."
	if command -v brew &>/dev/null; then
		brew install duti
		ok "duti installed successfully."
	else
		err "Homebrew is not installed. Please install duti manually: brew install duti"
		exit 1
	fi
}

# macOS LaunchServices cache rebuild preserving ExtensionKit
rebuild_launchservices_cache() {
	local lsregister="$1"
	local app_path="$2"
	local dry_run="$3"

	if [[ ${dry_run} -eq 1 ]]; then
		info "[DRY-RUN] Would reset user LaunchServices cache safely."
		return 0
	fi

	step "Purging stale user LaunchServices cache (-domain user only)..."
	"${lsregister}" -kill -r -domain user 2>/dev/null || true

	step "Re-seeding LaunchServices system & local domains..."
	"${lsregister}" -seed -domain system -domain local -domain user 2>/dev/null || true

	step "Re-registering ExtensionKit & System Settings plugins to guarantee preference integrity..."
	if [[ -d "/System/Library/ExtensionKit/Extensions" ]]; then
		"${lsregister}" -f -R -domain system "/System/Library/ExtensionKit/Extensions" 2>/dev/null || true
	fi
	if [[ -d "/System/Applications/System Settings.app" ]]; then
		"${lsregister}" -f -domain system "/System/Applications/System Settings.app" 2>/dev/null || true
	fi

	ok "LaunchServices cache safely reset and system extensions re-seeded."
}

register_app_launchservices() {
	local lsregister="$1"
	local app_path="$2"
	local dry_run="$3"

	if [[ ${dry_run} -eq 1 ]]; then
		info "[DRY-RUN] Would register with lsregister: ${app_path}"
		return 0
	fi

	"${lsregister}" -f "${app_path}" 2>/dev/null || true
	ok "Application registered with LaunchServices."
}

# Batch apply handlers on macOS via duti
apply_batch_handlers_macos() {
	local dry_run="$1"

	TMP_BATCH_CONFIG=$(mktemp -t sparknotepadpad_duti_XXXXXX)
	chmod 600 "${TMP_BATCH_CONFIG}"

	{
		for u in "${UTIS[@]}"; do
			printf "%s %s all\n" "${ACTIVE_BUNDLE_ID}" "${u}"
		done
		for e in "${EXTENSIONS[@]}"; do
			printf "%s .%s all\n" "${ACTIVE_BUNDLE_ID}" "${e}"
		done
	} >"${TMP_BATCH_CONFIG}"

	if [[ ${dry_run} -eq 1 ]]; then
		local line_count
		line_count=$(wc -l <"${TMP_BATCH_CONFIG}" | tr -d ' ')
		info "[DRY-RUN] Would apply batch config with ${line_count} mappings via duti."
		return 0
	fi

	step "Executing atomic batch registration via duti..."
	duti "${TMP_BATCH_CONFIG}"
	ok "Applied ${#UTIS[@]} UTIs and ${#EXTENSIONS[@]} file extensions to ${ACTIVE_BUNDLE_ID}."
}

restart_desktop_services_macos() {
	local dry_run="$1"

	if [[ ${dry_run} -eq 1 ]]; then
		info "[DRY-RUN] Would restart Finder and Dock."
		return 0
	fi

	step "Refreshing Finder and Dock..."
	killall Finder 2>/dev/null || true
	killall Dock 2>/dev/null || true
	ok "Desktop services refreshed."
}

# Audit & check current defaults
audit_status_macos() {
	ensure_duti
	header "======================================================"
	header "  SparkNotePad Default Editor Association Audit (macOS)"
	header "======================================================"
	echo

	local check_list=(
		"public.data (Non-Ext/Unknown):public.data"
		"public.plain-text (UTI):public.plain-text"
		"public.text (UTI):public.text"
		"public.source-code (UTI):public.source-code"
		".txt extension:txt"
		".py extension:py"
		".js extension:js"
		".json extension:json"
		".yaml extension:yaml"
		".md extension:md"
		".sh extension:sh"
		".env extension:env"
	)

	local item label query current
	for item in "${check_list[@]}"; do
		label="${item%%:*}"
		query="${item##*:}"
		current=""

		if [[ ${query} == public.* || ${query} == com.* || ${query} == org.* ]]; then
			current=$(duti -d "${query}" 2>/dev/null || true)
		else
			current=$(duti -x "${query}" 2>/dev/null | head -n 3 | tail -n 1 || true)
		fi
		current="${current:-unknown}"

		if [[ ${current} == *"${BUNDLE_ID}"* ]]; then
			printf "  %b✓%b %-30s → SparkNotePad (%s)\n" "${GREEN}" "${NC}" "${label}" "${current}"
		else
			printf "  %b✗%b %-30s → %s\n" "${RED}" "${NC}" "${label}" "${current}"
		fi
	done
	echo
}

# Restore default text editor on macOS (TextEdit)
restore_defaults_macos() {
	ensure_duti
	local textedit_id="com.apple.TextEdit"
	header "Restoring macOS defaults to TextEdit (${textedit_id})..."
	duti -s "${textedit_id}" "public.plain-text" all
	duti -s "${textedit_id}" "public.text" all
	duti -s "${textedit_id}" "txt" all
	ok "Basic plain-text associations restored to Apple TextEdit."
}

# Linux implementation
apply_linux() {
	local dry_run="$1"
	local auto_confirm="$2"

	local script_dir
	script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	local linux_script="${script_dir}/SparkNotePad-Default-Editor-Linux.sh"
	if [[ ! -f ${linux_script} ]]; then
		linux_script="${script_dir}/scripts/SparkNotePad-Default-Editor-Linux.sh"
	fi

	if [[ -x ${linux_script} ]] || [[ -f ${linux_script} ]]; then
		local forward_args=()
		if [[ ${auto_confirm} -eq 1 ]]; then
			forward_args+=("-f")
		fi
		if [[ ${dry_run} -eq 1 ]]; then
			forward_args+=("-d")
		fi
		exec bash "${linux_script}" "${forward_args[@]}"
	fi

	header "======================================================"
	header "  Set SparkNotePad as Default Editor (Linux)"
	header "======================================================"
	echo

	if ! command -v xdg-mime &>/dev/null; then
		err "xdg-mime command not found. Please install xdg-utils."
		exit 1
	fi

	if [[ ${auto_confirm} -eq 0 ]] && [[ ${dry_run} -eq 0 ]]; then
		info "This will associate ${DESKTOP_FILE} with ${#LINUX_MIME_TYPES[@]} text/code MIME types."
		local answer=""
		if [[ -t 0 ]]; then
			read -r -p "Proceed with setting SparkNotePad as default? [y/N] " answer || answer="n"
		else
			warn "Non-interactive shell. Pass -y / -f to proceed."
			exit 1
		fi
		if [[ ! ${answer} =~ ^[Yy]$ ]]; then
			warn "Operation cancelled by user."
			exit 0
		fi
	fi

	step "Setting xdg-mime associations for ${DESKTOP_FILE}..."
	for mime in "${LINUX_MIME_TYPES[@]}"; do
		if [[ ${dry_run} -eq 1 ]]; then
			printf "  [DRY-RUN] xdg-mime default %s %s\n" "${DESKTOP_FILE}" "${mime}"
		else
			xdg-mime default "${DESKTOP_FILE}" "${mime}" 2>/dev/null || true
		fi
	done

	# Configure update-alternatives if running as root or with sudo
	if command -v update-alternatives &>/dev/null; then
		local binary_path=""
		for bin in "/usr/local/bin/SparkNotePad" "/usr/bin/SparkNotePad" "/usr/local/bin/sparknotepad" "/usr/bin/sparknotepad"; do
			if [[ -x ${bin} ]]; then
				binary_path="${bin}"
				break
			fi
		done
		local user_id
		user_id=$(id -u)
		if [[ -n ${binary_path} ]] && [[ ${user_id} -eq 0 ]]; then
			step "Registering /usr/bin/editor alternative with: ${binary_path}..."
			update-alternatives --install /usr/bin/editor editor "${binary_path}" 50 2>/dev/null || true
			update-alternatives --set editor "${binary_path}" 2>/dev/null || true
			ok "System terminal editor alternative configured."
		fi
	fi

	ok "Linux desktop associations successfully updated for SparkNotePad."
}

show_help() {
	cat <<EOF
SparkNotePad Default Editor Automation Tool v${SCRIPT_VERSION}

Usage: $(basename "$0") [OPTIONS]

Options:
  -y, --yes, -f, --force   Non-interactive force mode (no prompts)
  -d, --dry-run            Simulate operations without making changes
  -s, --status, --check    Audit and report current default editor associations
  --restore, --revert      Restore system defaults (TextEdit on macOS)
  --app-path <path>        Custom path to SparkNotePad.app (macOS)
  --no-rebuild-cache       Skip LaunchServices cache rebuild (macOS)
  --no-restart             Skip restarting Finder/Dock (macOS)
  -h, --help               Display this help message
  -v, --version            Display version information

Examples:
  ./SparkNotePad-Default-Editor.sh -f               # Set SparkNotePad as default non-interactively
  ./SparkNotePad-Default-Editor.sh --status         # Check current associations
  ./SparkNotePad-Default-Editor.sh -d               # Dry-run preview
EOF
}

# Main entrypoint
main() {
	detect_platform

	local auto_confirm=0
	local dry_run=0
	local rebuild_cache=1
	local restart_services=1
	local audit_only=0
	local restore_only=0

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
			audit_only=1
			shift
			;;
		--restore | --revert)
			restore_only=1
			shift
			;;
		--no-rebuild-cache)
			rebuild_cache=0
			shift
			;;
		--no-restart)
			restart_services=0
			shift
			;;
		--app-path)
			if [[ $# -lt 2 ]]; then
				err "--app-path requires a path argument"
				exit 1
			fi
			APP_RESOLVED_PATH="$2"
			shift 2
			;;
		-h | --help)
			show_help
			exit 0
			;;
		-v | --version)
			echo "SparkNotePad-Default-Editor.sh v${SCRIPT_VERSION}"
			exit 0
			;;
		*)
			err "Unknown option: $1"
			show_help
			exit 1
			;;
		esac
	done

	if [[ ${PLATFORM} == "macos" ]]; then
		if [[ ${audit_only} -eq 1 ]]; then
			audit_status_macos
			exit 0
		fi
		if [[ ${restore_only} -eq 1 ]]; then
			restore_defaults_macos
			exit 0
		fi

		header "======================================================"
		header "  Set SparkNotePad as Default Editor (macOS Force Mode)"
		header "======================================================"
		echo

		resolve_app
		unquarantine_app "${APP_RESOLVED_PATH}" "${dry_run}"
		ensure_duti

		local lsregister_bin
		lsregister_bin=$(get_lsregister_path)

		if [[ ${auto_confirm} -eq 0 ]] && [[ ${dry_run} -eq 0 ]]; then
			info "This will configure LaunchServices to set SparkNotePad (${ACTIVE_BUNDLE_ID})"
			info "as default editor for 38 UTIs and 470+ file extensions."
			local answer=""
			if [[ -t 0 ]]; then
				read -r -p "Proceed with setting SparkNotePad as default editor? [y/N] " answer || answer="n"
			else
				warn "Non-interactive shell. Pass -y / -f to proceed."
				exit 1
			fi
			if [[ ! ${answer} =~ ^[Yy]$ ]]; then
				warn "Operation cancelled by user."
				exit 0
			fi
		fi

		if [[ ${rebuild_cache} -eq 1 ]]; then
			rebuild_launchservices_cache "${lsregister_bin}" "${APP_RESOLVED_PATH}" "${dry_run}"
			echo
		fi

		step "Registering application with LaunchServices..."
		register_app_launchservices "${lsregister_bin}" "${APP_RESOLVED_PATH}" "${dry_run}"
		echo

		header "--- Setting Handlers (${#UTIS[@]} UTIs & ${#EXTENSIONS[@]} Extensions) ---"
		apply_batch_handlers_macos "${dry_run}"

		if [[ ${restart_services} -eq 1 ]]; then
			echo
			restart_desktop_services_macos "${dry_run}"
		fi

		echo
		audit_status_macos
		ok "SparkNotePad default editor configuration complete."

	elif [[ ${PLATFORM} == "linux" ]]; then
		apply_linux "${dry_run}" "${auto_confirm}"
	else
		local plat_name
		plat_name=$(uname -s)
		err "Unsupported platform: ${plat_name}. This script supports macOS and Linux."
		exit 1
	fi
}

main "$@"
