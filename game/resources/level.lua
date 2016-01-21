level = {
    objects = {
        template_tools.instanciates(1, full_path_to_template, {
            ["Position"] = {
                x = 120,
                y = 80
            }
        }),
        template_tools.instanciates(1, full_path_to_template, {
            ["Position"] = {
                x = 400,
                y = 80
            }
        }),
        template_tools.instanciates(1, full_path_to_template, {
            ["Position"] = {
                x = 140,
                y = 100,
            },
            ["Render"] = {
                z = 10
            }
        }),
    }
}
