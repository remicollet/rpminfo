--TEST--
Check for rpmvercmp function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpmvercmp("1.0", "1.1"));
var_dump(rpmvercmp("1.1", "1.0"));
var_dump(rpmvercmp("1.0", "1.0"));
// Errors
var_dump(rpmvercmp());
var_dump(rpmvercmp("a"));
var_dump(rpmvercmp("a", "b", "c"));
?>
Done
--EXPECT--
int(-1)
int(1)
int(0)

Warning: rpmvercmp() expects exactly 2 parameters, 0 given in /work/GIT/php-rpminfo/tests/002.php on line 6
NULL

Warning: rpmvercmp() expects exactly 2 parameters, 1 given in /work/GIT/php-rpminfo/tests/002.php on line 7
NULL

Warning: rpmvercmp() expects exactly 2 parameters, 3 given in /work/GIT/php-rpminfo/tests/002.php on line 8
NULL
Done
