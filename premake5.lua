workspace "ParasiteEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include dir relative to solution dir (root dir)
IncludeDir = {}
IncludeDir["GLFW"] = "ParasiteEngine/Vendor/GLFW/include"

-- Add GLFW premake file
include "ParasiteEngine/Vendor/GLFW"

project "ParasiteEngine"
	location "ParasiteEngine"
	kind "SharedLib"
	language "C++"

	buildoptions 
	{ 
		"/utf-8",
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pepch.h"
	pchsource "ParasiteEngine/Source/pepch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
	}

	links
	{
		"GLFW",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		staticruntime "Off"

		defines
		{
			"PE_PLATFORM_WINDOWS",
			"PE_BUILD_DLL",
		}

		postbuildcommands
		{
			("{MKDIR} ../bin/" .. outputdir .. "/Sandbox/"),
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
		}

		filter "configurations:Debug"
			defines "PE_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "PE_Release"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "PE_DIST"
			buildoptions "/MD"
			optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	buildoptions 
	{ 
		"/utf-8",
	}

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	includedirs
	{
		"ParasiteEngine/Vendor/spdlog/include",
		"ParasiteEngine/Source",
	}

	links
	{
		"ParasiteEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PE_PLATFORM_WINDOWS",
		}

		filter "configurations:Debug"
			defines "PE_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "PE_Release"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "PE_DIST"
			buildoptions "/MD"
			optimize "On"