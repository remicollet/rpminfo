--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
$a = rpmdbinfo('php*', false, RPM_MATCH_GLOB);
var_dump(count($a) > 1);
$a = rpmdbinfo('^php', false, RPM_MATCH_REGEX);
var_dump(count($a) > 1);
?>
Done
--EXPECTF--
bool(true)
bool(true)
Done
