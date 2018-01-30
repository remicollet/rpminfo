# RPM information extension for PHP

Retrieve RPM information from PHP code using librpm.

**Notice**: this is a experimental extension, a work in progress, so don't expect a stable API yet.

----

# Build

You need the rpm development files (rpm-devel) version >= 4.11.3.

From the sources tree

    $ phpize
    $ ./configure --enable-rpminfo
    $ make
    $ make test

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

	array rpminfo(string path [, bool full ]);

Retrieve information from a rpm file, reading its metadata.
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


----

# LICENSE

Author: Remi Collet

This extension is licensed under [The PHP License, version 3.01](http://www.php.net/license/3_01.txt)

-----

# History

Created as a PoC, for fun, see history on
https://blog.remirepo.net/post/2018/01/26/About-repomanage-performance-regression
