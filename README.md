# lowocale
linux locale uwuifier

## how to use
- clone or download the code, whatever works
- open a terminal, cd to wherever you put the files
- if you want to uwuify specific locale files only:
  - compile `lowocale.c` with `cc lowocale.c -o lowocale`
  - run `./lowocale <path to input file> <path to output file>`
- if you want to uwu everything:
  - run `sudo bash lowocalize.sh <your locale here>`
  - this should probably work, only tested on en_US void linux though

## things to note
this only uwuifies system locale files in the GNU MO format.
any programs that handle localization differently will not be uwuified.
additionally, any special formatting in locale files other than C formatting specifiers may break.
use at your own risk
