@echo off
REM LockBox Windows installer (batch wrapper)
REM Usage: setup.bat

REM Locate PowerShell and run the main script
powershell -NoProfile -ExecutionPolicy Bypass -Command "& { 
  $ErrorActionPreference='Stop'; 
  $pwd = Split-Path -Parent $MyInvocation.MyCommand.Definition;
  Set-Location $pwd;
  .\scripts\setup_windows.ps1
}"
pause
