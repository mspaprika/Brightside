-------------------------------------------------------------
-- Name: Play3dAnimation
-- Description: Play3d Animation Tutorial Application
-------------------------------------------------------------
project "SKL-2-Starter"
	kind "WindowedApp"
	includedirs {
		"./",
		"./Data/Shaders/",
	}	
	
	-- Local Source Files
	files{
		'**.h',
		'**.cpp',
		'**.hlsl',
		'**.hlsli',
	}

	libdirs { }

	links { "Play3dLib" }

	filter{"files:**.hlsl"}
		buildaction "None"

	filter{}
	
	vpaths {
	["Shaders"] = {"Data/Shaders/*.hlsl", "Data/Shaders/*.hlsli"},
	}

	debugdir "."