project "yaml-cpp"
	location "ParasiteEngine/Vendor/Premakes/yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	files
	{
		"../../yaml-cpp/src/**.cpp",
		"../../yaml-cpp/include/**.h"
	}

	includedirs
	{
		"../../yaml-cpp/include"
	}

	filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
		defines "PE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PE_Release"
		runtime "Release"
		optimize "on"
