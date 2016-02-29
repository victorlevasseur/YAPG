# Utility classes

Utility classes are classes used by components to represent some specific data structures.

## Vocabulary

In the attribute's tables below, the "U" column tells if the attributes can be unserialized from lua/XML. The "R" column tells if the attributes can be changed at runtime with lua scripting. "✓" means "yes", "…" means that it will be included in a future version (planned) and nothing means "no".

## Animation class

#### Attributes

 Name | Type | Description | [U](#vocabulary) | [R](#vocabulary)
 -----|------|-------------|---|---
duration | Number | The total duration of the animation, in seconds. | ✓ | …
frames | Array of [Frame](#frame-class) | The frames of the animation. | ✓ | …

#### Lua serialization example
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

## Frame class

#### Attributes

Name | Type | Description | [U](#vocabulary) | [R](#vocabulary)
-----|------|-------------|---|---
rect | [IntRect](#intrect-class) | The part of the texture (defined in the RenderComponent) to use during this frame. | ✓ | …
relative_duration | Number | The relative duration of this frame, relatively to the [Animation](#animation-class) total duration. | ✓ | …

## IntRect class

Represents a axis-aligned rectangle.

#### Attributes

Name | Type | Description | [U](#vocabulary) | [R](#vocabulary)
-----|------|-------------|---|---
x | Number | The X position of the top-left corner. | ✓ | …
y | Number | The Y position of the top-left corner. | ✓ | …
width | Number | The width of the rectangle. | ✓ | …
height | Number | The height of the rectangle. | ✓ | …
