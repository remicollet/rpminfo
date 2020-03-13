--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
echo "Name / glob\n";
$a = rpmdbsearch('php*', RPMTAG_NAME , RPMMIRE_GLOB);
var_dump(count($a) > 1);

echo "Name / regex\n";
$a = rpmdbsearch('^php', RPMTAG_NAME, RPMMIRE_REGEX);
var_dump(count($a) > 1);

echo "Installed file\n";
$a = rpmdbsearch(PHP_BINARY, RPMTAG_INSTFILENAMES);
var_dump(count($a) == 1);

$phprpm = $a[0]['Name'];
$p = rpmdbinfo($phprpm, 1);

echo "Pkgid\n";
$a = rpmdbsearch($p[0]['Sigmd5'], RPMTAG_PKGID);
var_dump($a[0]['Name'] == $phprpm);

echo "Hdrid\n";
$a = rpmdbsearch($p[0]['Sha1header'], RPMTAG_HDRID);
var_dump($a[0]['Name'] == $phprpm);
var_dump(count($a[0]) < 10);

echo "Hdrid (full)\n";
$a = rpmdbsearch($p[0]['Sha1header'], RPMTAG_HDRID, -1, true);
var_dump($a[0]['Name'] == $phprpm);
var_dump($a[0]['Sha1header'] == $p[0]['Sha1header']);
var_dump(count($a[0]) > 20);

echo "Installtid\n";
$a = rpmdbsearch($p[0]['Installtid'], RPMTAG_INSTALLTID);
var_dump(count($a) >= 1);

echo "Version\n";
$a = rpmdbsearch($p[0]['Version'], RPMTAG_VERSION);
var_dump(count($a) > 1);

?>
Done
--EXPECTF--
Name / glob
bool(true)
Name / regex
bool(true)
Installed file
bool(true)
Pkgid
bool(true)
Hdrid
bool(true)
bool(true)
Hdrid (full)
bool(true)
bool(true)
bool(true)
Installtid
bool(true)
Version
bool(true)
Done
