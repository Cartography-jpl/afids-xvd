Summary: This is the AFIDS version of XVD
Name: afids-xvd
Version: 1.05
Release: 1.el%{rhel}
License: Copyright 2018 California Institute of Technology ALL RIGHTS RESERVED
Group: Applications/Engineering
Vendor: California Institute of Technology
URL: http://www-mipl.jpl.nasa.gov/cartlab/cartlab.html
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: vicar-rtl vicar openmotif-devel libXpm-devel libX11-devel
# We require VICAR, which rpmbuild doesn't automatically pick up
Requires: vicar
Prefix: /opt/afids

%description

This is the AFIDS version of the XVD program. We have this separated out
from the rest of AFIDS because it is much more involved to build, and also
is the only code depending the VICAR GUI and Motiff libraries. 

%prep
%setup -q

%build
./configure --prefix=/opt/afids
make %_smp_mflags 

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc
/opt/afids/afids/vdev/xvd
/opt/afids/etc/afids/setup_afids_xvd.sh
/opt/afids/etc/afids/setup_afids_xvd.csh
/opt/afids/afids/vdev/xvd.pdf
/opt/afids/share/afids/xresource/XVd_blink.xres 
/opt/afids/share/afids/xresource/xvd.dict
/opt/afids/share/afids/xresource/xvd.pdf
/opt/afids/share/afids/xresource/xvd_save_script  
/opt/afids/share/afids/xresource/XVd.xres
/opt/afids/vicar/data/gui/ps1.ibis-2
/opt/afids/vicar/data/gui/ps2.ibis-2
/opt/afids/vicar/data/gui/ps3.ibis-2
/opt/afids/vicar/data/gui/ps4.ibis-2
/opt/afids/vicar/data/gui/ps5.ibis-2
/opt/afids/vicar/data/gui/ps6.ibis-2
/opt/afids/vicar/data/gui/ps7.ibis-2
/opt/afids/vicar/data/gui/ps_bw.ibis-2
/opt/afids/bin/xvd

%changelog
* Fri Jun 29 2018 Smyth <smyth@macsmyth> - 1.05-1.el%{rhel}
- Numerous changes to compile cleanly with gcc 7.2. No functional
  changes, but we modified enough code that we'll want to treat this
  as a new version.

* Sat Oct  8 2016 Mike M Smyth <smyth@pistol> - 1.04-4.el%{rhel}
- Rebuild with newer version of vicar

* Mon Feb 29 2016 Mike M Smyth <smyth@pistol> - 1.04-3.el%{rhel}
- Rebuild with newer version of vicar.

* Fri Dec 18 2015 Mike M Smyth <smyth@pistol> - 1.04-2
- Rebuild

* Wed Nov 25 2015 Mike M Smyth <smyth@pistol> - 1.04-1
- Minor changes in setup file

* Wed Nov 12 2014 Mike M Smyth <smyth@pistol> - 1.03-1
- Update setup_afids_xvd. No functional changes.

* Tue Jun 25 2013 Mike M Smyth <smyth@pistol> - 1.02-1
- Add new setup_afids_xvd.csh file that properly sets up V2DATA.

* Thu Jun  6 2013 Mike M Smyth <smyth@pistol> - 1.01-1
- Various updates for building on the Mac, no functional changes

* Fri Nov 30 2012 Mike M Smyth <smyth@pistol> - xvd-1
- Initial build.

