--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
$a = rpmdbsearch('php*', RPM_TAG_NAME , RPM_MATCH_GLOB);
var_dump(count($a) > 1);

$a = rpmdbsearch('^php', RPM_TAG_NAME, RPM_MATCH_REGEX);
var_dump(count($a) > 1);

$a = rpmdbsearch(PHP_BINARY, RPM_TAG_INSTFILENAMES);
var_dump(count($a) == 1);

?>
Done
--EXPECTF--
bool(true)
bool(true)
bool(true)
Done
