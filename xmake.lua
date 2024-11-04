set_project("Brocken")
set_version("0.0.1", {build = "%c" -- Sets format of VERSION_BUILD in config.hh.in
--~ ", soname = true
})

-- set_xmakever("2.5.3") -- Required to stuff GIT_* into config.h
set_xmakever("2.9.6") -- Required to stuff GIT_* into config.h

set_languages("c17", "c++20")

add_rules("mode.release", "mode.debug", "mode.releasedbg", "mode.coverage")

if is_mode("release") then
    set_optimize("fastest")
    set_strip("all")
elseif is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
end

--~ includes("src", "include")

--~ set_config("buildir", "build.xmake")
includes("@builtin/check") -- detect C++20 features: cxx_constexpr, cxx_impl_coroutine

add_cflags("-fPIC", "-pipe")
add_cxxflags("-fPIC", "-pipe")

-- add_cxxflags("-Wno-invalid-offsetof")

set_encodings("utf-8") --~ http://utf8everywhere.org/

-- TODO: make these actually do something
--~ option("tests")
--~     set_description( "Build the test suite")
--~     set_default(false)
--~     set_showmenu(true)
--~     add_defines("TEST")

--~ option("coverage") -- TODO: make this actually do something
--~     set_description("Build the code coverage suite")
--~     set_default(false)
--~     set_showmenu(true)
--~     add_defines("COVER")

--~ option("benchmarks")
--~     set_description( "Build the code benchmarking suite")
--~     set_default(false)
--~     set_showmenu(true)
--~     add_defines("BENCHMARK")

--~ option("mode")
--~     set_description("Set build mode",
--~                     " - debug",
--~                     " - release",
--~                     " - profile")
--~     set_default("debug")
--~     set_showmenu(true)

if has_config("benchmarks") then
    set_configvar("BROCKEN_BENCHMARKS", 1)
end

if has_config("tests") then
    set_configvar("BROCKEN_TESTS", 1)
end

includes("t")

--~ clang has trouble linking with fmt built through Conan
add_requires("fmt", {alias = "fmt"})
--~ add_requires("conan::fmt/10.1.1", {alias = "fmt"})

target("rocken")
    if has_config("static") then
        set_kind("static")
    else
        set_kind("shared")
    end
    set_group("core")
    add_files("src/*.cc")
    add_includedirs("include")
    add_configfiles("include/config.hh.in", {variables = {brocken_mode = "xmake"}, prefixdir = "include"})
    add_includedirs("$(buildir)/include")
    add_headerfiles("include/*.hh", {install = false})
    configvar_check_features("HAS_CONSTEXPR", "cxx_constexpr")
    configvar_check_features("HAS_COROUTINE", "cxx_impl_coroutine", {cxxflags = {"-fPIC", "-fcoroutines", "-fconcepts"}})
    configvar_check_features("HAS_CONSEXPR_AND_STATIC_ASSERT", {"cxx_constexpr", "c_static_assert"}, {cxxflags = {"-fPIC", "-fcoroutines", "-fconcepts"}, languages = "c++20"})

--~     set_basename("rocken_$(mode)_$(arch)")
    add_packages("fmt")

target("brocken")
    set_kind("binary")
    set_group("core")
    add_files("src/brocken.cc")
    add_includedirs("include")
--~     add_configfiles("include/config.hh.in", {variables = {brocken_mode = "xmake"}, prefixdir = "include"})
    add_includedirs("$(buildir)/include")
    add_deps("rocken")
    add_packages("fmt")

option("tests")
    set_default(false)
    set_showmenu(true)
    set_description("Enable tests")

option("benchmarks")
    add_deps("tests")
    set_default(false)
    set_showmenu(true)
    set_description("Enable benchmarks (automatically enables tests)")
    after_check(function (option)
        if not option:dep("tests"):enabled() then
            option:dep("tests"):enable(true)
        end
    end)

task("test")
    on_run(function ()
        import("core.project.project")
        import("core.project.config")
        os.exec("xmake config --tests=y")
        os.exec("xmake")
        local tests = "prove --timer"
--~         local count = 1
        for targetname, target in pairs(project.targets()) do
            if target:is_binary() and target:get("group") == "tests" then
                tests = tests .. " " .. target:targetfile()
--~                 count = math.min(count + 1, 10) -- max 10 tests at a time
            end
        end
        print(tests)
        os.exec(tests)
    end)
    set_menu {
  usage = "xmake test",
  description = "Build and run test suite with prove"
}

task("echo")

    -- Set the run script
    on_run(function ()

        -- Import parameter option module
        import("core.base.option")

        -- Initialize color mode
        local modes = ""
        for _, mode in ipairs({"bright", "dim", "blink", "reverse"}) do
            if option.get(mode) then
                modes = modes .. " " .. mode
            end
        end

        -- Get parameter content and display information
        cprint("${%s%s}%s", option.get("color"), modes, table.concat(option.get("contents") or {}, " "))
    end)

    -- Set the command line options for the plugin. There are no parameter options here, just the plugin description.
    set_menu {
                -- Settings menu usage
                usage = "xmake echo [options]"

                -- Setup menu description
            ,   description = "Echo the given info!"

                -- Set menu options, if there are no options, you can set it to {}
            ,   options =
                {
                    -- Set k mode as key-only bool parameter
                    {'b', "bright", "k", nil, "Enable bright." }
                ,   {'d', "dim", "k", nil, "Enable dim." }
                ,   {'-', "blink", "k", nil, "Enable blink." }
                ,   {'r', "reverse", "k", nil, "Reverse color." }

                    -- When the menu is displayed, a blank line
                ,   {}

                    -- Set kv as the key-value parameter and set the default value: black
                ,   {'c', "color", "kv", "black", "Set the output color."
                                                     , " - red"
                                                     , " - blue"
                                                     , " - yellow"
                                                     , " - green"
                                                     , " - magenta"
                                                     , " - cyan"
                                                     , " - white" }

                    -- Set `vs` as a value multivalued parameter and a `v` single value type
                    -- generally placed last, used to get a list of variable parameters
                ,   {}
                ,   {nil, "contents", "vs", nil, "The info contents." }
                }
            }




--~ 		if has_config("coverage") then
--~ 			...
--~ 		end
--~

--~ target("benchmark")
--~     set_kind("binary")
--~     set_default(false) -- Don't build benchmark tests by default
--~     add_files("t/*.cc")
--~     add_packages("cli11")
--~     add_packages("fmt")
--~     add_deps("foo")

--xmake config --arch=x86_64 --mode=debug --ccache=n --tests=y --toolchain=gcc --yes
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
