Name: 		pinball
Version: 	0.3.1
Release: 	1rh90

Group:    	Amusements/Games
Summary: 	  Emilia Pinball is free OpenGL pinball game.

Vendor:		  Henrik Enqvist  <henqvist@users.sourceforge.net>
Packager: 	Henrik Enqvist  <henqvist@users.sourceforge.net>
License: 	  GPL
URL: 		    http://pinball.sourceforge.net
Source: 	  http://prdownloads.sourceforge.net/pinball/%{name}-%{version}.tar.gz
Source1:    %{name}.desktop
BuildRoot: 	%{_tmppath}/%{name}-%{version}-root

%description
Pinball game.


%prep
rm -rf %{buildroot}

%setup -n %{name}-%{version}

%build
./configure --prefix=%{_prefix} --with-highscore-prefix=/var/lib/games/pinball
make


%install
make DESTDIR=%{buildroot} install

#Install application link for X-Windows
mkdir -p %{buildroot}%{_datadir}/applications
desktop-file-install --vendor=emilia                              \
  --dir %{buildroot}%{_datadir}/applications                      \
  --add-category X-Red-Hat-Extra                                  \
  --add-category Application                                      \
  --add-category Game                                             \
  %{SOURCE1}


%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc AUTHORS COPYING INSTALL NEWS README
%{_prefix}/bin/pinball
%{_prefix}/bin/pinball-config
%dir %{_prefix}/share/pinball
%{_prefix}/share/pinball/*
%dir %{_prefix}/lib/pinball
%{_prefix}/lib/pinball/*
%dir %{_prefix}/include/pinball
%{_prefix}/include/pinball/*
%{_prefix}/share/applications/*.desktop
%attr(0775, root, root) %dir /var/lib/games/pinball
%attr(0666, games, games) %dir /var/lib/games/pinball/professor/highscores
%attr(0666, games, games) %dir /var/lib/games/pinball/tux/highscores


%changelog
* Mon Apr 07 2003 Michal Ambroz (O_O) <rebus@seznam.cz>
- rebuild for Fedora Core 1
* Mon Apr 07 2003 Michal Ambroz (O_O) <rebus@seznam.cz>
- cleanup for RHL 9
* Wed Mar 05 2003 Henrik Enqvist
- removed build_root to make it work with libtool (ugly)
* Fri Mar 08 2002 Michal Ambroz (O_O) <rebus@seznam.cz>
- initial pinball RPM
