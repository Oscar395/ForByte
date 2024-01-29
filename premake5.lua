workspace "ForByte"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "ForByte/vendor/GLFW/include"

include "ForByte/vendor/GLFW"

project "ForByte"
    location "ForByte"
    kind "SharedLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "fbpch.h"
    pchsource "ForByte/src/fbpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}"
    }

    links 
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
             "FB_PLATFORM_WINDOWS",
             "FB_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

     filter "configurations:Debug"
         defines "FB_DEBUG"
         symbols "On"

     filter "configurations:Release"
         defines "FB_RELEASE"
         optimize "On"

     filter "configurations:Dist"
         defines "FB_DIST"
         optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "ForByte/vendor/spdlog/include",
        "ForByte/src"
    }

    links
    {
        "ForByte"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
             "FB_PLATFORM_WINDOWS"
        }

     filter "configurations:Debug"
         defines "FB_DEBUG"
         symbols "On"

     filter "configurations:Release"
         defines "FB_RELEASE"
         optimize "On"

     filter "configurations:Dist"
         defines "FB_DIST"
         optimize "On"