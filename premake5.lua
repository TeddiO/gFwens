-- linux / osx still get the .dll extension even though yknow, those are the wrong binary suffixes for those OS's

workspace "gmsv_gfwens"
    configurations {"Debug", "Release"}
    platforms {"x32", "x64"}

project "gmsv_gfwens"
    kind "SharedLib"
    language "C++"
    files { "src/*.cpp", "src/*.h" }

    local gmodBaseDir =  os.getenv("GMODHEADERS") or ""
    local steamworksBaseDir = os.getenv("STEAMWORKS_LIB_BASEDIR") or ""
   
    includedirs { "../gmodheaders/include", "../steamworks", gmodBaseDir, steamworksBaseDir}
    targetname ("gmsv_fwens")
    targetprefix ""
    targetextension ".dll"

    filter "configurations:Debug"
        defines { "DEBUG", "_CRT_SECURE_NO_WARNINGS" }
        symbols "On"
        targetdir "bin/debug/"
        objdir "bin/debug/"

    filter "configurations:Release"
        defines { "NDEBUG", "_CRT_SECURE_NO_WARNINGS" }
        optimize "On"
        symbols "off"
        targetdir "bin/release/"
        objdir "bin/release/"

    filter {"platforms:*32" }
        architecture "x86"

    filter {"platforms:*64" }
        architecture "x86_64"

    filter({"system:windows", "platforms:*32"})
        targetsuffix("_win32")
        if steamworksBaseDir ~= "" then 
            links {steamworksBaseDir .. "/sdk/redistributable_bin/steam_api.lib"} 
        else
            links { "../steamworks/sdk/redistributable_bin/steam_api.lib"}
        end

    filter({"system:windows", "platforms:*64"})
        targetsuffix("_win64")
        
        if steamworksBaseDir ~= "" then 
            links {steamworksBaseDir .."/sdk/redistributable_bin/win64/steam_api64.lib"}
        else
            links { "../steamworks/sdk/redistributable_bin/win64/steam_api64.lib"}
        end

    filter({"system:linux", "platforms:*32"})
        targetsuffix("_linux")
        if steamworksBaseDir ~= "" then 
            libdirs {steamworksBaseDir .. "/sdk/redistributable_bin/linux32/"}
        else
            libdirs {"../steamworks/sdk/redistributable_bin/linux32/"}
        end
        links {"steam_api"}

    filter({"system:linux", "platforms:*64"})
        targetsuffix("_linux64")
        
        if steamworksBaseDir ~= "" then 
            libdirs {steamworksBaseDir .. "/sdk/redistributable_bin/linux64/"}
        else
            libdirs {"../steamworks/sdk/redistributable_bin/linux64/"}
        end
        links {"steam_api"}

    filter {}
