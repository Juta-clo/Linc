set_toolchains("clang")
set_toolset("cc", "clang")
set_toolset("ld", "clang")
set_policy("build.ccache", false)

add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })
add_cflags("-g", "-O0", "-std=gnu23", "-Wall", "-Wextra")

target("test_all")
    set_kind("binary")
    set_targetdir(".")
    add_defines("_GNU_SOURCE", "UNITY_FIXTURE_NO_EXTRAS", "UNITY_OUTPUT_COLOR")
    add_includedirs("test/unity/", "include", "src")
    add_files("test/*.c", "test/unity/*.c", "src/*.c")
