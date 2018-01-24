rpminfo
=======

Experimental wrapper for librpm

For now, only expose

	int rpmvercmp(string evr1, string evr2);


Mostly a PoC build for fun because of

https://bugzilla.redhat.com/1537981
performance issue: dnf repomanage is really slow


Some benchmark results (find 15 old RPMs among 5000)

# DNF on Fedora

    $ time dnf repomanage --old --keep 5 .
    ...
    real	0m15,971s

# YUM on RHEL / CentOS

    $ time repomanage --old --keep 5 .
    ...
    real	0m5.519s

# PHP script

    $ time php repomanage.php --old --keep 5 .
    ...
    real	0m0,634s

