# LVLEDIT
Level Editor for single TLB (Temi Level Binary) level files, used in developing games for TEMI, a 2D games engine.
## What is TLB?
TLB is a run-length format for storing a single 20 x 15 tile screen. The format first records the tile information until 300 tiles are accounted for. The format then records entity data.
### Section 1: Tile data
Read the file until 300 tiles are accounted for. If this does account for exactly 300 tiles, then something is wrong with the file. Read the file in 3-byte groups as so:
 - Graphic ID (u8)
 - Property ID (u8)
 - Repeat (u8)
The graphic ID informs what tile to reference (defined elsewhere). The property ID informs how the user will interact with this tile (defined by the engine). Repeat informs how many additional tiles this group defines (0 = just one tile, 255 = 256 tiles).
### Section 2: Entity Data
Continue reading the file in 3-byte sections until you reach an entity of type 0 at any coordinate. Read the file in 3-byte groups as so:
 - X coordinate (i8)
 - Y coordinate (i8)
 - Type (u8)
### Section 3: Validation
The following properties are true of all valid TLB files:
 - A size in bytes that is a positive integer multiple of 3, and be at least 9 bytes long.
 - There must be an arrangement of 3-byte groups starting from the first group where the sum of the third bytes added to the number of groups read in are equal to 300.
 - The final group must have the third byte as a value of zero, which is separate of the tile data.
The minimum size of the file is not an obligation of enforcement, but an emergent property of the format. Because a single group cannot define the entire canvas of tile (as 256 < 300), two groups are required. Because the entity read is terminated by a null entity, one more group is required.
