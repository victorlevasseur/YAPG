do
    local P = {}
    template_tools = P

    P.instanciates = function(id, template, instance_params)
        local result
        --The result is a copy of the template
        result = {}
        result.id = id
        result.components = {}
        result.components = array_tools.deep_copy(template.components)

        --Merge components with instance params
        result.components = array_tools.merge_tables(result.components, instance_params)

        return result
    end

    P.inherits = function(parent, sub)
        --Copy sub-template and parent template
        local sub_copy = array_tools.deep_copy(sub)
        local parent_copy = array_tools.deep_copy(parent)

        --Merge them
        sub_copy.components = array_tools.merge_tables(parent_copy.components, sub_copy.components)

        --Process template parameters : get them from parent, then add the
        --others from the sub-template. Also delete the parameters that the
        --sub-template declares deleted (parameters from parent that are
        --forced by the sub-template)
        sub_copy.parameters = array_tools.merge_tables(parent_copy.parameters, sub_copy.parameters)

        --return the result
        return sub_copy
    end
end
