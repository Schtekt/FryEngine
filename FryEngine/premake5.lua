project "FryEngine"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "../Build/bin/%{cfg.buildcfg}-%{cfg.architecture}"
   objdir "../Build/bin-int/%{cfg.buildcfg}-%{cfg.architecture}"

   files { "src/**.h", "src/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"