#define _LOGO_ <img src="rockbox400.png" align=center width=400 height=123>
#define _PAGE_ Open Source Jukebox Firmware
#define MAIN_PAGE
#include "head.t"

<h2>Purpose</h2>

<p>The purpose of this project is to write an Open Source replacement
firmware for the Archos Jukebox <i>5000</i>, <i>6000</i>, <i>Studio</i> and <i>Recorder</i> MP3 players.

<h2>News</h2>

<p><i>2002-06-30</i>: USB cable detection added.

<p><i>2002-06-27</i>: MP3 playback now works for Recorder 6000 and Recorder 10 too.

<p><i>2002-06-19</i>: Version 1.1 is released. <a href="download/">Download it here</a>.

<p><i>2002-06-19</i>: Recorder sound support checked into CVS. Good news: No background noise!

<p><i>2002-06-10</i>: Playlist and scroll support added. Testing for release v1.1.

<p><i>2002-06-07</i>: The ATA driver now works for the Recorder models too.

<p><i><small>(Old news items have moved to a 
<a href="history.html">separate page</a>.)</small></i>

<p>We have a mailing list: rockbox@cool.haxx.se.
To subscribe, send a message to <a href="mailto:majordomo@cool.haxx.se">majordomo@cool.haxx.se</a> with the words "subscribe rockbox" in the body.

<h2>Roadmap</h2>
<p>This is a rough indication of which features we plan/expect/hope to be
included in which versions. Naturally, this is all subject to change without
notice.

<dl>
<dt><b>Version 1.1</b>
<dd>Playlist support, scrolling, recorder support

<dt><b>Version 1.2</b>
<dd>UI improvements

<dt><b>Version 1.3</b>
<dd>Resume, persistent settings, autobuild playlists, UI improvements
</dl>

<h2>About the hardware</h2>

<p>I wrote a <a href="internals/bjorn.html">"dissection" page</a> some months ago,
showing the inside of the Archos and listing the main components.
I have also collected a couple of <a href="docs/">data sheets</a>.
Also, don't miss the <a href="notes.html">research notes</a>
from my reverse-engineering the firmware.

<h2>About the software</h2>

<p>The player has one version of the firmware burnt into flash ROM.
The first thing this version does after boot is to look for a file called
"archos.mod" in the root directory of the harddisk. 
If it exists, it is loaded into RAM and started.
This is how firmware upgrades are loaded.

<h2>Dreams</h2>
<p>Ok, forget about reality, what could we do with this?

<ul>
<li>All those simple mp3-play features we sometimes miss:
 <ul>
 <li>No pause between songs
 <li>Mid-song resume
 <li>Mid-playlist resume
 <li>No-scan playlists
 <li>Unlimited playlist size
 <li>Autobuild playlists (such as "all songs in this directory tree")
 <li>Auto-continue play in the next directory
 <li>Current folder and all sub-folder random play
 <li>Full disk random play
 <li>REAL random (if press back it goes to the previous song that was played)
 <li>Multi song queue (folder queue)
</ul>
<li>Faster scroll speed
<li>Archos Recorder support. Most of the hardware is the same, but the display and some other things differ.
<li>All kinds of cool features done from the wire remote control, including controlling your Archos from your car radio (req hw mod)
<li>Ogg Vorbis support [unverified: the MAS is somewhat programmable, but enough?]
<li>Support for megabass switch (req hw mod) [unverified: I just saw the DAC docs shows how to do it switchable. we need a free port pin to be able to switch]
<li>Player control via USB [unverified]
<li>Memory expansion? [doubtful: the current DRAM chip only has 10 address lines. we'd have to pull off one heck of a hw mod to expand that]
</ul>

#include "foot.t"
