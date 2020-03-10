--TEST--
Check for rpmvercmp function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpmvercmp("1.0", "1.1"));
var_dump(rpmvercmp("1.1", "1.0"));
var_dump(rpmvercmp("1.0", "1.0"));
var_dump(rpmvercmp("2.0.14-22.el7_0", "2.0.14.1-35.el7_6"));
// Errors
var_dump(rpmvercmp());
var_dump(rpmvercmp("a"));
var_dump(rpmvercmp("a", "b", "c"));
?>
Done
--EXPECTF--
int(-1)
int(1)
int(0)
int(-1)

Warning: rpmvercmp() expects exactly 2 parameters, 0 given in %s/002-rpmvercmp.php on line %d
NULL

Warning: rpmvercmp() expects exactly 2 parameters, 1 given in %s/002-rpmvercmp.php on line %d
NULL

Warning: rpmvercmp() expects exactly 2 parameters, 3 given in %s/002-rpmvercmp.php on line %d
NULL
Done
