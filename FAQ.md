## FAQ

**Q:** What hardware is supported
**A:**
* minimal 300 Mhz cpu.
* 450 Mhz PIII+ preferred
* Decent graphics card, voodoo3, tnt2, geforce, matrox g450
  (you do not need anything superfast).
Graphics card known to work: Voodoo3, Matrox G450, GeForce4.

**Q:** What minimal OS is supported ?

**A:** Compiles without problems on: RedHat 7.3, 8.0, 9.0, Debian 3.0.

**Q:** The 'boot' and 'tux' lights on the floor blinks or have vertical
lines?

**A:** Some buggy implementations of OpenGL does not implement the polygon
offset command - which is need by the game for correct rendering.

**Q:** Sounds are delayed with 0.5 seconds?

**A:** You're either using arts, esd or have buggy audio drivers. Stop
arts and esd, if that does not help install alsa drivers.

**Q:** The rpm doesn't fit my dist and the source won't compile?
Search the web for rpms, e.g. Mandrake rpms seems to pop up on
rpmfind a couple of weeks/months after my releases.
For compiling: double check that you the _development_ packages of OpenGL, X, SDL,
SDL_image, SDL_mixer are installed.

**Q:** Can I help?

**A:** Yes, I would like to check that the physics model of the game is
somewhat like a real pinball machine without putting all my money into
the pinball games at the local game corner. So if you have a spare
pinball machine send it to me (it don't think it will fit in the
mail though...).
