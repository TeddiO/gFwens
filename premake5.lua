
platformAlias = {
    ["windows"] = "win32",
    ["linux"] = "linux",
    ["macosx"] = "osx"
}

workspace "gfwens"
    configurations {"Debug", "Release"}

project "gmsv_gfwens"
    kind "SharedLib"
    language "C++"

    files { "src/*.cpp", "src/*.h" }

    includedirs { "../gmodheaders/include", "../steamworks" }
    links { "../steamworks/sdk/redistributable_bin/steam_api.lib"}
    targetname ("gmsv_gfwens_" .. platformAlias[os.target()])
    targetprefix ""
    targetextension ".dll"

    filter { "configurations:Debug" }
        defines { "DEBUG", "_CRT_SECURE_NO_WARNINGS" }
        symbols "On"
        targetdir "bin/debug/"
        objdir "bin/debug/"


    filter { "configurations:Release" }
        defines { "NDEBUG", "_CRT_SECURE_NO_WARNINGS" }
        optimize "On"
        targetdir "bin/release/"
        objdir "bin/release/"
        symbols "off"
        
