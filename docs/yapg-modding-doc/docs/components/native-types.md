# Native types

Natives types are not used directly by the engine. But they are often used as attributes of components.

## Lua and XML serialization

**Most types can be defined in two ways:**

 - the first one shows how the type should be represented in a template lua file in its **lua serialization form**: the form used to instanciate an entity from a template where classes are represented using a lua table containing keys for attributes. In fact, this is the way classes/values are written in attributes of a template lua file.
 - the second one shows how the type is represented in its **XML serialization form** when serialized into a level XML file.

To sum up, use the **lua serialization form** to describe objects in template lua files. Use the **XML serialization form** to describe objects in level XML files (mainly used to define **parameters**).

## Number
Attributes of type "Number" can be any value, with or without a point.

Lua serialization | XML runtime
------------------|----------------
2.56 | 2.56 | 2.56

## String
Attributes of type "String" can be any string, written between `"`.

Lua serialization | XML runtime
------------------|----------------
"my string" | my string (no quotes!)

## Boolean
Attributes of type "Boolean" can be `true` or `false`.

Lua serialization | XML runtime
------------------|----------------
true | true | true

## Array of "T"
Attributes of type "Array of T", with T a type.

### Lua serialization
```lua             
{
    first_item, --items must be respectively in the lua serialization form of T and U
    second_item, -- they are not an lua object of type T as it's just a description
    ...
}
```

### XML serialization
```xml
<value>first_item_value</value>
<!-- first_item_value is the XML serialization form of T -->
<value>second_item_value</value>
...
```

In this form, the table contains real objects of type T (a real lua type).

## Map associating "T" to "U"
Attributes of type "Map associating T to U", with T and U types.

### Lua serialization
```lua             
{
    first_key = first_item, --keys and items must be in the lua serialization form of T
    second_key = second_item, -- they are not an lua object of type T or U as it's just a description
    ... = ...
}
```

### XML serialization
```xml
<pair>
    <key>first_key_value</key>
    <!-- first_item_key is the XML serialization form of T -->
    <value>first_item_value</value>
    <!-- first_item_value is the XML serialization form of U -->
</pair>
<pair>
    <key>second_key_value</key>
    <value>second_item_value</value>
</pair>
...
```

In this form, the table contains real objects of type T and U (a real lua type).

## Function returning T with parameters X, Y...

Function are only defined in template lua files. So, only the lua serialization form exists.

### Lua serialization form
```lua
function(parameter_of_type_x, parameter_of_type_y, ...)
    --doing something there
    return object_of_type_t --an object of type T
end
```
