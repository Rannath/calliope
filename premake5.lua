workspace "calliope"
    startproject "calliopeeditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"

-- External Dependancies
externals = {}
externals["sdl2"] = "external/sdl2"
externals["spdlog"] = "external/spdlog"
externals["glad"] = "external/glad"

--Process Glad before anything else
include "external/glad"

project "calliope"
    location "calliope"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.location}/include/**.hpp",
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.cpp",
    }

    externalincludedirs
    {
        "%{prj.location}/include",
        "%{externals.sdl2}/include",
        "%{externals.spdlog}/include",
        "%{externals.glad}/include"
    }

    flags
    {
        "FatalWarnings"
    }

    defines
    {
        GLFW_INCLUDE_NONE --Ensure glad doesn't include glfw
    }

    filter { "system:windows", "configurations:*" }
        systemversion "latest"

        defines
        {
            "CLP_PLATFORM_WINDOWS"
        }
    
    filter { "system:maxosx", "configurations:*" }
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = 10.15,
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "CLP_PLATFORM_MAC"
        }
    
    filter { "system:linux", "configurations:*" }
        defines
        {
            "CLP_PLATFORM_LINUX"
        }
    
    filter "configurations:Debug"
        defines
        {
            "CLP_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines
        {
            "CLP_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

project "calliopeeditor"
    location "calliopeeditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "calliope"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.cpp",
    }

    externalincludedirs
    {
        "calliope/include"
    }

    flags
    {
        "FatalWarnings"
    }

     filter { "system:windows", "configurations:*" }
        systemversion "latest"

        defines
        {
            "CLP_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }

        links
        {
            "SDL2",
            "glad"
        }
    
    filter { "system:maxosx", "configurations:*" }
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = 10.15,
            ["UseModernBuildSystem"] = "NO"
        }

        defines
        {
            "CLP_PLATFORM_MAC"
        }

        links
        {
            "SDL2.framework",
            "glad"
        }
    
    filter { "system:linux", "configurations:*" }
        defines
        {
            "CLP_PLATFORM_LINUX"
        }

        links
        {
            "SDL2",
            "glad"
        }
    
    filter "configurations:Debug"
        defines
        {
            "CLP_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines
        {
            "CLP_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"