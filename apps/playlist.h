/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by wavey@wavey.org
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include <stdbool.h>
#include "file.h"

/* playlist data */

#define MAX_PLAYLIST_SIZE 10000
typedef struct 
{
    char filename[MAX_PATH];  /* path name of m3u playlist on disk       */
    int  dirlen;         /* Length of the path to the playlist file */
    int  indices[MAX_PLAYLIST_SIZE]; /* array of indices            */
    int  index;          /* index of *NEXT* track to play           */
    int  seed;           /* random seed                             */
    int  amount;         /* number of tracks in the index           */
    bool in_ram;         /* True if the playlist is RAM-based       */
} playlist_info_t;

//extern playlist_info_t playlist;
extern bool playlist_shuffle;

void play_list(char *dir, char *file);
char* playlist_next(int steps);
void randomise_playlist( unsigned int seed );
void sort_playlist(void);
void empty_playlist(void);
void add_indices_to_playlist(void);
void playlist_clear(void);
int playlist_add(char *filename);

#endif /* __PLAYLIST_H__ */
