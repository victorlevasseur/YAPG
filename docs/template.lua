full_path_to_template = template_tools.inherits(parenttemplate, {
    friendlyname = "Friendly name of template",

    parameters = { -- The properties that need to be set when instanciating the template
        ["width"] = "",
        ["height"] = ""
    },

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
            width = 100,
            height = 100
        },
    }
})
