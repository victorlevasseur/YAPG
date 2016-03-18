Lua runtime
===========

This section describe how to write lua script / function meant to be
executed during the level runtime.

Callback functions
------------------

Some components have parameters of type "function". Thsese are lua
function that are called during runtime.

*For example, the PlayerComponent has a ``onStartWalking`` attribute
that is called when the player starts moving. In the players templates
provided in the vanilla game, it is often used to change the animation
of the player.*

Most callback function receive an argument called ``entity``, it's a
reference on the entity concerned. *In the previous example, the
``onStartWalking`` function receive an ``entity`` argument that is the
player entity that starts walking.*

**Warning: when the functions are executed (during runtime), the lua
table representing the template is not accessible (it's just a template,
not the instanciated entity). You must act on the entity instance (given
by the ``entity`` argument).**

Here is an example of the callback function that changes the animation
of the player (template ``kenney/player1.lua``) when he jumps:

.. code:: lua

    -- The other components here
    ["Platformer"] = {
        --... (other attributes)
        onStartJumping = function(entity) --The callback function
            entity:set_attribute("Render", "current_animation", string_value("jump"))
        end,
        --... (other attributes)
    }

The ``entity`` object
---------------------

As previously said, functions often have an ``entity`` parameter. It
represents a particular entity instance.

Methods
^^^^^^^

-  ``destroy()`` : destroys the entity
-  ``get_attribute("componentName", "attributeName")`` : get the value
   of an attribute of the entity
-  ``set_attribute("componentName", "attributeName", newValue)`` : set
   the value of an attribute of the entity

Usage
^^^^^

In the last two methods, ``"componentName"`` is the name of the
Component owning the attribute. **Do not put** ``"Component"`` **at the end of
the name: for example, for the PositionComponent, just type**
``"Position"``.

The ``"attributeName"`` is the name of the attribute (as given in the
component's documentation).

The ``get/set_attribute`` methods don't work directly with values. In
fact, ``get_attribute`` return a small **proxy object** containing the
value and for ``set_attribute``, the ``newValue`` must be a **proxy
object** too.

However, the game provides functions to convert from/to these proxy
objects. These functions are :

-  ``as_xxx()``: converts from the proxy object returned by
   ``get_attribute`` to the xxx type.
-  ``xxx_value()``: converts a value of type xxx to a proxy object usable
   by ``set_attribute``.

``xxx`` must be the name of the attribute's type. See the :doc:`components` to
see the precise type of each attributes.

**Warning: using int_value()/as_int() for an attribute of type
float will not work. You must use the exactly correct type for
converting proxy objects.**

*Note:* Why proxy objects ? These is mainly because the game engine is
written in C++ which is a statically typed langage. They allow the
engine to work with any types of values to receive or change attributes'
values.

Example
^^^^^^^

This example demonstates how to add 200 pixels to the X position of an
entity:

.. code:: lua

    -- considering that "entity" is the variable holding the entity instance.
    local old_x_pos = as_float(entity:get_attribute("Position", "x"))
    entity:set_attribute("Position", "x", float_value(old_x_pos + 200))

This other example shows how to change an animation duration of an entity:

.. code:: lua

    -- considering that "entity" is the variable holding the entity instance.
    local animations_table = {}
    --
    -- get_table_attribute is a special method allowing to get array and map attributes of entities' components.
    -- you must give it an already existing table where the result is put.
    entity:get_table_attribute("Render", "animations", animations_table)
    --
    -- We are going to change the duration of the animation named "default"
    local theAnimationToChange = animations_table["default"]
    theAnimationToChange.duration = 2
    -- Note that theAnimationToChange is not a table but an animation object.
    -- It's not the serialized version of an animation but the runtime object used by the game engine.
    --
    -- update the animation attribute with the table
    entity:set_table_attribute("Render", "animations", animations_table)

Per entity instance storage
---------------------------

It's sometimes useful to be able to store custom data in entities for a later use
in a callback. As callbacks are used by all entities instanciated from a template,
you can't use local variables to store data related to a particular entity instance.
But you're lucky, YAPG provides a special component called ``CustomDataComponent``
that can contains **any type of object** (supported by the engine).

This component is quite different from the others because it doesn't have any preset
attributes and it doesn't need to be declared in a template to be created for each
instanciated entities.

Gettting and settings values from/to ``CustomDataComponent``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``get_attribute`` and ``set_attribute`` are not used to get or set a value inside it
(remember, it doesn't have real attributes). But the component itself is accessible through
the ``get_custom_data`` method of ``entity``. This method returns the custom data component.
It has the following methods:

 - ``get_value(field)``: returns a proxy object containing the value stored in a field
 - ``set_value(field, value)``: set the value stored in a field (``value`` must be a proxy object)
 - ``has_value(field)``: returns ``true`` if the specified field exists, ``false`` otherwise.

**Warning:** These methods work with proxy object as the ``get/set_attribute`` methods
of ``entity``. So, the same rules apply: you must use the ``as_XXX(...)`` and ``XXX_value(...)``
conversion functions to get or set values.

Define default values
^^^^^^^^^^^^^^^^^^^^^

To set default values to some fields, you need to declare the component in the template's
``components`` table and just write key/values pairs. **You also need to use the** ``XXX_value(...)``
**conversion function according to which type of data you want to store.**

**Example of declaring a default string value to a field in** ``CustomDataComponent``:

.. code:: lua

    -- Some other stuff

    components = {
        -- Other components here

        ["CustomDataComponent"] = {
            my_custom_state = string_value("idle state"),
        },

        -- Other components here too
    },

    --Some other stuff

Use a ``CustomDataComponent`` field as a template parameter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

It is **possible under some conditions** to have a ``CustomDataComponent`` field as a
template parameter:

 - the field **must have a default value** defined in the template (so that YAPG knows its type)
 - the type of the field **must be supported by the editor**

The syntax for these special parameters is a bit different from the others: instead of
``component`` and ``attribute`` keys, it has a ``custom_data_field`` that contains the name
of the field linked to the parameter.

**Example (use the code of the previous example):**

.. code:: lua

    -- Some other stuff

    parameters = {
        my_field_parameter = {
            name = "My special parameter!",
            custom_data_field = "my_custom_state",
        }
    }

    -- Some other stuff
