#!/usr/bin/env bash
set -euo pipefail

# LockBox setup script (Linux)
# Usage:
#   ./setup.sh        -> interactive
#   ./setup.sh --yes  -> non-interactive (auto-install deps, build, install)

REPO_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$REPO_DIR/build"
INSTALL_PREFIX="/usr/local"
AUTO_YES=0

# Required packages for Fedora
FEDORA_PKGS=(cmake gcc-c++ make libsodium-devel libzip-devel ImageMagick nlohmann-json-devel)

print() { echo -e "$@"; }

usage() {
  cat <<EOF
LockBox setup script

Usage:
  ./setup.sh [--yes] [--no-install] [--prefix <path>]

Options:
  --yes         : non-interactive, install missing dependencies automatically (uses sudo)
  --no-install  : skip automatic package install, just build
  --prefix PATH : installation prefix (default: /usr/local)

EOF
  exit 1
}

# parse args
while [[ $# -gt 0 ]]; do
  case "$1" in
    --yes) AUTO_YES=1; shift ;;
    --no-install) NO_INSTALL=1; shift ;;
    --prefix) INSTALL_PREFIX="$2"; shift 2 ;;
    -h|--help) usage ;;
    *) echo "Unknown arg: $1"; usage ;;
  esac
done

# detect distro (very simple)
detect_distro() {
  if command -v dnf >/dev/null 2>&1; then
    echo "fedora"
  elif command -v apt-get >/dev/null 2>&1; then
    echo "debian"
  else
    echo "unknown"
  fi
}

ensure_packages_fedora() {
  missing=()
  for pkg in "${FEDORA_PKGS[@]}"; do
    if ! rpm -q --quiet "$pkg"; then
      missing+=("$pkg")
    fi
  done

  if [ ${#missing[@]} -eq 0 ]; then
    print "All Fedora packages present."
    return 0
  fi

  print "Missing packages: ${missing[*]}"
  if [ "$AUTO_YES" -eq 1 ]; then
    sudo dnf install -y "${missing[@]}"
  else
    read -rp "Install missing packages via sudo dnf? [y/N] " ans
    if [[ "$ans" =~ ^[Yy]$ ]]; then
      sudo dnf install -y "${missing[@]}"
    else
      print "Skipping package install. You must install: ${missing[*]}"
      return 1
    fi
  fi
}

# check system prerequisites
print "LockBox installer"
distro="$(detect_distro)"
print "Detected distro: $distro"

if [ "${NO_INSTALL:-0}" = "1" ]; then
  print "Skipping automatic dependency installation (user requested --no-install)."
else
  if [ "$distro" = "fedora" ]; then
    ensure_packages_fedora
  elif [ "$distro" = "debian" ]; then
    print "Debian/Ubuntu detected. Installing dependencies with apt (requires sudo)."
    # basic Debian package names (may differ)
    DEBIAN_PKGS=(cmake g++ make libsodium-dev libzip-dev imagemagick nlohmann-json3-dev)
    missing=()
    for pkg in "${DEBIAN_PKGS[@]}"; do
      if ! dpkg -s "$pkg" >/dev/null 2>&1; then
        missing+=("$pkg")
      fi
    done
    if [ ${#missing[@]} -ne 0 ]; then
      if [ "$AUTO_YES" -eq 1 ]; then
        sudo apt update && sudo apt install -y "${missing[@]}"
      else
        read -rp "Install missing packages via sudo apt? [y/N] " ans
        if [[ "$ans" =~ ^[Yy]$ ]]; then
          sudo apt update && sudo apt install -y "${missing[@]}"
        else
          print "Skipping package install. You must install: ${missing[*]}"
        fi
      fi
    fi
  else
    print "Unsupported/unknown distro. Please install these packages manually:"
    print "  - cmake, g++, make, libsodium (dev), libzip (dev), ImageMagick, nlohmann-json (dev)"
  fi
fi

# Build
print "\nBuilding LockBox in: $BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
print "Running cmake..."
cmake .. -DCMAKE_BUILD_TYPE=Release
print "Running build..."
cmake --build . --parallel

# Optional install
if [ -n "${INSTALL_PREFIX:-}" ]; then
  read -rp "Install lockbox to ${INSTALL_PREFIX}/bin ? [y/N] " inst_ans
  if [ "$inst_ans" = "y" ] || [ "$AUTO_YES" -eq 1 ]; then
    if [ "$AUTO_YES" -eq 1 ]; then
      sudo cmake --install . --prefix "$INSTALL_PREFIX"
    else
      sudo cmake --install . --prefix "$INSTALL_PREFIX"
    fi
    print "Installed to ${INSTALL_PREFIX}/bin/lockbox"
  else
    print "Skipping system install. You can run the binary from $BUILD_DIR"
  fi
fi

print "Done. Run: $BUILD_DIR/lockbox (or lockbox if installed)."
