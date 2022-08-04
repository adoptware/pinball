## Installing and building

It is best to remove any previous version of pinball before installing a new
version. If you previously installed from rpm-package, write ```sudo rpm -e pinball```.
If you previously installed from source code, go to the pinball source directory 
and write ```sudo make uninstall```.

_NOTE:_ A user and group named 'games' must exist before installation. This
user and group exist by default on most systems. Else:
```sudo useradd 'games'```

After checking that you have the necessary libraries (libsdl, libsdl-image, 
libsdl-mixer) installed, you are ready to install. 

If you're installing from an .rpm package, write ```sudo rpm -i pinball-x.x.x.rpm```.

If you're compiling from source, check that you also have development versions 
of the libraries installed. They usually have a name like sdl-devel.x.x.x.rpm 
or libsdl-dev. 

Then install with the usual:

~~~
libtoolize
aclocal
autoheader
autoconf
automake --add-missing
./configure
sudo make
sudo make install
sudo make clean
~~~

The game is started with ```pinball```.
