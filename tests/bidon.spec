%{!?ver:  %global ver 1}

Name:		bidon
Version:	%{ver}
Release:	2%{?dist}
Summary:	Bidon
License:	Public Domain
URL:		https://rpms.remirepo.net/

Obsoletes:  fooobs < 2


%description
A dummy package

%prep
date >README
echo "content" >conf

%build
: nothing to build

%install
install -Dpm644 conf %{buildroot}%{_sysconfdir}/foo.conf
ln %{buildroot}%{_sysconfdir}/foo.conf %{buildroot}%{_sysconfdir}/bar.conf

%files
%doc README
%config(noreplace) %{_sysconfdir}/*.conf


%changelog
* Fri Oct 13 2023 Remi Collet <remi@fedoraproject.org> - 1-2
- add some hardlinks

* Wed Dec 24 2014 Remi Collet <remi@fedoraproject.org> - 1-1
- create
