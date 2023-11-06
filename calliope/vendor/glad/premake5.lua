project "Glad"
  kind "StaticLib"
  language "C"
  
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
    staticruntime "On"
  
  filter {"system:windows", "configurations:Debug"}
    buildoptions "/MDd"
  
  filter {"system:windows", "configurations:Test"}
    buildoptions "/MD"
    
  filter {"system:windows", "configurations:Release"}
    buildoptions "/MD"