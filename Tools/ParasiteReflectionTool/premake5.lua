project "ParasiteReflectionTool"
	location "."
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	-- Built to the same bin/<outputdir> layout as everything else, keyed off
	-- the workspace location so it can be found from any project's prebuild
	-- step regardless of that project's own `location`.
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.cpp",
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
