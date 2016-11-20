Name:           pinball
Version:        0.3.3.1
Release:        1%{?dist}
Summary:        Emilia 3D Pinball Game
# core license is GPL+
# gnu table licenses are (GFDL or Free Art or CC-BY-SA) and GPLv3 and CC-BY-SA
# hurd table license is GPLv2+
License: GPL+ and (GFDL or Free Art or CC-BY-SA) and GPLv3 and CC-BY-SA and GPLv2+
URL:            http://pinball.sourceforge.net
Source0:        https://github.com/sergiomb2/pinball/archive/%{version}/%{name}-%{version}.tar.gz
BuildRequires:  libXt-devel freeglut-devel SDL_image-devel SDL_mixer-devel
BuildRequires:  libpng-devel libvorbis-devel libtool-ltdl-devel
BuildRequires:  desktop-file-utils libappstream-glib
BuildRequires:  autoconf automake libtool
Requires:       hicolor-icon-theme opengl-games-utils timidity++-patches

%description
The Emilia Pinball project is an open source pinball simulator for linux
and other unix systems. The current release features a number of tables:
tux, professor, professor2, gnu and hurd and is very addictive.

%package devel
Summary:    Development files for %{name}
Requires:   %{name}%{?_isa} = %{version}-%{release}

%description devel
This package contains files for development with %{name}.
May be used in pinball-pinedit.


%prep
%setup -q
sed -i 's/Exec=pinball/Exec=pinball-wrapper/' pinball.desktop
./bootstrap


%build
%configure --disable-static
%make_build


%install
%make_install INSTALL="install -p"
ln -s opengl-game-wrapper.sh $RPM_BUILD_ROOT%{_bindir}/%{name}-wrapper
# remove unused global higescorefiles:
#rm -r $RPM_BUILD_ROOT%{_localstatedir}
# remove unused test module
rm $RPM_BUILD_ROOT%{_libdir}/%{name}/libModuleTest.*
# .la files are needed for ltdl
rm $RPM_BUILD_ROOT%{_libdir}/%{name}/lib*.a
# remove bogus development files
#rm $RPM_BUILD_ROOT%{_bindir}/%{name}-config
#rm -r $RPM_BUILD_ROOT%{_includedir}/%{name}

# below is the desktop file and icon stuff.
mkdir -p $RPM_BUILD_ROOT%{_datadir}/applications
desktop-file-install --dir $RPM_BUILD_ROOT%{_datadir}/applications \
  --set-key='Keywords' --set-value='Game;Arcade;Pinball;' \
  pinball.desktop
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/48x48/apps
install -p -m 644 pinball.png \
  $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/48x48/apps
mkdir -p $RPM_BUILD_ROOT%{_datadir}/appdata
install -p -m 644 pinball.appdata.xml $RPM_BUILD_ROOT%{_datadir}/appdata
appstream-util validate-relax --nonet \
  $RPM_BUILD_ROOT%{_datadir}/appdata/%{name}.appdata.xml


%post
touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

%postun
if [ $1 -eq 0 ] ; then
    touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :


