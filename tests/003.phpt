--TEST--
Check for rpminfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpminfo(__DIR__ . "/bidon.rpm"));
var_dump(rpminfo(__DIR__ . "/bidon.rpm", true));
var_dump(rpminfo(__DIR__ . "/bidon-src.rpm", true));
// Errors
var_dump(rpminfo(__DIR__ . "/missing.rpm"));
var_dump(rpminfo(__FILE__));
?>
Done
--EXPECTF--
array(4) {
  ["Name"]=>
  string(5) "bidon"
  ["Version"]=>
  string(1) "1"
  ["Release"]=>
  string(11) "1.fc25.remi"
  ["Arch"]=>
  string(6) "x86_64"
}
array(63) {
  ["Headeri18ntable"]=>
  string(1) "C"
  ["Sigsize"]=>
  int(2304)
  ["Sigmd5"]=>
  string(32) "644819c3566819b1e10a5c97943de094"
  ["Sha1header"]=>
  string(40) "0a86742fe53973ac9ab4611187a83ffb44f1de5a"
  ["Sha256header"]=>
  string(64) "9aab7242a80212ad1fe4fdd3b250c0c4f176c0b3fb1355c0d62ff094fc3f7da0"
  ["Name"]=>
  string(5) "bidon"
  ["Version"]=>
  string(1) "1"
  ["Release"]=>
  string(11) "1.fc25.remi"
  ["Summary"]=>
  string(5) "Bidon"
  ["Description"]=>
  string(15) "A dummy package"
  ["Buildtime"]=>
  int(1516882146)
  ["Buildhost"]=>
  string(20) "builder.remirepo.net"
  ["Size"]=>
  int(29)
  ["Vendor"]=>
  string(11) "Remi Collet"
  ["License"]=>
  string(13) "Public Domain"
  ["Packager"]=>
  string(36) "Remi Collet <remi@fedoraproject.org>"
  ["Group"]=>
  string(11) "Unspecified"
  ["Url"]=>
  string(30) "http://blog.famillecollet.com/"
  ["Os"]=>
  string(5) "linux"
  ["Arch"]=>
  string(6) "x86_64"
  ["Filesizes"]=>
  int(0)
  ["Filemodes"]=>
  int(0)
  ["Filerdevs"]=>
  int(0)
  ["Filemtimes"]=>
  int(0)
  ["Filedigests"]=>
  NULL
  ["Filelinktos"]=>
  NULL
  ["Fileflags"]=>
  int(0)
  ["Fileusername"]=>
  NULL
  ["Filegroupname"]=>
  NULL
  ["Sourcerpm"]=>
  string(27) "bidon-1-1.fc25.remi.src.rpm"
  ["Fileverifyflags"]=>
  int(0)
  ["Archivesize"]=>
  int(428)
  ["Providename"]=>
  NULL
  ["Requireflags"]=>
  int(0)
  ["Requirename"]=>
  NULL
  ["Requireversion"]=>
  NULL
  ["Rpmversion"]=>
  string(6) "4.14.0"
  ["Changelogtime"]=>
  int(1419422400)
  ["Changelogname"]=>
  string(42) "Remi Collet <remi@fedoraproject.org> - 1-1"
  ["Changelogtext"]=>
  string(8) "- create"
  ["Cookie"]=>
  string(31) "builder.remirepo.net 1516882146"
  ["Filedevices"]=>
  int(0)
  ["Fileinodes"]=>
  int(0)
  ["Filelangs"]=>
  NULL
  ["Provideflags"]=>
  int(0)
  ["Provideversion"]=>
  NULL
  ["Dirindexes"]=>
  int(0)
  ["Basenames"]=>
  NULL
  ["Dirnames"]=>
  NULL
  ["Optflags"]=>
  string(219) "-O2 -g -pipe -Wall -Werror=format-security -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches -specs=/usr/lib/rpm/redhat/redhat-hardened-cc1 -m64 -mtune=generic"
  ["Payloadformat"]=>
  string(4) "cpio"
  ["Payloadcompressor"]=>
  string(2) "xz"
  ["Payloadflags"]=>
  string(1) "2"
  ["Platform"]=>
  string(23) "x86_64-redhat-linux-gnu"
  ["Filecolors"]=>
  int(0)
  ["Fileclass"]=>
  int(0)
  ["Classdict"]=>
  NULL
  ["Sourcepkgid"]=>
  string(32) "188da2a3966f4a5f0dd48e784be76846"
  ["Filedigestalgo"]=>
  int(8)
  ["Encoding"]=>
  string(5) "utf-8"
  ["Payloaddigest"]=>
  string(64) "ace77d50077cb8088d9bf224c9a9e89343a2aa40fe596b3e60ef10a9a200a3bd"
  ["Payloaddigestalgo"]=>
  int(8)
  ["IsSource"]=>
  bool(false)
}
array(54) {
  ["Headeri18ntable"]=>
  string(1) "C"
  ["Sigsize"]=>
  int(1753)
  ["Sigmd5"]=>
  string(32) "188da2a3966f4a5f0dd48e784be76846"
  ["Sha1header"]=>
  string(40) "994275fb4366d82043c791c50682cbe46e1c96d6"
  ["Sha256header"]=>
  string(64) "4c2f1cba929cc05ce58d4a9184d4652f2f7d7bdf05ba1dc92966ce9e9cefe93c"
  ["Name"]=>
  string(5) "bidon"
  ["Version"]=>
  string(1) "1"
  ["Release"]=>
  string(11) "1.fc25.remi"
  ["Summary"]=>
  string(5) "Bidon"
  ["Description"]=>
  string(15) "A dummy package"
  ["Buildtime"]=>
  int(1516882146)
  ["Buildhost"]=>
  string(20) "builder.remirepo.net"
  ["Size"]=>
  int(360)
  ["Vendor"]=>
  string(11) "Remi Collet"
  ["License"]=>
  string(13) "Public Domain"
  ["Packager"]=>
  string(36) "Remi Collet <remi@fedoraproject.org>"
  ["Group"]=>
  string(11) "Unspecified"
  ["Url"]=>
  string(30) "http://blog.famillecollet.com/"
  ["Os"]=>
  string(5) "linux"
  ["Arch"]=>
  string(6) "x86_64"
  ["Filesizes"]=>
  int(360)
  ["Filemodes"]=>
  int(33188)
  ["Filerdevs"]=>
  int(0)
  ["Filemtimes"]=>
  int(1516882140)
  ["Filedigests"]=>
  string(64) "195d7dd3ca9518024a1554e68b3f63fa7e2bdaa4efac59f06c1ab231283e6067"
  ["Filelinktos"]=>
  string(0) ""
  ["Fileflags"]=>
  int(32)
  ["Fileusername"]=>
  string(6) "extras"
  ["Filegroupname"]=>
  string(4) "remi"
  ["Fileverifyflags"]=>
  int(4294967295)
  ["Archivesize"]=>
  int(608)
  ["Requireflags"]=>
  int(0)
  ["Requirename"]=>
  NULL
  ["Requireversion"]=>
  NULL
  ["Rpmversion"]=>
  string(6) "4.14.0"
  ["Changelogtime"]=>
  int(1419422400)
  ["Changelogname"]=>
  string(42) "Remi Collet <remi@fedoraproject.org> - 1-1"
  ["Changelogtext"]=>
  string(8) "- create"
  ["Cookie"]=>
  string(31) "builder.remirepo.net 1516882146"
  ["Filedevices"]=>
  int(1)
  ["Fileinodes"]=>
  int(1)
  ["Filelangs"]=>
  string(0) ""
  ["Sourcepackage"]=>
  int(1)
  ["Dirindexes"]=>
  int(0)
  ["Basenames"]=>
  string(10) "bidon.spec"
  ["Dirnames"]=>
  string(0) ""
  ["Payloadformat"]=>
  string(4) "cpio"
  ["Payloadcompressor"]=>
  string(4) "gzip"
  ["Payloadflags"]=>
  string(1) "9"
  ["Filedigestalgo"]=>
  int(8)
  ["Encoding"]=>
  string(5) "utf-8"
  ["Payloaddigest"]=>
  string(64) "b104f6e80a0b761ca05b0c478c5a5e3f5fe57cf079cfca53d360351806c23951"
  ["Payloaddigestalgo"]=>
  int(8)
  ["IsSource"]=>
  bool(true)
}

Warning: rpminfo(): Can't open '%s/tests/missing.rpm': No such file or directory in %s/003.php on line 6
bool(false)

Warning: rpminfo(): Can't read '%s/tests/003.php': Argument is not a RPM file in %s/003.php on line 7
bool(false)
Done
