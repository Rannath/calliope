workspace "calliope"  architecture "x64"    configurations  {    "Debug",    "Test",    "Release"  }    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"IncludeDir = {}IncludeDir["glad"] = "calliope/vendor/glad/include"include "calliope/vendor/glad"project "calliope"  location "calliope"  kind "SharedLib"  language "C++"    targetdir ("bin/" .. outputdir .. "/%{prj.name}")  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")    pchheader "pch.hpp"  pchsource "calliope/src/pch.cpp"    files  {    "%{prj.name}/src/**.hpp",    "%{prj.name}/src/**.cpp"  }    includedirs  {    "%{prj.name}/src",    "%{IncludeDir.glad}"  }    links  {    "Glad",    "opengl32.lib"  }    postbuildcommands  {    {"{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/sandbox" }  }    filter "system:windows"    cppdialect "C++20"    staticruntime "On"    systemversion "latest"        defines    {      "CLP_PLATFORM_WINDOWS",      "CLP_BUILD_DLL"    }      filter "configurations:Debug"    defines {"CLP_DEBUG", "CLP_ENABLE_ASSERTS"}    buildoptions "/MDd"    symbols "On"    filter "configurations:Test"    defines {"CLP_TEST", "CLP_ENABLE_ASSERTS"}    buildoptions "/MD"    optimize "On"    filter "configurations:Release"    defines "CLP_RELEASE"    buildoptions "/MD"    optimize "On"project "sandbox"  location "sandbox"  kind "ConsoleApp"  language "C++"    targetdir ("bin/" .. outputdir .. "/%{prj.name}")  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")    files  {    "%{prj.name}/src/**.hpp",    "%{prj.name}/src/**.cpp"  }    includedirs  {    "calliope/src"  }    links  {    "calliope"  }    filter "system:windows"    cppdialect "C++20"    staticruntime "On"    systemversion "latest"        defines    {      "CLP_PLATFORM_WINDOWS"    }      filter "configurations:Debug"    defines {"CLP_DEBUG", "CLP_ENABLE_ASSERTS"}    buildoptions "/MDd"    symbols "On"    filter "configurations:Test"    defines {"CLP_TEST", "CLP_ENABLE_ASSERTS"}    buildoptions "/MD"    optimize "On"    filter "configurations:Release"    defines "CLP_RELEASE"    buildoptions "/MD"    optimize "On"