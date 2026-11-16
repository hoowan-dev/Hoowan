-- Box2D is an upstream submodule without a premake script, so it's defined here
-- instead of inside the submodule. Paths are relative to this file.
local box2dDir = "../../Hoowan/vendor/Box2D"

project "Box2D"
	location (box2dDir)
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/Hoowan/vendor/Box2D/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Hoowan/vendor/Box2D/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		box2dDir .. "/src/**.h",
		box2dDir .. "/src/**.cpp",
		box2dDir .. "/include/**.h"
	}

	includedirs
	{
		box2dDir .. "/include",
		box2dDir .. "/src"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
