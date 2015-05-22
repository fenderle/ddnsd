Summary:           Daemon to update dynamic DNS host entries
Name:              ddnsd
Version:           1.0.2
Release:           1%{?dist}
License:           MIT
Group:             System Environment/Daemons
URL:               http://www.anamica.de/ddnsd
Source0:           %{name}-%{version}.tar.bz2
BuildArch:         noarch
Requires:          perl(LWP::Simple)
BuildRoot:         %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:     perl(Pod::Perldoc::ToNroff)

%description
ddnsd is a dynamic dns updater which has been inspired by ddclient but
utilizes standard dynamic dns protocols for dns updates and thus does
not rely on more than a dynamic dns capable name server such as bind9.

%prep
%setup -q

%build
touch %{name}.8
chmod 666 %{name}.8
perldoc -o nroff -d %{name}.8 -w center:%{name} -w section:8 %{name}

%install
%{__rm} -rf %{buildroot}
%{__mkdir_p} %{buildroot}/%{_sbindir}/
%{__mkdir_p} %{buildroot}%{_unitdir}/
%{__mkdir_p} %{buildroot}/%{_sysconfdir}/sysconfig/
%{__mkdir_p} %{buildroot}/%{_sysconfdir}/%{name}/modules
%{__mkdir_p} %{buildroot}/%{_mandir}/man8
%{__install} -m 755 %{name} %{buildroot}/%{_sbindir}/%{name}
%{__install} -m 644 %{name}.8 %{buildroot}/%{_mandir}/man8/%{name}.8
%{__install} -m 600 %{name}.conf %{buildroot}/%{_sysconfdir}/%{name}/%{name}.conf
%{__install} -m 755 ipcheck.url %{buildroot}/%{_sysconfdir}/%{name}/modules/ipcheck.url
%{__install} -m 755 ipcheck.interface %{buildroot}/%{_sysconfdir}/%{name}/modules/ipcheck.interface
%{__install} -m 644 contrib/rhel7/ddnsd.sysconfig %{buildroot}/%{_sysconfdir}/sysconfig/%{name}
%{__mkdir_p} %{buildroot}/%{_sysconfdir}/NetworkManager/dispatcher.d
%{__install} -m 755 contrib/rhel7/ddnsd.NetworkManager %{buildroot}/%{_sysconfdir}/NetworkManager/dispatcher.d/50-%{name}
%{__mkdir_p} %{buildroot}/%{_localstatedir}/run/%{name}

%post
%systemd_post ddnsd.service

%preun
%systemd_preun ddnsd.service

%postun
%systemd_postun_with_restart ddnsd.service

%files
%{_sysconfdir}/NetworkManager/dispatcher.d/50-%{name}
%{_sysconfdir}/%{name}/modules/ipcheck.url
%{_sysconfdir}/%{name}/modules/ipcheck.interface
%config(noreplace) %{_sysconfdir}/%{name}/%{name}.conf
%config(noreplace) %{_sysconfdir}/sysconfig/%{name}
%{_sbindir}/%{name}
%{_mandir}/man8/%{name}.8.gz
%dir %{_localstatedir}/run/%{name}/

%changelog
* Fri May 22 2015 Frank Enderle <frank.enderle@anamica.de> 1.0.2-1
Initial release for RHEL7 and compatibles

