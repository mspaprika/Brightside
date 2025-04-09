@echo off
setlocal
set rootpath=%~dp0..\..\
pushd %rootpath%

set AssetTool=Tools\Play3dAssetTool.exe



%AssetTool% gltf SourceAssets/Powers/powerup_shield.glb --scale 0.1