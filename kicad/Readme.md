# Kicad Drawings and Computer Organization

Before you get into the Kicad schematics, I want to touch on the computer layout a bit.  Overall, the computer is laid out in 3 columns with a bus between each column.

There are several breadboards in each column, with columns 1 & 3 having 13 breadboard and the middle column (2) having 12.  The extra space is taken up with a "bus bridge" to link the 2 buses together.

This is important for these drawings since the naming convention will indicate the breadboard location for any given schematic.  However, the computer is also organized into modules and some components may exist on a separate breadboard.

As an example, each 16-bit register has a 74LS04 Quad-AND Gate and a 74LS08 Hex-Inverter.  Each register is built on 2 breadboards.  One will have the 74LS04 and the other will have the 74LS08.  However, all the logic symbols will be located on a single schematic.  These facts need to be taken into account if you want to duplicate the boards.

So, that said, each schematic drawing will have a 2-character location specification where:
* the first character indicates its column (1, 2, 3)
* the second character indicates which row (hex from 0 to C)

See the [computer layout](../Readme.md#overall-layout) for more information.



