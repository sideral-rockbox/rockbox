/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Pacbox - a Pacman Emulator for Rockbox
 *
 * Based on PIE - Pacman Instructional Emulator
 *
 * Copyright (c) 1997-2003,2004 Alessandro Scotti
 * http://www.ascotti.org/
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "plugin.h"
#include "arcade.h"

PLUGIN_HEADER

#ifdef USE_IRAM
extern char iramcopy[];
extern char iramstart[];
extern char iramend[];
extern char iedata[];
extern char iend[];
#endif

/* How many video frames (out of a possible 60) we display each second */
#define FPS 20

#if CONFIG_KEYPAD == IPOD_4G_PAD

#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_MENU
#define PACMAN_RIGHT   BUTTON_PLAY
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_COIN    BUTTON_SELECT

#elif CONFIG_KEYPAD == IRIVER_H100_PAD || CONFIG_KEYPAD == IRIVER_H300_PAD

#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_UP
#define PACMAN_RIGHT   BUTTON_DOWN
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_ON
#define PACMAN_COIN    BUTTON_REC
#define PACMAN_MENU    BUTTON_MODE

#elif CONFIG_KEYPAD == GIGABEAT_PAD

#define PACMAN_UP      BUTTON_UP
#define PACMAN_DOWN    BUTTON_DOWN
#define PACMAN_LEFT    BUTTON_LEFT
#define PACMAN_RIGHT   BUTTON_RIGHT
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_POWER
#define PACMAN_COIN    BUTTON_A
#define PACMAN_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_X5_PAD

#define PACMAN_UP      BUTTON_RIGHT
#define PACMAN_DOWN    BUTTON_LEFT
#define PACMAN_LEFT    BUTTON_UP
#define PACMAN_RIGHT   BUTTON_DOWN
#define PACMAN_1UP     BUTTON_SELECT
#define PACMAN_2UP     BUTTON_POWER
#define PACMAN_COIN    BUTTON_REC
#define PACMAN_MENU    BUTTON_PLAY

#endif

#if (LCD_HEIGHT >= 288)
#define XOFS ((LCD_WIDTH-224)/2)
#define YOFS ((LCD_HEIGHT-288)/2)
#elif (LCD_WIDTH >= 288)
#define XOFS ((LCD_WIDTH-288)/2)
#define YOFS ((LCD_HEIGHT-224)/2)
#elif (LCD_WIDTH >= 220)
#define XOFS ((LCD_WIDTH-(288*3/4))/2)
#define YOFS ((LCD_HEIGHT-(224*3/4))/2)
#elif (LCD_WIDTH >= 144)
#define XOFS ((LCD_WIDTH-288/2)/2)
#define YOFS ((LCD_HEIGHT-224/2)/2)
#endif

struct plugin_api* rb;

unsigned framesPerSecond    = VideoFrequency;
unsigned long frame_counter = 0;

struct pacman_settings {
    int difficulty;
    int numlives;
    int bonus;
    int ghostnames;
    int showfps;
};

struct pacman_settings settings;

bool loadFile( const char * name, unsigned char * buf, int len )
{
    char filename[MAX_PATH];

    rb->snprintf(filename,sizeof(filename),"/.rockbox/pacman/%s",name);

    int fd = rb->open( filename, O_RDONLY);

    if( fd < 0 ) {
        return false;
    }
    
    int n = rb->read( fd, buf, len);
    
    rb->close( fd );

    if( n != len ) {
        return false;
    }

    return true;
}

bool loadROMS( void )
{
    bool romsLoaded = false;

    romsLoaded = loadFile( "pacman.6e", ram_,           0x1000) &&
                 loadFile( "pacman.6f", ram_+0x1000,    0x1000) &&
                 loadFile( "pacman.6h", ram_+0x2000,    0x1000) &&
                 loadFile( "pacman.6j", ram_+0x3000,    0x1000) &&
                 loadFile( "pacman.5e", charset_rom_,   0x1000) &&
                 loadFile( "pacman.5f", spriteset_rom_, 0x1000);

    if( romsLoaded ) {
        decodeROMs();
        reset_PacmanMachine();
    }

    return romsLoaded;
}

