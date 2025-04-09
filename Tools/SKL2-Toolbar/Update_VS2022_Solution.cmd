@echo off
set PROJECT_ROOT_DIR=%~dp0\..\..\

pushd %PROJECT_ROOT_DIR%
.\Tools\premake5.exe vs2022
popd
