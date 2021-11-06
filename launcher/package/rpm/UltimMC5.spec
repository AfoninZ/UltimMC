Name:           UltimMC5
Version:        1.4
Release:        3%{?dist}
Summary:        A local install wrapper for UltimMC

License:        ASL 2.0
URL:            https://github.com/AfoninZ/MultiMC5-Cracked
BuildArch:      x86_64

Requires:       zenity qt5-qtbase wget xrandr
Provides:       ultimmc UltimMC ultimmc5

%description
A local install wrapper for UltimMC

%prep


%build

%install
mkdir -p %{buildroot}/opt/ultimmc
install -m 0644 ../ubuntu/ultimmc/opt/ultimmc/icon.svg %{buildroot}/opt/ultimmc/icon.svg
install -m 0755 ../ubuntu/ultimmc/opt/ultimmc/run.sh %{buildroot}/opt/ultimmc/run.sh
mkdir -p %{buildroot}/%{_datadir}/applications
install -m 0644 ../ubuntu/ultimmc/usr/share/applications/ultimmc.desktop %{buildroot}/%{_datadir}/applications/ultimmc.desktop
mkdir -p %{buildroot}/%{_datadir}/metainfo
install -m 0644 ../ubuntu/ultimmc/usr/share/metainfo/ultimmc.metainfo.xml %{buildroot}/%{_datadir}/metainfo/ultimmc.metainfo.xml
mkdir -p %{buildroot}/%{_mandir}/man1
install -m 0644 ../ubuntu/ultimmc/usr/share/man/man1/ultimmc.1 %{buildroot}/%{_mandir}/man1/ultimmc.1

%files
%dir /opt/ultimmc
/opt/ultimmc/icon.svg
/opt/ultimmc/run.sh
%{_datadir}/applications/ultimmc.desktop
%{_datadir}/metainfo/ultimmc.metainfo.xml
%dir /usr/share/man/man1
%{_mandir}/man1/ultimmc.1.gz

%changelog
* Sun Oct 03 2021 imperatorstorm <30777770+ImperatorStorm@users.noreply.github.com>
- added manpage

* Tue Jun 01 2021 kb1000 <fedora@kb1000.de> - 1.4-2
- Add xrandr to the dependencies

* Tue Dec 08 00:34:35 CET 2020 joshua-stone <joshua.gage.stone@gmail.com>
- Add metainfo.xml for improving package metadata

* Wed Nov 25 22:53:59 CET 2020 kb1000 <fedora@kb1000.de>
- Initial version of the RPM package, based on the Ubuntu package
