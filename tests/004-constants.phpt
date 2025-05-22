--TEST--
Check for RPMVERSION constant
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(RPMVERSION);
?>
Done
--EXPECTF--
string(%d) "%s"
Done
