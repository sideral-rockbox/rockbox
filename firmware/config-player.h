/* define this if you have a charcell LCD display */
#define HAVE_LCD_CHARCELLS 1

/* define this if you have the Player's keyboard */
#define HAVE_PLAYER_KEYPAD 1

/* Define this if you have a MAS3507D */
#define HAVE_MAS3507D

/* Define this if you have a DAC3550A */
#define HAVE_DAC3550A

/* Define this to the CPU frequency */
#define CPU_FREQ 12000000 /* cycle time ~83.3ns */

/* Battery scale factor (?) */
#define BATTERY_SCALE_FACTOR 6546

/* Define this if you must discharge the data line by driving it low
   and then set it to input to see if it stays low or goes high */
#define HAVE_I2C_LOW_FIRST

/* Define this if you control power on PADR (instead of PBDR) */
#define HAVE_POWEROFF_ON_PADR