%files
%doc README ChangeLog
%license COPYING
%{_bindir}/%{name}
%{_bindir}/%{name}-wrapper
%dir %{_libdir}/%{name}
%{_libdir}/%{name}/*so.*
%{_libdir}/%{name}/*la
%{_datadir}/%{name}
%{_datadir}/appdata/%{name}.appdata.xml
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/48x48/apps/%{name}.png

%files devel
%{_bindir}/%{name}-config
%{_libdir}/%{name}/*.so
%{_includedir}/%{name}


%changelog
* Sat Nov 12 2016 Sérgio Basto <sergio@serjux.com> - 0.3.3.1-1
- Test pinball-0.3.3.1

* Tue Jul 26 2016 Sérgio Basto <sergio@serjux.com> - 0.3.3-1
- Update to 0.3.3

* Thu Feb 04 2016 Fedora Release Engineering <releng@fedoraproject.org> - 0.3.2-4
- Rebuilt for https://fedoraproject.org/wiki/Fedora_24_Mass_Rebuild

* Sat Jan  9 2016 Hans de Goede <hdegoede@redhat.com> - 0.3.2-3
- Add Keywords field to desktop file

* Mon Dec 14 2015 Jon Ciesla <limburgher@gmail.com> - 0.3.2-2
- Fix license tag, BZ 1290935.

* Mon Oct 26 2015 Hans de Goede <hdegoede@redhat.com> - 0.3.2-1
- Switch to new github upstream
- Update to 0.3.2 release
- Add an appdata file
- Add Requires: timidity++-patches so that the music works

* Fri Aug 21 2015 Ralf Corsépius <corsepiu@fedoraproject.org> - 0.3.1-27
- Let configure honor CFLAGS (Add pinball-0.3.1-cflags.patch)
  (Fix F23FTBS, RHBZ#1239792).
- Add %%license.
- Modernize spec.

* Thu Jun 18 2015 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-26
- Rebuilt for https://fedoraproject.org/wiki/Fedora_23_Mass_Rebuild

* Sun Aug 17 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-25
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_22_Mass_Rebuild

* Tue Jul 01 2014 Yaakov Selkowitz <yselkowi@redhat.com> - 0.3.1-24
- Run autoreconf to fix FTBFS on aarch64 (#926341)

* Sat Jun 07 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-23
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_Mass_Rebuild

* Sun Aug 04 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-22
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

* Mon Feb 11 2013 Jon Ciesla <limburgher@gmail.com> - 0.3.1-21
- Drop desktop vendor tag.

* Sat Jul 21 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-20
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Tue Feb 28 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-19
- Rebuilt for c++ ABI breakage

* Sat Jan 14 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-18
- Rebuilt for https://fedoraproject.org/wiki/Fedora_17_Mass_Rebuild

* Wed Feb 09 2011 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-17
- Rebuilt for https://fedoraproject.org/wiki/Fedora_15_Mass_Rebuild

* Mon Nov 15 2010 Jon Ciesla <limb@jcomserv.net> - 0.3.1-16
- Fix FTBFS, BZ 631379.

* Sun Jul 26 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-15
- Rebuilt for https://fedoraproject.org/wiki/Fedora_12_Mass_Rebuild

* Mon Mar 02 2009 Jon Ciesla <limb@jcomserv.net> - 0.3.1-14
- Patch for strict prototypes.

* Thu Feb 26 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-13
- Rebuilt for https://fedoraproject.org/wiki/Fedora_11_Mass_Rebuild

* Mon Nov 24 2008 Jon Ciesla <limb@jcomserv.net> - 0.3.1-12
- Cleaned up summary.

* Tue Feb 19 2008 Fedora Release Engineering <rel-eng@fedoraproject.org> - 0.3.1-11
- Autorebuild for GCC 4.3

* Sun Oct 21 2007 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-10
- Drop the bogus -devel package (also fixing the multilib conficts caused by
  it, bz 342881)

* Mon Sep 24 2007 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-9
- Use opengl-games-utils wrapper to show error dialog when DRI is missing

* Wed Aug 15 2007 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-8
- Update License tag for new Licensing Guidelines compliance

* Sat Mar 10 2007 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-7
- Fixup .desktop file categories for games-menus usage

* Mon Aug 28 2006 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-6
- FE6 Rebuild

* Thu Aug 10 2006 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-5
- Add missing rm -rf $RPM_BUILD_ROOT to %%install

* Fri Aug  4 2006 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-4
- Make building use / honor $RPM_OPT_FLAGS
- Add missing BR: libtool-ltdl-devel
- Remove %%{?_smp_mflags} as that breaks building when actually set

* Thu Aug  3 2006 Hans de Goede <j.w.r.degoede@hhs.nl> 0.3.1-3
- Cleaned up specfile for Fedora Extras submission, based on Che's newrpms spec
- Use system ltdl

* Sat Apr 05 2003 che
- upgrade to version 0.2.0a

* Mon Mar 03 2003 Che
- upgrade to version 0.1.3

* Mon Nov 04 2002 Che
- upgrade to version 0.1.1

* Wed Oct 30 2002 Che
- initial rpm release
