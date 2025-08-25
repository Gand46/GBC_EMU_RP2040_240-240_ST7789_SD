Game Boy Color emulator for PicoPad.

Screen Scaling
--------------
The Game Boy frame buffer is 160x144 while the target display is 240x240.
Scaling is performed with a fixed Bresenham-style cadence without filtering.

- Horizontal 2→3: each pair of source pixels expands to three destination pixels using the pattern [S0,S0,S1].
- Vertical 3→5: each trio of source rows expands to five destination rows using the pattern [R0,R1,R1,R2,R2].

The algorithm uses only additions and comparisons, making it very fast and
keeping the image geometry unchanged.
