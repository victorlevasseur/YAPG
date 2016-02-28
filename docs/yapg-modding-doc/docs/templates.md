# Templates

## What is a template ?

When loading a level, the engine **instanciate** multiples **templates** to
create the objects on screen. An **template** is a model that describe
how an **entity** (created with it) should act in the game. For example, a
"grass floor" entity template describe the size of a grass floor block,
which texture it should display and that the player can go through it. In
the level, this **template** can be **instanciated** multiple times so
that multiple "grass floor" **entities** are displayed on screen.

However, when a **template** is **instanciated**, it can have some variations (*for example, the "grass floor" template is not always instanciated at the same position*). The data that can change depending
a particular instanciation are called ** the template parameters**. *For
a lot of templates, the X and Y position will be parameters.*

To avoid useless repetition, templates can **inherit** other templates so that the properties of the inherited template are repeated and even overridden in the "child" template. The template also inherits the parameters.

## Writing templates

Templates are written in **lua** and must be located in the `templates` folder of the game so that it can detect them.

They must follow this structure:

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
        }
        components = {
            ["ComponentNumberOne"] = {
                component_attribute_one = "value",
                another_attribute = "different value",
            },
            ["ComponentNumberTwo"] = {
                component2_attribute_one = "second value",
                another_attribute2 = "different value 2",
            },
            ...
        }
    }

### The components

The `components` field contains an array of component's tables describing the values of their attributes when an entity is instanciated with the template. For example, the `"Render"` components contains attributes describing the texture used to display the entity in-game. **Each entity instanciated will receive all the components listed in the corresponding template initialized with the values written in each component's tables.**

In the above code, if the game instanciates an entity with this template, the entity will be initialized with components `"ComponentNumberOne"` and `"ComponentNumberTwo"` and with their attributes set to the values written in the tables (the attribute `component_attribute_one` of `"ComponentNumberOne"` will be initialized with `"value"`). This is important to understand that after instanciation, the attributes of the entities' components can change independently: a mysterious script can decide to change the texture used by one and only one particular **instance** (entity) of the template.

All availables components and their respective attributes are described in the *"Entities components reference"* section.

### The parameters
