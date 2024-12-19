# RPM information extension for PHP

Retrieve RPM information from PHP code using librpm.

This extension can be considered as stable, and be used on production environement.


----

# Sources

* Official git repository: https://git.remirepo.net/cgit/tools/php-rpminfo.git/
* Mirror on github for contributors: https://github.com/remicollet/rpminfo

# Build

You need the rpm development files (rpm-devel) version >= 4.13.

From the sources tree

    $ phpize
    $ ./configure --enable-rpminfo
    $ make
    $ make test

From https://pecl.php.net/ using pecl command

    $ pecl install rpminfo

From https://packagist.org/ using PHP Installer for Extensions

    $ pie install remi/rpminfo


----

# Usage

## rpmvercmp

	int rpmvercmp(string evr1, string evr2);

Allow to compare 2 EVR (epoch:version-release) strings. The return value is < 0 if evr1 < evr2, > 0 if evr1 > evr2, 0 if equal.

    $ php -a
    php > var_dump(rpmvercmp("1.0", "1.1"));
    int(-1)
    php > var_dump(rpmvercmp("1:1.0", "1.1"));
    int(1)


## rpminfo

	rpminfo(string path [, bool full  [, string &error]]): array;

Retrieve information from a rpm file, reading its metadata.
If given `error` will be used to store error message instead of raising a warning.
The return value is a hash table, or false if it fails.

    $ php -a
    php > print_r(rpminfo("tests/bidon.rpm"));
    Array
    (
        [Name] => bidon
        [Version] => 1
        [Release] => 1.fc25.remi
        [Arch] => x86_64
    )
    
    php > print_r(rpminfo("tests/bidon.rpm", true));
    Array
    (
        [Headeri18ntable] => C
        [Sigsize] => 2304
        [Sigmd5] => 644819c3566819b1e10a5c97943de094
        [Sha1header] => 0a86742fe53973ac9ab4611187a83ffb44f1de5a
        [Sha256header] => 9aab7242a80212ad1fe4fdd3b250c0c4f176c0b3fb1355c0d62ff094fc3f7da0
        [Name] => bidon
        [Version] => 1
        [Release] => 1.fc25.remi
        [Summary] => Bidon
        [Description] => A dummy package
        ...
        [IsSource] => 
    )
    
    php > var_dump(rpminfo("missing.rpm"));
    Warning: rpminfo(): Can't open 'missing.rpm': No such file or directory in php shell code on line 1
    bool(false)
    
    php > var_dump(rpminfo("missing.rpm", false, $error));
    bool(false)
    php > echo $error;
    Can't open 'missing.rpm': No such file or directory

## rpmdbinfo

	rpmdbinfo(string path [, bool full ]): array;

Retrieve information from rpm database about an installed package.
The return value is an array of hash tables, or false if it fails.

    $ php -a
    php > print_r(rpmdbinfo("php"));
    Array
    (
        [0] => Array
            (
                [Name] => php
                [Version] => 7.3.5
                [Release] => 1.fc31.remi
                [Summary] => PHP scripting language for creating dynamic web sites
                [Arch] => x86_64
            )
    )

Retrieve information from rpm database about installed packages using glob or regex.
The return value is an array of hash tables, or false if it fails.

    $ php -a
    php > print_r(rpmdbsearch("php-pecl-r*", RPMTAG_NAME, RPMMIRE_GLOB));
    Array
    (
        [0] => Array
            (
                [Name] => php-pecl-radius
                [Version] => 1.4.0
                [Release] => 0.10.b1.fc31
                [Summary] => Radius client library
                [Arch] => x86_64
            )
        [1] => Array
            (
                [Name] => php-pecl-redis5
                [Version] => 5.2.0
                [Release] => 1.fc31.remi.7.3
                [Summary] => Extension for communicating with the Redis key-value store
                [Arch] => x86_64
            )
        [2] => Array
            (
                [Name] => php-pecl-rpminfo
                [Version] => 0.2.3
                [Release] => 1.fc31.remi.7.3
                [Summary] => RPM information
                [Arch] => x86_64
            )
    )

    $ php -a
    php > print_r(rpmdbsearch("^php-pecl-r", RPMTAG_NAME, RPMMIRE_REGEX));
    Array
    (
        [0] => Array
            (
                [Name] => php-pecl-radius
                [Version] => 1.4.0
                [Release] => 0.10.b1.fc31
                [Summary] => Radius client library
                [Arch] => x86_64
            )
        [1] => Array
            (
                [Name] => php-pecl-redis5
                [Version] => 5.2.0
                [Release] => 1.fc31.remi.7.3
                [Summary] => Extension for communicating with the Redis key-value store
                [Arch] => x86_64
            )
        [2] => Array
            (
                [Name] => php-pecl-rpminfo
                [Version] => 0.2.3
                [Release] => 1.fc31.remi.7.3
                [Summary] => RPM information
                [Arch] => x86_64
            )
    )

    $ php -a
    php > print_r(rpmdbsearch(PHP_BINARY, RPMTAG_INSTFILENAMES));
    Array
    (
        [0] => Array
            (
                [Name] => php-cli
                [Version] => 7.3.15
                [Release] => 1.fc31.remi
                [Summary] => Command-line interface for PHP
                [Arch] => x86_64
            )
    )

## rpmexpand

	rpmexpand($text): string

Retrieve expanded value of a RPM macro

    $ php -a
    php > var_dump(rpmexpand("%{?fedora:Fedora %{fedora}}%{?rhel:Enterprise Linux %{rhel}}"));
    string(9) "Fedora 41"

## rpmexpandnumeric

	rpmexpandnumeric($text): int

Retrieve numerical value of a RPM macro

    $ php -a
    php > var_dump(rpmexpandnumeric("%__isa_bits"));
    int(64)

## rpmdefine

	rpmdefine($text): bool

Define or change a RPM macro value.

For example, can be used to set the Database path and backend

    $ mock -r almalinux-8-x86_64 init
    ...
    $ mock -r fedora-41-x86_64 init
    ...
    $ php -a
    php > // use an old database (bdb) from an EL-8 chroot
    php > rpmdefine("_dbpath /var/lib/mock/almalinux-8-x86_64/root/var/lib/rpm");
    php > rpmdefine("_db_backend bdb_ro");
    php > var_dump(rpmdbinfo("almalinux-release")[0]["Summary"]);
    string(22) "AlmaLinux release file"
    php > // use a new database (sqlite) from a Fedora-41 chroot
    php > rpmdefine("_dbpath /var/lib/mock/fedora-41-x86_64/root/usr/lib/sysimage/rpm");
    php > rpmdefine("_db_backend sqlite");
    php > var_dump(rpmdbinfo("fedora-release")[0]["Summary"]);
    string(20) "Fedora release files"

----

# LICENSE

Author: Remi Collet

This extension is licensed under [The PHP License, version 3.01](http://www.php.net/license/3_01.txt)

-----

# History

Created as a PoC, for fun, see history on
https://blog.remirepo.net/post/2018/01/26/About-repomanage-performance-regression
