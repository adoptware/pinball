Name: 		pinball
Version: 	0.1.3
Release: 	1rh73

Group:    Amusements/Games
Summary: 	Emilia Pinball is free OpenGL pinball game.

Vendor:		Henrik Enqvist  (henqvist@users.sourceforge.net)
Packager: Henrik Enqvist  (henqvist@users.sourceforge.net)
License: 	GPL
URL: 		  http://pinball.sourceforge.net
Source: 	http://prdownloads.sourceforge.net/pinball/%{name}-%{version}.tar.gz
#BuildRoot: 	%{_tmppath}/%{name}-%{version}-root

%description
Pinball game.


%prep
%setup

%build
#./configure --prefix=%{buildroot}%{_prefix}
./configure --prefix=%{_prefix}
#mkdir -p %{buildroot}%{_prefix}/bin
#make prefix=%{buildroot}%{_prefix}
make


%install
#make prefix=%{buildroot}%{_prefix} install
#some sort of hack
#cp %{_prefix}/bin/pinball %{buildroot}%{_prefix}/bin/.
#cp %{_prefix}/bin/pinball-config %{buildroot}%{_prefix}/bin/.
make install

#Install application link for X-Windows
install -d /etc/X11/applnk/Games
echo -e "[Desktop Entry]
Name=Emilia Pinball
Comment=OpenGL pinball game
Exec=pinball
Icon=pinball.xpm
Terminal=0
Type=Application" > /etc/X11/applnk/Games/EmiliaPinball.desktop


%clean
make uninstall
#rm -rf %{buildroot}
rm /etc/X11/applnk/Games/EmiliaPinball.desktop

%files
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL NEWS README
/etc/X11/applnk/Games/EmiliaPinball.desktop
%{_prefix}/bin/pinball
%{_prefix}/bin/pinball-config
%{_prefix}/share/pinball/*
%{_prefix}/lib/pinball/*
%{_prefix}/include/pinball/*



%changelog
* Wed Mar 05 2003 Henrik Enqvist
- removed build_root to make it work with libtool (ugly)
* Fri Mar 08 2002 Michal Ambroz (O_O) <rebus@seznam.cz>
- initial pinball RPM
