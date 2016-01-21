parenttemplate = {
    friendlyname = "Some general template",

    parameters = { -- The properties that need to be set when instanciating the template
        ["x"] = "Position.x", -- What property to map it to (of the form "Component.property[.subproperty...]")
        ["y"] = "Position.y",
        ["width"] = "Position.width",
        ["height"] = "Position.height"
    },

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
            width = 0,
            height = 0
        },
        ["Render"] = {
            picturefilename = "image.png",
            zorder = 0
        }
    }
}
