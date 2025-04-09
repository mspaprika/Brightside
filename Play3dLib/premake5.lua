-------------------------------------------------------------
-- Name: Play3dLib
-- Description: Play3d Library
-------------------------------------------------------------
project "Play3dLib"
	kind "StaticLib"
	includedirs {
		"./",
	}	
	
	-- Local Source Files
	files{
		'**.h',
		'**.cpp',
	}

	libdirs { }

	links { }

	filter{}
		
	debugdir "."
