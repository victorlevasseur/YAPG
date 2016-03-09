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
Component owning the attribute. **Do not put ``"Component"`` at the end of
the name: for example, for the PositionComponent, just type
``"Position"``.**

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
