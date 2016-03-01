# Basic types

This section describes how objects of a specific type are represented when **serialized in a lua template file** or **in an XML level file**. This **does not describe how the objects are represented during runtime** (See [Lua functions reference](lua.md) for that).

## Lua and XML serialization

**Most values can be defined in two ways:**

 - the first one shows how the value should be represented in a template lua file in its **lua serialization form**: the form used to instanciate an entity from a template where classes are represented using a lua table containing keys for attributes.
 - the second one shows how the type is represented in its **XML serialization form** when serialized into a XML level file. This is mainly used to write the parameters values of a particular instance of a template.

**The runtime representation (in lua functions) of objects has nothing to do with these two formats that are just two ways of unserialize an entity/template.** See [Lua functions reference](lua.md) to see how to get/change attributes/objects values during runtime.

## Numbers (float, double, int, unsigned_int)
Attributes of type "float", "double", "int", "unsigned_int" can be any value, with or without a point.

Lua | XML
------------------|----------------
2.56 | 2.56

## string
Attributes of type "string" can be any string, written between `"`.

Lua | XML
------------------|---------------
"my string" | my string (no quotes!)

## bool
Attributes of type "bool" can be `true` or `false`.

Lua | XML
------------------|----------------
true | true

## array
Attributes of type "array of T", with T a type.

### Lua
```lua             
{
    first_item, --items must be respectively in the lua serialization form of T and U
    second_item, -- they are not an lua object of type T as it's just a description
    ...
}
```

### XML
```xml
<value>first_item_value</value>
<!-- first_item_value is the XML serialization form of T -->
<value>second_item_value</value>
...
```

In this form, the table contains real objects of type T (a real lua type).

## map
Attributes of type "Map associating T to U", with T and U types.

### Lua
```lua             
{
    first_key = first_item, --keys and items must be in the lua serialization form of T
    second_key = second_item, -- they are not an lua object of type T or U as it's just a description
    ... = ...
}
```

### XML
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

## function

Function are only defined in template lua files. So, only the lua form exists.

For a function returning type T with parameters of types X, Y, ... it's written this way:
### Lua
```lua
function(parameter_of_type_x, parameter_of_type_y, ...)
    --doing something there
    return object_of_type_t --an object of type T
end
```

## Class
Classes can contain multiple attributes of different types. All the objects described in the [Components section](components/components.md) and [Utility classes section](components/utility-classes.md) are classes.

When serialized in a lua template file, classes are represented with a lua table with keys being the attributes' names (and the associated values being the lua serialized representation of the values).

For example, for a class name MyClass containing two attributes (a1 of type T and a2 of type U), we can describe the class like this:

### Lua
```lua
{
    a1 = value_of_a1, --must be the lua serialization form of the type T
    a2 = value_of_a2, --must be the lua serialization form of the type U
}
```

### XML
```xml
<a1>value_of_a1</a1>
<!-- value_of_a1 must be the XML serialization form of the type T -->
<a2>value_of_a2</a2>
```

**XML serialization form not yet implemented in the engine**
