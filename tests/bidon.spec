%{!?ver:  %global ver 1}

Name:		bidon
Version:	%{ver}
Release:	3
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
cd %{buildroot}%{_sysconfdir}
ln foo.conf bar.conf
ln -s foo.conf toto.conf

%files
%doc README
%config(noreplace) %{_sysconfdir}/*.conf


%changelog
* Thu Oct 19 2023 Remi Collet <remi@fedoraproject.org> - 1-3
- add symlinks

* Fri Oct 13 2023 Remi Collet <remi@fedoraproject.org> - 1-2
- add some hardlinks

* Wed Dec 24 2014 Remi Collet <remi@fedoraproject.org> - 1-1
- create
