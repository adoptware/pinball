## Installing and building

It is best to remove any previous version of pinball before installing a new
version. If you have used a rpm-package write as root user 'rpm -e pinball'.
If you have installed from source code goto the pinball source directory and
write as root user 'make uninstall'.

_NOTE:_ A user and group named 'games' must exist before installation, this
user and group exist by defualt on most systems else:
useradd 'games' (as root user)

After checking that you have the necessary libraries (SDL, SDL_IMAGE and
SDL_MIXER) installed you are ready to install. If you have an rpm-package
you write as root user 'rpm -i pinball-x.x.x.rpm'.

If you compile from a source package check that you also have the have the
development libraries installed. They usually have a name like
sdl-devel.x.x.x.rpm. Then install with the usual:

Did you remember to install the development packages of SDL? Did you remember
to install also SDL_Image and SDL_Mixer?

configure
make
make install (as root user)
make clean (if you want to save some disc space)

The game is started with 'pinball'.
