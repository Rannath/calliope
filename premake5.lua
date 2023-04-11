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

project "calliopeeditor"
    location "clp_editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(tdir)
    objdir(odir)

    files
    {
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.cpp",
    }

    flags
    {
        "FatalWarnings"
    }