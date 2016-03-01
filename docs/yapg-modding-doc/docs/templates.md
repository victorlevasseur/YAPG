# Templates

## What is a template ?

When loading a level, the engine **instanciate** multiples **templates** to create the objects on screen. An **template** is a model that describe how an **entity** (created with it) should act in the game. For example, a "grass floor" entity template describe the size of a grass floor block, which texture it should display and that the player can't go through it. In the level, this **template** can be **instanciated** multiple times so that multiple "grass floor" **entities** are displayed on screen.

However, when a **template** is **instanciated**, it can have some variations (*for example, the "grass floor" template is not always instanciated at the same position*). The data that can change depending a particular instanciation are called ** the template parameters**. *For a lot of templates, the X and Y position will be parameters.*

To avoid useless repetition, templates can **inherit** other templates so that the properties of the inherited template are repeated and even overridden in the "child" template. The template also inherits the parameters.

## Writing templates

Templates are written in **lua** and must be located in the `templates` folder of the game so that it can detect them.

They must follow this structure:

```lua
entity_template = {
    name = "a_unique_identifier",
    friendlyname = "A more friendly name, displayed in level editor",
    inherits = "the_inherited_template_optional!",
    parameters = {
        parameter_one = {
            name = "The value of component_attribute_one",
            component = "ComponentNumberOne",
            attribute = "component_attribute_one",
        },
        ...
    },
    components = {
        ["ComponentNumberOne"] = {
            component_attribute_one = "value",
            another_attribute = "different value",
        },
        ["ComponentNumberTwo"] = {
            component2_attribute_one = "second value",
            another_attribute2 = "different value 2",
            a_callback = function(entity)
                --A function that can act on the entity
            end
        },
        ...
    }
}
```

### The components

The `components` field contains an array of component's tables describing the values of their attributes when an entity is instanciated with the template. For example, the `"Render"` components contains attributes describing the texture used to display the entity in-game. **Each entity instanciated will receive all the components listed in the corresponding template initialized with the values written in each component's tables.**

In the above code, if the game instanciates an entity with this template, the entity will be initialized with components `"ComponentNumberOne"` and `"ComponentNumberTwo"` and with their attributes set to the values written in the tables (the attribute `component_attribute_one` of `"ComponentNumberOne"` will be initialized with `"value"`). This is important to understand that after instanciation, the attributes of the entities' components can change independently: a mysterious script can decide to change the texture used by one and only one particular **instance** (entity) of the template.

**All availables components, how to write their attributes' values is described in detail in the "Lua/XML serialization" section. See [Basic types](native-types.md), [Components](components/components.md) and [Utility classes](components/utility-classes.md).**

You can also use the default templates as examples for writing new ones.

### The parameters

As stated in the first sub-section, **template parameters** allow the game to add some variation to the instanciated entities: each entities instanciated from a template can be instanciated with different values in their parameters.

**The parameters are declared in the `parameters` table as an array of parameters.** The key (on the left of "=") must be a unique name (for all the parameters in that template). Inside the table (inside "{}"), a friendly name is given (`name` field), the concerned component (`component` field) is also written and the name of this component's attribute is written.

In the example above, this means that the template has one parameter called `parameter_one` (with its friendly name being "The value of component_attribute_one"). This parameter, when set during an entity instanciation, changes the value affected to the attribute `component_attribute_one` of the component `ComponentNumberOne` (this means that the default value of this attribute, which is `"value"` can be overridden by the parameter value if provided).

**Parameters are not mandatory to** instanciate an entity, so be sure to provide a value in to the concerned attributes in the `components` table.

They are used mainly in the level editor (where they can be edited by the user) and in the level file that, for each entity, stores its template name and the values of the parameters.

Please note that not all components' attributes can be used as template parameters. See [Components reference > Components section](components/components.md) for further information.
