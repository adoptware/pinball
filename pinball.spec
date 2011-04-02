Name:           pinball
Version:        0.0.0
Release:        0%{?dist}
Summary:        Emilia arcade game
Group:          Amusements/Games
License:        GPL+
URL:            http://pinball.sourceforge.net
Source0:        http://downloads.sourceforge.net/pinball/%{name}-%{version}.tar.gz
Source1:        %{name}.desktop
Source2:        %{name}.png
#Patch0:         pinball-0.3.1-sys-ltdl.patch
#Patch1:         pinball-0.3.1-hiscore.patch
#Patch2:		pinball-0.3.1-strictproto.patch
#Patch3:		pinball-0.3.1-lacomment.patch
#Patch4:		pinball-0.3.1-cstddef.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:  libXt-devel SDL_image-devel SDL_mixer-devel
BuildRequires:  libpng-devel libvorbis-devel libtool-ltdl-devel
BuildRequires:  desktop-file-utils
Requires:       hicolor-icon-theme opengl-games-utils

%description
The Emilia Pinball project is an open source pinball simulator for linux
and other unix systems. The current release is a stable and mature alpha.
There is only one level to play with but it is however very addictive.


%prep
%setup -q
#%patch0 -p1 -z .sys-ltdl
#%patch1 -p1 -z .hiscore
#%patch2 -p0
#%patch3 -p0
#%patch4 -p0
[ -e configure ] || ./bootstrap
rm -fr libltdl
# sigh stop autoxxx from rerunning because of our patches above.
touch aclocal.m4
touch configure
touch pinconfig.h.in `find -name Makefile.in`

# cleanup a bit
chmod -x ChangeLog */*.h */*.cpp data/*/Module*.cpp


%build
%configure
make CXXFLAGS="$RPM_OPT_FLAGS"


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT INSTALL="%{__install} -p" install
ln -s opengl-game-wrapper.sh $RPM_BUILD_ROOT%{_bindir}/%{name}-wrapper
# remove unused global higescorefiles:
rm -fr $RPM_BUILD_ROOT%{_localstatedir}
# remove unused test module
rm $RPM_BUILD_ROOT%{_libdir}/%{name}/libModuleTest.*
# .la files are needed for ltdl
rm $RPM_BUILD_ROOT%{_libdir}/%{name}/lib*.{a,so}
# remove bogus development files
rm $RPM_BUILD_ROOT%{_bindir}/%{name}-config
rm -r $RPM_BUILD_ROOT%{_includedir}/%{name}

# below is the desktop file and icon stuff.
mkdir -p $RPM_BUILD_ROOT%{_datadir}/applications
desktop-file-install --vendor fedora            \
  --dir $RPM_BUILD_ROOT%{_datadir}/applications \
  %{SOURCE1}
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/48x48/apps
install -p -m 644 %{SOURCE2} \
  $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/48x48/apps


%clean
rm -rf $RPM_BUILD_ROOT


%post
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
   %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi

%postun
touch --no-create %{_datadir}/icons/hicolor || :
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
   %{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :
fi


%files
%defattr(-,root,root,-)
%doc README ChangeLog
%{_bindir}/%{name}*
%{_libdir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/fedora-%{name}.desktop
%{_datadir}/icons/hicolor/48x48/apps/%{name}.png


%changelog
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
