workspace "MMOClient"
    configurations {"Debug", "Release"}
    platforms { "x64" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "MMOClient"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int" .. outputdir .. "/%{prj.name}")

    files {"%{prj.location}/src/**.h", "%{prj.location}/src/**.cpp"}

    includedirs { 
        "%{prj.location}/external/SDL3/include",
        "%{prj.location}/external/glm"
    }
    libdirs { 
        "%{prj.location}/external/SDL3/lib"
    }

    links {"SDL3"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"RELEASE"}
        symbols "On"
