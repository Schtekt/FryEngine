project "gTest"
    kind "StaticLib"
    targetdir "../Build/bin/%{cfg.buildcfg}-%{cfg.architecture}"
    objdir "../Build/bin-int/%{cfg.buildcfg}-%{cfg.architecture}"
    files { "../googletest/googletest/src/gtest-all.cc" }
    includedirs { "../googletest/googletest/include", "../googletest/googletest" }
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Testing"
    kind "ConsoleApp"
    language "C++"
    targetdir "../Build/bin/%{cfg.buildcfg}-%{cfg.architecture}"
    objdir "../Build/bin-int/%{cfg.buildcfg}-%{cfg.architecture}"
    files {"src/**.cpp", "src/**.h"}
    includedirs{"../FryEngine/src/", "../googletest/googletest/include"}
    cppdialect "C++17"
    links{"FryEngine", "gTest"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"