@echo off
set PROJECT_ROOT_DIR=%~dp0\..\..\
cd /D %PROJECT_ROOT_DIR%
setlocal

"C:\Program Files\RenderDoc\qrenderdoc.exe" Tools/RenderDoc_Debug.cap

endlocal