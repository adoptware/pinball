Name: 		pinball
Version: 	0.3.0
Release: 	1rh80

Group:    	Amusements/Games
Summary: 	Emilia Pinball is free OpenGL pinball game.

Vendor:		Henrik Enqvist  (henqvist@users.sourceforge.net)
Packager: 	Henrik Enqvist  (henqvist@users.sourceforge.net)
License: 	GPL
URL: 		http://pinball.sourceforge.net
Source: 	http://prdownloads.sourceforge.net/pinball/%{name}-%{version}.tar.gz
BuildRoot: 	%{_tmppath}/%{name}-%{version}-root

%description
Pinball game.

%prep
# rm -rf %{buildroot}
%setup -q

%build
./configure --build=%{_target_platform} --prefix=%{_prefix} \
            --exec-prefix=%{_exec_prefix} --bindir=%{_bindir} \
            --sbindir=%{_sbindir} --sysconfdir=%{_sysconfdir} \
            --datadir=%{_datadir} --includedir=%{_includedir} \
            --libdir=%{_libdir} --libexecdir=%{_libexecdir} \
            --localstatedir=%{_localstatedir} \
            --sharedstatedir=%{_sharedstatedir} --mandir=%{_mandir} \
            --infodir=%{_infodir} --with-buildroot-prefix=%{buildroot} \
            $BUILD_ARGS
make


%install
make DESTDIR=%{?buildroot:%{buildroot}} LIBRARY_PATH=%{?buildroot:%{buildroot}}%{_libdir} install

#Install application link for X-Windows
#install -d /etc/X11/applnk/Games
#echo -e "[Desktop Entry]
#Name=Emilia Pinball
#Comment=OpenGL pinball game
#Exec=pinball
#Icon=%{_prefix}/share/pinball/pinball.xpm
#Terminal=0
#Type=Application" > /etc/X11/applnk/Games/EmiliaPinball.desktop


%clean
make uninstall
#rm -rf %{buildroot}
#rm /etc/X11/applnk/Games/EmiliaPinball.desktop

%files
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL NEWS README
#/etc/X11/applnk/Games/EmiliaPinball.desktop
%{_bindir}/pinball
%{_bindir}/pinball-config
%{_datadir}/pinball/*
%{_libdir}/pinball/*
%{_includedir}/pinball/*
/var/games/pinball/tux/highscores
/var/games/pinball/professor/highscores

%changelog
* Wed May 28 2003 Henrik Enqvist
- put buildroot back
- again some fixes to make things work, copied some stuff from xine-lib.spec
* Wed Mar 05 2003 Henrik Enqvist
- removed build_root to make it work with libtool (ugly)
* Fri Mar 08 2002 Michal Ambroz (O_O) <rebus@seznam.cz>
- initial pinball RPM
