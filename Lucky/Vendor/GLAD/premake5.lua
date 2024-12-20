project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }
    
	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"	--Debug
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"	--Release
		runtime "Release"
        optimize "on"