--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
$a = rpmdbsearch('php*', RPMTAG_NAME , RPMMIRE_GLOB);
var_dump(count($a) > 1);

$a = rpmdbsearch('^php', RPMTAG_NAME, RPMMIRE_REGEX);
var_dump(count($a) > 1);

$a = rpmdbsearch(PHP_BINARY, RPMTAG_INSTFILENAMES);
var_dump(count($a) == 1);

?>
Done
--EXPECTF--
bool(true)
bool(true)
bool(true)
Done
