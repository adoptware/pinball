Name: 		pinball
Version: 	0.0.2
Release: 	1

Group:          Amusements/Games
Group(cs): 	Zábava/Hry
Summary: 	Emilia Pinball is free OpenGL game of playing pinball.
Summary(cs): 	Emilia Pinball je volnì ¹iøitelná OpenGL hra.

Vendor:		Henrik Enqvist  (henquist@excite.com)
Packager:       Michal Ambroz (o_o) (rebus@seznam.cz)
License: 	GPL
URL: 		http://pinball.sourceforge.net
Source: 	http://prdownloads.sourceforge.net/pinball/%{name}-%{version}.tar.gz
BuildRoot: 	%{_tmppath}/%{name}-%{version}-root
Prefix: /usr
Prefix: /etc

%description
Pinball with Tux.


%prep
%setup

%build
./configure
make prefix=%{_prefix}


%install
mkdir -p %{buildroot}%{_prefix}/bin
make prefix=%{buildroot}%{_prefix} install

#Install application link for X-Windows
install -d %{buildroot}/etc/X11/applnk/Games
echo -e "[Desktop Entry]
Name=Emilia Pinball
Comment=OpenGL pinball game
Exec=pinball
Icon=pinball.xpm
Terminal=0
Type=Application" > %{buildroot}/etc/X11/applnk/Games/"Emilia Pinball".desktop




%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL NEWS README
%doc doc/*
/etc/X11/applnk/*
%{_bindir}/*
%{_prefix}/share/pinball/*



%changelog
* Fri Mar 08 2002 Michal Ambroz (O_O) <rebus@seznam.cz>
- initial pinball RPM
