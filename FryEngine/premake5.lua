project "FryEngine"
   kind "ConsoleApp"
   language "C++"
   targetdir "../Build/bin/%{cfg.buildcfg}-%{cfg.architecture}"
   objdir "../Build/bin-int/%{cfg.buildcfg}-%{cfg.architecture}"

   files { "src/**.h", "src/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"