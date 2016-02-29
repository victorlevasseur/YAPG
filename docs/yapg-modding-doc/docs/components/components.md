# Components and classes
This section describes the attributes of each components.

**Be sure to read [Native types](native-types.md) before continuing your reading.**

## Class attributes definition
Classes can contain multiple attributes of different types.

For example, for a class name MyClass containing two attributes (a1 of type T and a2 of type U), we can describe the class like this:

### Lua serialization form
```lua
{
    a1 = value_of_a1, --must be the lua serialization form of the type T
    a2 = value_of_a2, --must be the lua serialization form of the type U
}
```

### XML serialization form
```xml
<a1>value_of_a1</a1>
<!-- value_of_a1 must be the XML serialization form of the type T -->
<a2>value_of_a2</a2>
```

**Not yet implemented in the engine**

## Availables components

Currently, the game has the following components:

 - [PositionComponent](#positioncomponent)
 - [RenderComponent](#rendercomponent)

### PositionComponent

This a class containing attributes:

 - x : Number
 - y : Number
 - width : Number
 - height : Number

**Example of definition in a lua template (lua serialization form):**
```lua
components = {
    --The PositionComponent
    ["Position"] = {
        x = 10,
        y = 20,
        width = 64,
        height = 128,
    },
}
```

### RenderComponent

This a class containing attributes:

 - texture : String (path to the texture picture)
 - animations : Map associating String (animation's name) to Animation (*see [the definition of the Animation class](#animation-class)*)
 - **TODO: Write the other parameters**

**Example of definition in a lua template (lua serialization form):**
```lua
components = {
    --The RenderComponent
    ["Render"] = {
        texture = "myTexture.png",
        animations = {
            default_animation = {
                duration = 1,
            },
            alternative_animation = {
                duration = 2,
            },
        },
    },
}
```

#### Animation class

This is a class containing attributes:

 - duration : Number (the total duration of the animation)
 - frames : Array of Frame (*see [the definition of Frame](#frame-class)*)

**Lua serialization form:**
```lua
{
    duration = 1,
    frames = {
        {
            rect = frameRect1, --see the definition of Frame for more information
            relative_duration = 0.6,
        },
        {
            rect = frameRect2,
            relative_duration = 0.4,
        },
    },
}
```
