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
IncludeDir["glm"] = "ForByte/vendor/glm"
IncludeDir["stb_image"] = "ForByte/vendor/stb_image"

include "ForByte/vendor/GLFW"
include "ForByte/vendor/Glad"
include "ForByte/vendor/imgui"

project "ForByte"
    location "ForByte"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "fbpch.h"
    pchsource "ForByte/src/fbpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links 
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines
        {
             "FB_PLATFORM_WINDOWS",
             "FB_BUILD_DLL",
             "GLFW_INCLUDE_NONE"
        }

     filter "configurations:Debug"
         defines "FB_DEBUG"
         runtime "Debug"
         symbols "on"

     filter "configurations:Release"
         defines "FB_RELEASE"
         runtime "Release"
         optimize "on"

     filter "configurations:Dist"
         defines "FB_DIST"
         runtime "Release"
         optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
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
        "ForByte/src",
        "ForByte/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "ForByte"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines
        {
             "FB_PLATFORM_WINDOWS",
             "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
        }

     filter "configurations:Debug"
         defines "FB_DEBUG"
         runtime "Debug"
         symbols "on"

     filter "configurations:Release"
         defines "FB_RELEASE"
         runtime "Release"
         optimize "on"

     filter "configurations:Dist"
         defines "FB_DIST"
         runtime "Release"
         optimize "on"