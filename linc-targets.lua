local function define_module(name, srcs, deps)
    target("linc_" .. name)
        set_kind("static")
        set_default(false)
        set_languages("gnu23")
        add_defines("_GNU_SOURCE")
        add_includedirs("include", { public=true })

        for _, src in ipairs(srcs or {}) do
            add_files("src/" .. src)
        end
        for _, dep in ipairs(deps or {}) do
            add_deps("linc_" .. dep)
        end
end

define_module("core", { "alloc.c", "fatal.c" })

-- containers
define_module("array", { "array.c" }, { "core" })
define_module("string", { "string.c" }, { "core" })
define_module("hda", { "hda.c" }, { "core" })
define_module("hds", { "hds.c" }, { "core", "hda" })
define_module("trie", { "trie.c" }, { "core" })

-- utilities
define_module("log", { "log.c" }, { "core", "string" })
define_module("args", { "args.c" }, { "core" })
define_module("xsys", { "xsys.c" }, { "core" })
