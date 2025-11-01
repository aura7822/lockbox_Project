# scripts/setup_windows.ps1
# PowerShell installer for LockBox on Windows
param()

function Write-Ok($msg){ Write-Host "[OK] $msg" -ForegroundColor Green }
function Write-Err($msg){ Write-Host "[ERR] $msg" -ForegroundColor Red }
function Ask-YesNo($msg){
  $r = Read-Host "$msg (y/N)"
  return $r -match '^[Yy]'
}

# 1) Check CMake
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
  Write-Err "CMake not found. Install via https://cmake.org/download/ or use Chocolatey: choco install cmake"
  if (Ask-YesNo "Open Chocolatey install instructions?") {
    Start-Process "https://chocolatey.org/install" 
  }
  exit 1
} else { Write-Ok "Found cmake" }

# 2) Check Visual Studio Build Tools or mingw
if (Get-Command cl.exe -ErrorAction SilentlyContinue) {
  Write-Ok "Found MSVC (cl.exe)"
} elseif (Get-Command g++.exe -ErrorAction SilentlyContinue) {
  Write-Ok "Found g++"
} else {
  Write-Err "No compiler found. Install Visual Studio Build Tools or MinGW."
  exit 1
}

# 3) Check libsodium (user must install)
if (-not (Get-Command where.exe libsodium.dll -ErrorAction SilentlyContinue)) {
  Write-Host "libsodium not found in PATH. On Windows, install libsodium and ensure the DLL is in PATH." -ForegroundColor Yellow
  Write-Host "You can install via vcpkg or download prebuilt libs." -ForegroundColor Yellow
}

# Build with CMake
$buildDir = Join-Path (Get-Location) "build"
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }
Set-Location $buildDir

# Configure generator automatically (let CMake choose)
Write-Host "Running cmake..."
cmake .. 
Write-Host "Building..."
cmake --build . --config Release

Write-Ok "Build finished. Binary is in $buildDir"
Write-Host "Tip: Add $buildDir to your PATH or copy the binary to a folder on PATH."
