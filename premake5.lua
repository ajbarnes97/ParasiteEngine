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

-- Include dir relative to solution dir
IncludeDir = {}

IncludeDir["GLFW"] = "ParasiteEngine/Vendor/GLFW/include"
IncludeDir["Glad"] = "ParasiteEngine/Vendor/Glad/include"
IncludeDir["ImGui"] = "ParasiteEngine/Vendor/ImGui"
IncludeDir["glm"] = "ParasiteEngine/Vendor/glm"
IncludeDir["stb_image"] = "ParasiteEngine/Vendor/stb_image"
IncludeDir["entt"] = "ParasiteEngine/Vendor/entt"
IncludeDir["yaml_cpp"] = "ParasiteEngine/Vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "ParasiteEngine/Vendor/ImGuizmo/src"
IncludeDir["Reflection"] = "Tools/ParasiteReflectionTool/Source"

group "Dependencies"
	include "ParasiteEngine/Vendor/GLFW"
	include "ParasiteEngine/Vendor/Glad"
	include "ParasiteEngine/Vendor/ImGui"
	include "ParasiteEngine/Vendor/Premakes/yaml-cpp"
group ""

group "Tools"
	include "Tools/ParasiteReflectionTool"
group ""


----------------------------------------------------
-- Reflection
----------------------------------------------------
local root = os.getcwd()
local ReflectionToolExe = "%{wks.location}/bin/" .. outputdir .. "/ParasiteReflectionTool/ParasiteReflectionTool.exe"

function GenerateReflection()
	dependson { "ParasiteReflectionTool" }
	prebuildcommands
	{
		'"' .. ReflectionToolExe .. '" --scan "%{wks.location}/ParasiteEngine/Source" "%{wks.location}/ParasiteEditor/Source" --output "%{wks.location}/Generated"'
	}
end

newaction
{
	trigger = "reflection",
	description = "Generate C++ reflection headers",

	execute = function()
		local toolConfig = "Debug-windows-x64"
		local generator = path.join(root, "bin", toolConfig, "ParasiteReflectionTool", "ParasiteReflectionTool.exe")

		if not os.isfile(generator) then
			print("ParasiteReflectionTool executable not found at " .. generator)
			print("Build the ParasiteReflectionTool project first (matching toolConfig above), then re-run this action.")
			return
		end

		local projects =
		{
			"ParasiteEngine",
			"ParasiteEditor",
			"Sandbox"
		}

		for _, projectName in ipairs(projects) do

			local source = path.join(root, projectName, "Source")
			local output = path.join(root, "Generated", projectName)

			print("Generating reflection for " .. projectName)

			os.execute(
				'"' ..
				generator ..
				'" --scan "' ..
				source ..
				'" --output "' ..
				output ..
				'"'
			)

		end
	end
}


----------------------------------------------------
-- Engine
----------------------------------------------------
project "ParasiteEngine"

	location "ParasiteEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	buildoptions
	{
		"/utf-8"
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	GenerateReflection()
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
		"%{wks.location}/Generated",
		"%{IncludeDir.Reflection}",
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
		flags
		{
			"NoPCH"
		}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PE_PLATFORM_WINDOWS",
			"PE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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


----------------------------------------------------
-- Editor
----------------------------------------------------
project "ParasiteEditor"
	location "ParasiteEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	debugdir "%{prj.location}"

	buildoptions
	{
		"/utf-8"
	}

	GenerateReflection()

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
		"%{wks.location}/Generated",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Reflection}",
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
			"PE_PLATFORM_WINDOWS"
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


----------------------------------------------------
-- Sandbox
----------------------------------------------------
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	debugdir "%{prj.location}"

	buildoptions
	{
		"/utf-8"
	}

	GenerateReflection()

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	includedirs
	{
		"ParasiteEngine/Vendor/spdlog/include",
		"%{IncludeDir.Reflection}",
		"ParasiteEngine/Source",
		"ParasiteEngine/Vendor",
		"%{wks.location}/Generated",
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
			"PE_PLATFORM_WINDOWS"
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