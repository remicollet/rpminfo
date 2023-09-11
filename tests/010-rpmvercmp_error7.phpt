--TEST--
Check for rpmvercmp function error
--SKIPIF--
<?php
if (!extension_loaded("rpminfo")) print "skip";
if (PHP_VERSION_ID >= 80000) print "skip only for PHP 7";
?>
--FILE--
<?php
var_dump(rpmvercmp());
var_dump(rpmvercmp("a"));
var_dump(rpmvercmp("a", "b", "c"));
var_dump(rpmvercmp("a", "b", "c", "d"));
?>
Done
--EXPECTF--

Warning: rpmvercmp() expects at least 2 parameters, 0 given in %s
NULL

Warning: rpmvercmp() expects at least 2 parameters, 1 given in %s
NULL

Warning: rpmvercmp(): c is not a valid comparison operator in %s
NULL

Warning: rpmvercmp() expects at most 3 parameters, 4 given in %s
NULL
Done
