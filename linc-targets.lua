linc_targets = {}

local function define_target(name, srcs, deps)
    local target_name = "linc_" .. name
    table.insert(linc_targets, target_name)

    target(target_name)
        if srcs == nil then
            set_kind("headeronly")
        else
            set_kind("static")
            for _, src in ipairs(srcs or {}) do
                add_files("src/" .. src)
            end
        end

        set_default(false)
        set_languages("gnu23")
        add_defines("_GNU_SOURCE")
        add_includedirs("include", { public = true })

        for _, dep in ipairs(deps or {}) do
            add_deps("linc_" .. dep)
        end
end

define_target("core", { "linc.c", "xsys.c" })

-- containers
define_target("array", nil, { "core" })
define_target("string", { "string.c" }, { "core" })
define_target("hda", { "hda.c" }, { "core" })
define_target("hds", { "hds.c" }, { "core", "hda" })
define_target("trie", { "trie.c" }, { "core" })

-- utilities
define_target("log", { "log.c" }, { "core", "string" })
define_target("args", { "args.c" }, { "core" })