/* A buffer to render Pacman's 244x288 screen into */
unsigned char background[ScreenWidth*ScreenHeight] __attribute__ ((aligned (4)));
unsigned char video_buffer[ScreenWidth*ScreenHeight] __attribute__ ((aligned (4)));

long start_time;
long sleep_counter = 0;
long video_frames = 0;

int dipDifficulty[] = { DipDifficulty_Normal, DipDifficulty_Hard };
int dipLives[] = { DipLives_1, DipLives_2, DipLives_3, DipLives_5 };
int dipBonus[] = { DipBonus_10000, DipBonus_15000, DipBonus_20000, DipBonus_None };
int dipGhostNames[] = { DipGhostNames_Normal, DipGhostNames_Alternate };

int settings_to_dip(struct pacman_settings settings)
{
    return ( DipPlay_OneCoinOneGame | 
             DipCabinet_Upright | 
             DipMode_Play |
             DipRackAdvance_Off |

             dipDifficulty[settings.difficulty] |
             dipLives[settings.numlives] |
             dipBonus[settings.bonus] |
             dipGhostNames[settings.ghostnames]
           );
}



int pacbox_menu_cb(int key, int m)
{
    (void)m;
    switch(key)
    {
#ifdef MENU_ENTER2
    case MENU_ENTER2:
#endif
    case MENU_ENTER:
        key = BUTTON_NONE; /* eat the downpress, next menu reacts on release */
        break;

#ifdef MENU_ENTER2
    case MENU_ENTER2 | BUTTON_REL:
#endif
    case MENU_ENTER | BUTTON_REL:
        key = MENU_ENTER; /* fake downpress, next menu doesn't like release */
        break;
    }

    return key;
}

bool pacbox_menu(void)
{
    int m;
    int result;
    int menu_quit=0;
    int new_setting;
    bool need_restart = false;

    static const struct opt_items noyes[2] = {
        { "No", NULL },
        { "Yes", NULL },
    };

    static const struct opt_items difficulty_options[2] = {
        { "Normal", NULL },
        { "Harder", NULL },
    };

    static const struct opt_items numlives_options[4] = {
        { "1", NULL },
        { "2", NULL },
        { "3", NULL },
        { "5", NULL },
    };

    static const struct opt_items bonus_options[4] = {
        { "10000 points", NULL },
        { "15000 points", NULL },
        { "20000 points", NULL },
        { "No bonus", NULL },
    };

    static const struct opt_items ghostname_options[2] = {
        { "Normal", NULL },
        { "Alternate", NULL },
    };

    static const struct menu_item items[] = {
        { "Difficulty", NULL },
        { "Pacmen Per Game", NULL },
        { "Bonus Life", NULL },
        { "Ghost Names", NULL },
        { "Display FPS", NULL },
        { "Restart", NULL },
        { "Quit", NULL },
    };
    
    m = rb->menu_init(items, sizeof(items) / sizeof(*items),
                      pacbox_menu_cb, NULL, NULL, NULL);

    rb->button_clear_queue();

    while (!menu_quit) {
        result=rb->menu_show(m);

        switch(result)
        {
            case 0:
                new_setting=settings.difficulty;
                rb->set_option("Difficulty", &new_setting, INT,
                               difficulty_options , 2, NULL);
                if (new_setting != settings.difficulty) {
                    settings.difficulty=new_setting;
                    need_restart=true;
                }
                break;
            case 1:
                new_setting=settings.numlives;
                rb->set_option("Pacmen Per Game", &new_setting, INT,
                               numlives_options , 4, NULL);
                if (new_setting != settings.numlives) {
                    settings.numlives=new_setting;
                    need_restart=true;
                }
                break;
            case 2:
                new_setting=settings.bonus;
                rb->set_option("Bonus Life", &new_setting, INT,
                               bonus_options , 4, NULL);
                if (new_setting != settings.bonus) {
                    settings.bonus=new_setting;
                    need_restart=true;
                }
                break;
            case 3:
                new_setting=settings.ghostnames;
                rb->set_option("Ghost Names", &new_setting, INT,
                               ghostname_options , 2, NULL);
                if (new_setting != settings.ghostnames) {
                    settings.ghostnames=new_setting;
                    need_restart=true;
                }
                break;
            case 4: /* Show FPS */
                rb->set_option("Display FPS",&settings.showfps,INT, noyes, 2, NULL);
                break;
            case 5: /* Restart */
                need_restart=true;
                menu_quit=1;
                break;
            default:
                menu_quit=1;
                break;
        }
    }

    rb->menu_exit(m);

    if (need_restart) {
        init_PacmanMachine(settings_to_dip(settings));
    }

    /* Possible results: 
         exit game
         restart game
         usb connected
    */
    return (result==6);
}


