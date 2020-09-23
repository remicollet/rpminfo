--TEST--
Check for rpmvercmp function
--SKIPIF--
<?php
if (!extension_loaded("rpminfo")) print "skip";
if (PHP_VERSION_ID < 80000) print "skip only for PHP 8";
?>
--FILE--
<?php
try {
	var_dump(rpmvercmp());
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}
try {
	var_dump(rpmvercmp("a"));
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}
try {
	var_dump(rpmvercmp("a", "b", "c"));
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}
?>
Done
--EXPECTF--
rpmvercmp() expects exactly 2 %s, 0 given
rpmvercmp() expects exactly 2 %s, 1 given
rpmvercmp() expects exactly 2 %s, 3 given
Done
