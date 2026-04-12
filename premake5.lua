workspace "ParasiteEngine"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["Glad"] = "ParasiteEngine/Vendor/Glad/include"
IncludeDir["ImGui"] = "ParasiteEngine/Vendor/ImGui"
IncludeDir["glm"] = "ParasiteEngine/Vendor/glm"
IncludeDir["stb_image"] = "ParasiteEngine/Vendor/stb_image"

-- Add premake files
include "ParasiteEngine/Vendor/GLFW"
include "ParasiteEngine/Vendor/Glad"
include "ParasiteEngine/Vendor/ImGui"


project "ParasiteEngine"
	location "ParasiteEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

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
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/stb_image/**.cpp",
		"%{prj.name}/Source/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PE_PLATFORM_WINDOWS",
			"PE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
		}

		filter "configurations:Debug"
			defines "PE_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "PE_Release"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "PE_DIST"
			runtime "Release"
			optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

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
		"ParasiteEngine/Vendor",
		"%{IncludeDir.glm}",
	}

	links
	{
		"ParasiteEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PE_PLATFORM_WINDOWS",
		}

		filter "configurations:Debug"
			defines "PE_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "PE_Release"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "PE_DIST"
			runtime "Release"
			optimize "on"