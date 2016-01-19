do
    local P = {}
    array_tools = P

    P.table_size = function(table)
        local count = 0
        for k, v in pairs(table) do
            count = count + 1
        end

        return count
    end

    P.get_keys = function(table)
        local keys = ""
        for k, v in pairs(table) do
            keys = keys..k.."|"
        end

        return keys
    end

    P.array_contains = function(table, key)
        for k, v in pairs(table) do
            if k == key then
                return true
            end
        end
        return false
    end

    P.deep_copy = function(orig)
        local orig_type = type(orig)
        local copy
        if orig_type == 'table' then
            copy = {}
            for orig_key, orig_value in next, orig, nil do
                copy[P.deep_copy(orig_key)] = P.deep_copy(orig_value)
            end
            setmetatable(copy, P.deep_copy(getmetatable(orig)))
        else -- number, string, boolean, etc
            copy = orig
        end
        return copy
    end

    P.merge_tables = function(source, merge_with)
        for k,v in pairs(merge_with) do
        	if type(v) == "table" then
        		if type(source[k] or false) == "table" then
        			P.merge_tables(source[k] or {}, merge_with[k] or {})
        		else
        			source[k] = v
        		end
        	else
        		source[k] = v
        	end
        end
        return source
    end
end
