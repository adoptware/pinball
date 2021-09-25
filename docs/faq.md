# FAQ #

Why does the game run so sloooooow?

1. Buy a 3D card.

2. Check that you have configured your card right, 'glxgears' should run with at least 400 fps on low end machines (300Mhz Voodoo3). Make sure that you have the latest drivers installed. Read the manual for your card or search the web for help.

3. Turn off all other applications, yes linux is able of multitasking but some programs may spontaneously start to consume cpu time.

4. Turn off music. Your SDL version most likely uses timidity for midi sequencing. Timidity sounds great but uses quite a lot of resources. You may need to restart the game for the sound system to be properly shut down. Sound effects should, however, not affect the frame rate.

5. Use smaller screen size, especially with software rendering.

Will it run on my P166 with a S3 4Mb gfx card? No, you need a fast cpu and prefferable a 3D card. If you want you can try the software-rendering version which uses the Allegro library.

Is there a level editor?

Yes. Download the 'pinedit' package at sourceforge.

Why doesn't the rpms work on Mandrake?

Mandrake doesn't have libpng.so.2 installed, instead there is a libpng.so.3. To compile the source code should, however, not be any trouble. I noticed that some friendly soul has made some mandrake binaries, you can find them at rpmfind, search with the word pinball.

Why are the sounds delayed with 0.5 seconds?

You're either using arts, esd or have buggy audio drivers. Stop arts and esd, if that does not help install alsa drivers.

Why can't I hear any music even if I hear the sounds?

The game uses the default midi player of SDL_mixer, 'timidity', which is compiled into SDL_mixer. You can compile SDL_mixer yourself and tell it to use native midi or something. First check that your system is able to play midi files, use 'kmidi', 'timidity' or something. Then you have check that SDL_mixer can play midi files, the name of the program is 'playmus'.

Things that might fix your problems are: fiddle with the /etc/timidity.cfg file, use latest version of SDL_mixer, compile SDL_mixer yourself, use alsa drivers (I always recommend alsa drivers). I'll give more answers when I know more about what causes these troubles.

Compiling the editor complaines about moc_* files?

Your version of QT is different than in the package. Simply delete all moc_* files, they will be regenerated.
