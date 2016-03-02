Utility classes
===============

Utility classes are classes used by components to represent some
specific data structures.

Vocabulary
----------

In the attribute's tables below:

-  The "U" column tells if the attributes can be unserialized from
   lua/XML.
-  The "R" column tells if the attributes can be changed at runtime with
   lua scripting using the ``myObject.attribute = newValue`` syntax.

"✓" means "yes", "…" means that it will be included in a future version
(planned) and nothing means "no".

.. _utility_classes_animation:

animation class
---------------

Attributes
^^^^^^^^^^

+------------+-------------------------------------+----------------------------------------------------+-----------------------+-----------------------+
| Name       | Type                                | Description                                        | `U <#vocabulary>`__   | `R <#vocabulary>`__   |
+============+=====================================+====================================================+=======================+=======================+
| duration   | float                               | The total duration of the animation, in seconds.   | ✓                     | …                     |
+------------+-------------------------------------+----------------------------------------------------+-----------------------+-----------------------+
| frames     | array of `frame <#frame-class>`__   | The frames of the animation.                       | ✓                     | …                     |
+------------+-------------------------------------+----------------------------------------------------+-----------------------+-----------------------+

Lua serialization example
^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: lua

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

.. _utility_classes_frame:

frame class
-----------

Attributes
^^^^^^^^^^

+----------------------+-----------------------------------+-----------------------------------------------------------------------------------------------------------+-----------------------+-----------------------+
| Name                 | Type                              | Description                                                                                               | `U <#vocabulary>`__   | `R <#vocabulary>`__   |
+======================+===================================+===========================================================================================================+=======================+=======================+
| rect                 | `int\_rect <#int_rect-class>`__   | The part of the texture (defined in the RenderComponent) to use during this frame.                        | ✓                     | …                     |
+----------------------+-----------------------------------+-----------------------------------------------------------------------------------------------------------+-----------------------+-----------------------+
| relative\_duration   | float                             | The relative duration of this frame, relatively to the `Animation <#animation-class>`__ total duration.   | ✓                     | …                     |
+----------------------+-----------------------------------+-----------------------------------------------------------------------------------------------------------+-----------------------+-----------------------+

.. _utility_classes_int_rect:

int\_rect class
---------------

Represents a axis-aligned rectangle.

Attributes
^^^^^^^^^^

+----------+--------+------------------------------------------+-----------------------+-----------------------+
| Name     | Type   | Description                              | `U <#vocabulary>`__   | `R <#vocabulary>`__   |
+==========+========+==========================================+=======================+=======================+
| x        | int    | The X position of the top-left corner.   | ✓                     | …                     |
+----------+--------+------------------------------------------+-----------------------+-----------------------+
| y        | int    | The Y position of the top-left corner.   | ✓                     | …                     |
+----------+--------+------------------------------------------+-----------------------+-----------------------+
| width    | int    | The width of the rectangle.              | ✓                     | …                     |
+----------+--------+------------------------------------------+-----------------------+-----------------------+
| height   | int    | The height of the rectangle.             | ✓                     | …                     |
+----------+--------+------------------------------------------+-----------------------+-----------------------+

.. _utility_classes_polygon:

polygon class
-------------

Represents a convex polygon.

Attributes
^^^^^^^^^^

+----------+-----------------------------------------+---------------------------+-----------------------+-----------------------+
| Name     | Type                                    | Description               | `U <#vocabulary>`__   | `R <#vocabulary>`__   |
+==========+=========================================+===========================+=======================+=======================+
| points   | array of `vector2f <#vector2f-class>`__ | The points of the hitbox. | ✓                     |                       |
+----------+-----------------------------------------+---------------------------+-----------------------+-----------------------+

Lua serialization example
^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: lua

    {
        points = {
            {
                x = 0,
                y = 0
            },
            {
                x = 64,
                y = 0
            },
            {
                x = 64,
                y = 64
            },
            {
                x = 0,
                y = 64
            },
        }
    }


.. _utility_classes_vector2f:

vector2f class
--------------

Represents a point with two float coordinates.

Attributes
^^^^^^^^^^

+----------+-----------------------------------+---------------------------+-----------------------+-----------------------+
| Name     | Type                              | Description               | `U <#vocabulary>`__   | `R <#vocabulary>`__   |
+==========+===================================+===========================+=======================+=======================+
| x        | float                             | The X coordinate          | ✓                     | …                     |
+----------+-----------------------------------+---------------------------+-----------------------+-----------------------+
| y        | float                             | The Y coordinate          | ✓                     | …                     |
+----------+-----------------------------------+---------------------------+-----------------------+-----------------------+
