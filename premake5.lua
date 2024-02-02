workspace "ForByte"
    architecture "x64"
    startproject "Sandbox"
    
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
IncludeDir["Glad"] = "ForByte/vendor/Glad/include"
IncludeDir["ImGui"] = "ForByte/vendor/imgui"

include "ForByte/vendor/GLFW"
include "ForByte/vendor/Glad"
include "ForByte/vendor/imgui"

project "ForByte"
    location "ForByte"
    kind "SharedLib"
    language "C++"
    staticruntime "off"
    
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
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

    links 
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
             "FB_PLATFORM_WINDOWS",
             "FB_BUILD_DLL",
             "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

     filter "configurations:Debug"
         defines "FB_DEBUG"
         runtime "Debug"
         symbols "On"

     filter "configurations:Release"
         defines "FB_RELEASE"
         runtime "Release"
         optimize "On"

     filter "configurations:Dist"
         defines "FB_DIST"
         runtime "Release"
         optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    
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
        systemversion "latest"
        
        defines
        {
             "FB_PLATFORM_WINDOWS"
        }

     filter "configurations:Debug"
         defines "FB_DEBUG"
         runtime "Debug"
         symbols "On"

     filter "configurations:Release"
         defines "FB_RELEASE"
         runtime "Release"
         optimize "On"

     filter "configurations:Dist"
         defines "FB_DIST"
         runtime "Release"
         optimize "On"