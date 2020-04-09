-- x64 is experimental and not guaranteed to work. Pending on gmod actually getting 64 bit support. 
-- win32 suffix doesn't change currently as we don't know what the plan will be for this. Similar to how 
-- linux / osx still get the .dll extension even though yknow, those are the wrong binary suffixes for those OS's

platformAlias = {
    ["windows"] = "win32",
    ["linux"] = "linux",
    ["macosx"] = "osx"
}

workspace "gfwens"
    configurations {"Debug", "Release"}
    platforms {"x32", "x64"}

project "gmsv_gfwens"
    kind "SharedLib"
    language "C++"

    files { "src/*.cpp", "src/*.h" }

    includedirs { "../gmodheaders/include", "../steamworks" }
    targetname ("gmsv_fwens_" .. platformAlias[os.target()])
    targetprefix ""
    targetextension ".dll"

    -- Debug | x32 / x64
    filter { "configurations:Debug", "platforms:x32" }
        defines { "DEBUG", "_CRT_SECURE_NO_WARNINGS" }
        links { "../steamworks/sdk/redistributable_bin/steam_api.lib"}
        symbols "On"
        targetdir "bin/x32/debug/"
        objdir "bin/x32/debug/"
        architecture "x32"

    filter { "configurations:Debug","platforms:x64" }
        defines { "DEBUG", "_CRT_SECURE_NO_WARNINGS" }
        links { "../steamworks/sdk/redistributable_bin/win64/steam_api64.lib"}
        symbols "On"
        targetdir "bin/x64/debug/"
        objdir "bin/x64/debug/"
        architecture "x64"

    -- Release | x32 / x64
    filter { "configurations:Release","platforms:x32" }
        defines { "NDEBUG", "_CRT_SECURE_NO_WARNINGS" }
        links { "../steamworks/sdk/redistributable_bin/steam_api.lib"}
        optimize "On"
        targetdir "bin/x32/release/"
        objdir "bin/x32/release/"
        symbols "off"
        architecture "x32"

    filter { "configurations:Release", "platforms:x64" }
        defines { "NDEBUG", "_CRT_SECURE_NO_WARNINGS" }
        links { "../steamworks/sdk/redistributable_bin/win64/steam_api64.lib"}
        optimize "On"
        targetdir "bin/x64/release/"
        objdir "bin/x64/release/"
        symbols "off"
        architecture "x64"
 