/*
    Runs the game engine for one frame.
*/
int gameProc( void )
{
    int x,y;
    int fps;
    char str[80];
    int status;
    long end_time;
    unsigned char* vbuf = video_buffer;
    fb_data* dst;
    fb_data* next_dst;

    /* Run the machine for one frame (1/60th second) */
    run();

    frame_counter++;

    /* Check the button status */
    status = rb->button_status();

#ifdef PACMAN_MENU
    if (status & PACMAN_MENU) {
#else
    if (rb->button_hold()) {
#endif
        end_time = *rb->current_tick;
        x = pacbox_menu();
        rb->lcd_clear_display();
#ifdef HAVE_REMOTE_LCD
        rb->lcd_remote_clear_display();
        rb->lcd_remote_update();
#endif
        if (x == 1) { return 1; }
        start_time += *rb->current_tick-end_time;
    }

    setDeviceMode( Joy1_Left, (status & PACMAN_LEFT) ? DeviceOn : DeviceOff);
    setDeviceMode( Joy1_Right, (status & PACMAN_RIGHT) ? DeviceOn : DeviceOff);
    setDeviceMode( Joy1_Up, (status & PACMAN_UP) ? DeviceOn : DeviceOff);
    setDeviceMode( Joy1_Down, (status & PACMAN_DOWN) ? DeviceOn : DeviceOff);
    setDeviceMode( CoinSlot_1, (status & PACMAN_COIN) ? DeviceOn : DeviceOff);
    setDeviceMode( Key_OnePlayer, (status & PACMAN_1UP) ? DeviceOn : DeviceOff);
#ifdef PACMAN_2UP
    setDeviceMode( Key_TwoPlayers, (status & PACMAN_2UP) ? DeviceOn : DeviceOff);
#endif

    /* We only update the screen every third frame - Pacman's native 
       framerate is 60fps, so we are attempting to display 20fps */
    if( (frame_counter % (60/FPS)) == 0) {

        video_frames++;

        /* The following functions render the Pacman screen from the contents
           of the video and color ram.  We first update the background, and
           then draw the Sprites on top. 

           Note that we only redraw the parts of the background that have
           changed, which is why we need to keep a copy of the background without
           the sprites on top.  Even with the memcpy, this is faster than redrawing
           the whole background.
        */
        renderBackground( background );
        rb->memcpy(video_buffer,background,sizeof(video_buffer));
        renderSprites( video_buffer );

#ifdef HAVE_LCD_COLOR
#if (LCD_WIDTH >= 224) && (LCD_HEIGHT >= 288)
        /* Native resolution = 224x288 */
        (void)next_dst;
        dst=&rb->lcd_framebuffer[YOFS*LCD_WIDTH+XOFS];
        for (y=0;y<ScreenHeight;y++) {
            for (x=0;x<ScreenWidth;x++) {
                *(dst++) = palette[*(vbuf++)];
            }
            dst += XOFS*2;
        }
#elif (LCD_WIDTH >= 288) && (LCD_HEIGHT >= 224)
        /* Native resolution - rotated 90 degrees = 288x224 */
        next_dst=&rb->lcd_framebuffer[YOFS*LCD_WIDTH+XOFS+ScreenHeight-1];
        for( y=ScreenHeight-1; y>=0; y-- ) {
            dst = (next_dst--);
            for( x=0; x<ScreenWidth; x++ ) {
                *dst = palette[*(vbuf++)];
                dst+=LCD_WIDTH;
            }
        }
#elif (LCD_WIDTH >= 216) && (LCD_HEIGHT >= 168)
        /* 0.75 scaling - display 3 out of 4 pixels = 216x168 
           Skipping pixel #2 out of 4 seems to give the most legible display 
         */
        next_dst=&rb->lcd_framebuffer[YOFS*LCD_WIDTH+XOFS+((ScreenHeight*3)/4)-1];
        for (y=ScreenHeight-1;y >= 0; y--) {
            if ((y & 3) != 1) {
                dst = (next_dst--);
                for (x=0;x<ScreenWidth;x++) {
                    if ((x & 3) == 1) { vbuf++; }
                    else {
                       *dst = palette[*(vbuf++)];
                       dst+=LCD_WIDTH;
                    }
                }
            } else {
                vbuf+=ScreenWidth;
            }
        }
#elif (LCD_WIDTH >= 144) && (LCD_HEIGHT >= 112)
        /* 0.5 scaling - display every other pixel = 144x112 */
        next_dst=&rb->lcd_framebuffer[YOFS*LCD_WIDTH+XOFS+ScreenHeight/2-1];
        for (y=(ScreenHeight/2)-1;y >= 0; y--) {
            dst = (next_dst--);
            for (x=0;x<ScreenWidth/2;x++) {
                *dst = palette[*(vbuf)];
                vbuf+=2;
                dst+=LCD_WIDTH;
            }
            vbuf+=ScreenWidth;
        }
#endif
#else  /* Greyscale LCDs */
#if (LCD_WIDTH >= 144) && (LCD_HEIGHT >= 112)
#if LCD_PIXELFORMAT == VERTICAL_PACKING
        /* 0.5 scaling - display every other pixel = 144x112 */
        next_dst=&rb->lcd_framebuffer[YOFS/4*LCD_WIDTH+XOFS+ScreenHeight/2-1];
        for (y=(ScreenHeight/2)-1;y >= 0; y--) {
            dst = (next_dst--);
            for (x=0;x<ScreenWidth/8;x++) {
                *dst = (palette[*(vbuf+6)]<<6) | (palette[*(vbuf+4)] << 4) | (palette[*(vbuf+2)] << 2) | palette[*(vbuf)];
                vbuf+=8;
                dst+=LCD_WIDTH;
            }
            vbuf+=ScreenWidth;
        }
#endif /* Vertical Packing */
#endif /* Size >= 144x112 */
#endif /* Not Colour */

        if (settings.showfps) {
            fps = (video_frames*HZ*100) / (*rb->current_tick-start_time);
            rb->snprintf(str,sizeof(str),"%d.%02d / %d fps  ",fps/100,fps%100,
                                         FPS);
            rb->lcd_putsxy(0,0,str);
        }

        rb->lcd_update();

#ifdef SIMULATOR
        /* Keep the framerate at Pacman's 60fps */
        end_time = start_time + (video_frames*HZ)/FPS;
        while (TIME_BEFORE(*rb->current_tick,end_time)) {
             rb->sleep(1);
        }
#endif
    }

    return 0;
}

enum plugin_status plugin_start(struct plugin_api* api, void* parameter)
{
    (void)parameter;
    int status;

    rb = api;

#ifdef USE_IRAM
    rb->memcpy(iramstart, iramcopy, iramend-iramstart);
    rb->memset(iedata, 0, iend - iedata);
#endif

#ifdef HAVE_ADJUSTABLE_CPU_FREQ
    rb->cpu_boost(true);
#endif

    rb->lcd_set_foreground(LCD_WHITE);
    rb->lcd_set_background(LCD_BLACK);
    rb->lcd_clear_display();
    rb->lcd_update();

    /* Set the default settings (we should load these from a file) */
    settings.difficulty = 0; /* Normal */
    settings.numlives = 2;   /* 3 lives */
    settings.bonus = 0;      /* 10000 points */
    settings.ghostnames = 0; /* Normal names */
    settings.showfps = 0;    /* Do not show FPS */

    /* Initialise the hardware */
    init_PacmanMachine(settings_to_dip(settings));

    /* Load the romset */
    if (loadROMS()) {
        start_time = *rb->current_tick-1;
        do {
            status = gameProc();
        } while (!status);
    } else {
        rb->splash(HZ*2,true,"No ROMs in /.rockbox/pacman/");
    }

#ifdef HAVE_ADJUSTABLE_CPU_FREQ
    rb->cpu_boost(false);
#endif

    return PLUGIN_OK;
}
