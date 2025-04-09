-------------------------------------------------------------
-- Name: PBE Workspace
-------------------------------------------------------------
-------------------------------------------------------------
workspace "Brightside"
 	platforms {"x64"} 
 	configurations { "Debug", "Debug_ASan", "Release"}
		startproject "SKL-2-Starter"

 	filter "platforms:*x64*"
 		system "windows"
 		architecture ("x86_64")
 		defines {"_WIN32", "WIN32_LEAN_AND_MEAN", "NOMINMAX", "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"}
		links {}
		
	-- General Debug Configuration.
	filter "configurations:*Debug*"
		defines {"_DEBUG"}    
		symbols "On"
		optimize "Off"
		editandcontinue "Off"
	
		-- General Release Configuration
		filter "configurations:*Release*"
		defines { "NDEBUG" }
		symbols "On"
		optimize "Speed"
		editandcontinue "Off"
		
		filter "configurations:*ASan*"
			sanitize { "Address" }
			editandcontinue "Off"

   filter "system:x64"
      systemversion "latest" -- To use the latest version of the SDK available

	filter {}
		language "C++"
	    cppdialect "C++20"
	    warnings "Extra"
	    flags { "FatalCompileWarnings","MultiProcessorCompile"}
	    --characterset "MBCS"
	    characterset "Unicode"

		location("Build/")

		includedirs { }
	
group("Libraries")
include ("Play3dLib/premake5.lua")
	
group("Project")
include "Project/premake5.lua"