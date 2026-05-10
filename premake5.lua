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
IncludeDir["entt"] = "ParasiteEngine/Vendor/entt"
IncludeDir["yaml_cpp"] = "ParasiteEngine/Vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "ParasiteEngine/Vendor/ImGuizmo/src"

-- Add premake files
group "Dependencies"
	include "ParasiteEngine/Vendor/GLFW"
	include "ParasiteEngine/Vendor/Glad"
	include "ParasiteEngine/Vendor/ImGui"
	include "ParasiteEngine/Vendor/yaml-cpp"
group ""

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
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/stb_image/**.cpp",
		"%{prj.name}/Vendor/ImGuizmo/src/**.h",
		"%{prj.name}/Vendor/ImGuizmo/src/**.cpp",
	}

	defines
	{
	    "_CRT_SECURE_NO_WARNINGS",
	    "YAML_CPP_STATIC_DEFINE"
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp"
	}

	filter "files:**/ImGuizmo/src/**.cpp"
		flags { "NoPCH" }

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


project "ParasiteEditor"
	location "ParasiteEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	debugdir "%{prj.location}" -- Temp

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
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
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


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	debugdir "%{prj.location}" -- Temp

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
		"%{IncludeDir.entt}",
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

