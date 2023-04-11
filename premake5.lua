workspace "calliope"
    startproject "calliopeeditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "calliopeeditor"
    location "clp_editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files
    {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
    }

    flags
    {
        "FatalWarnings"
    }