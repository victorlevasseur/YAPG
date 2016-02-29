# Components and classes
This section describes the attributes of each components.

**Be sure to read [Basic types](native-types.md) before continuing your reading.**

## Vocabulary

In the attribute's tables below, the "U" column tells if the attributes can be unserialized from lua/XML. The "G/S" column tells if the attributes can be received or changed at runtime **with `entity:get_attribute("aComp", "attrName")` and `entity:set_attribute("aComp", "attrName", newVal)` methods.** See [Lua functions reference](../lua.md) to see how to get/change attributes' values of components during runtime.

"✓" means "yes", "…" means that it will be included in a future version (planned) and nothing means "no".

## Availables components

Currently, the game has the following components:

 - [PositionComponent](#positioncomponent)
 - [RenderComponent](#rendercomponent)

### PositionComponent

#### Attributes

Name | Type | Description | [U](#vocabulary) | [G/S](#vocabulary)
-----|------|-------------|---|---
x | float | | ✓ | ✓
y | float | | ✓ | ✓
width | float | | ✓ | ✓
height | float | | ✓ | ✓

#### Lua serialization

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

#### Lua script example
This example moves the entity by 100px towards the right direction.
```lua
--myEntity is an particular entity instance
local old_x_pos = as_float(myEntity:get_attribute("Position", "x"))
local new_x_pos = old_x_pos + 100
myEntity:set_attribute("Position", "x", float_value(new_x_pos))
```

### RenderComponent

#### Attributes

Name | Type | Description | [U](#vocabulary) | [G/S](#vocabulary)
-----|------|-------------|---|---
texture | string | The filepath to the texture picture. | ✓ | ✓
animations | map associating String to [Animation](utility-classes.md#animation-class) | Associates the animation name with the animation. | ✓ | …
current_animation | string | The current animation of the entity. | ✓ | ✓

#### Lua serialization

```lua
components = {
    --The RenderComponent
    ["Render"] = {
        texture = "myTexture.png",
        animations = {
            default_animation = {
                duration = 1, --See the doc of the Animation class to see how Animations are serialized.
            },
            alternative_animation = {
                duration = 2,
            },
        },
    },
}
```
