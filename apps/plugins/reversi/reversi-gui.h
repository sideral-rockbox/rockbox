/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (c) 2006 Alexander Levin
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef _REVERSI_GUI_H
#define _REVERSI_GUI_H

#include "plugin.h"

#define GAME_FILE         PLUGIN_GAMES_DIR "/reversi.rev"

/* variable button definitions */
#if CONFIG_KEYPAD == RECORDER_PAD
#define REVERSI_BUTTON_QUIT BUTTON_OFF
#define REVERSI_BUTTON_UP BUTTON_UP
#define REVERSI_BUTTON_DOWN BUTTON_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE BUTTON_PLAY
#define REVERSI_BUTTON_MENU BUTTON_F1

#elif CONFIG_KEYPAD == ONDIO_PAD
#define REVERSI_BUTTON_QUIT BUTTON_OFF
#define REVERSI_BUTTON_UP BUTTON_UP
#define REVERSI_BUTTON_DOWN BUTTON_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE_PRE BUTTON_MENU
#define REVERSI_BUTTON_MAKE_MOVE (BUTTON_MENU | BUTTON_REL)
#define REVERSI_BUTTON_ALT_MAKE_MOVE (BUTTON_MENU | BUTTON_DOWN)
#define REVERSI_BUTTON_MENU_PRE BUTTON_MENU
#define REVERSI_BUTTON_MENU (BUTTON_MENU | BUTTON_REPEAT)

#elif (CONFIG_KEYPAD == IRIVER_H100_PAD) || \
      (CONFIG_KEYPAD == IRIVER_H300_PAD)
#define REVERSI_BUTTON_QUIT BUTTON_OFF
#define REVERSI_BUTTON_UP BUTTON_UP
#define REVERSI_BUTTON_DOWN BUTTON_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE BUTTON_SELECT
#define REVERSI_BUTTON_MENU BUTTON_MODE

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#define REVERSI_BUTTON_UP BUTTON_MENU
#define REVERSI_BUTTON_DOWN BUTTON_PLAY
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_ALT_LEFT BUTTON_SCROLL_BACK
#define REVERSI_BUTTON_ALT_RIGHT BUTTON_SCROLL_FWD
#define REVERSI_BUTTON_MAKE_MOVE (BUTTON_SELECT | BUTTON_REL)
#define REVERSI_BUTTON_MENU (BUTTON_MENU | BUTTON_SELECT)

#elif (CONFIG_KEYPAD == IAUDIO_X5M5_PAD)
#define REVERSI_BUTTON_QUIT BUTTON_POWER
#define REVERSI_BUTTON_UP BUTTON_UP
#define REVERSI_BUTTON_DOWN BUTTON_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE BUTTON_SELECT
#define REVERSI_BUTTON_MENU BUTTON_PLAY

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)
#define REVERSI_BUTTON_QUIT BUTTON_A
#define REVERSI_BUTTON_UP BUTTON_UP
#define REVERSI_BUTTON_DOWN BUTTON_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE BUTTON_SELECT
#define REVERSI_BUTTON_MENU BUTTON_MENU

#elif (CONFIG_KEYPAD == IRIVER_H10_PAD)
#define REVERSI_BUTTON_QUIT BUTTON_POWER
#define REVERSI_BUTTON_UP BUTTON_SCROLL_UP
#define REVERSI_BUTTON_DOWN BUTTON_SCROLL_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE BUTTON_REW
#define REVERSI_BUTTON_MENU BUTTON_PLAY

#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
(CONFIG_KEYPAD == SANSA_C200_PAD)
#define REVERSI_BUTTON_QUIT BUTTON_POWER
#define REVERSI_BUTTON_UP BUTTON_UP
#define REVERSI_BUTTON_DOWN BUTTON_DOWN
#define REVERSI_BUTTON_LEFT BUTTON_LEFT
#define REVERSI_BUTTON_RIGHT BUTTON_RIGHT
#define REVERSI_BUTTON_MAKE_MOVE BUTTON_SELECT
#define REVERSI_BUTTON_MENU (BUTTON_SELECT|BUTTON_REPEAT)

#elif CONFIG_KEYPAD == MROBE500_PAD
#define REVERSI_QUIT                BUTTON_POWER
#define REVERSI_BUTTON_UP           BUTTON_RC_PLAY
#define REVERSI_BUTTON_DOWN         BUTTON_RC_DOWN
#define REVERSI_BUTTON_LEFT         BUTTON_RC_REW
#define REVERSI_BUTTON_RIGHT        BUTTON_RC_FF
#define REVERSI_BUTTON_MAKE_MOVE    BUTTON_RC_MODE
#define REVERSI_BUTTON_MENU         (BUTTON_POWER | BUTTON_REL)

#else
  #error REVERSI: Unsupported keypad
#endif


/* Modes for the cursor behaviour at the board edges  */
typedef enum _cursor_wrap_mode_t {
    WRAP_FLAT,   /* No wrapping */
    WRAP_SPHERE, /* (7,7) > right > (7,0); (7,7) > down > (0,7) */
    WRAP_TORUS,  /* (7,7) > right > (0,0); (7,7) > down > (0,0) */
} cursor_wrap_mode_t;


#endif
