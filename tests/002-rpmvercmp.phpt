--TEST--
Check for rpmvercmp function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php
$cases = [
	['1.0', '1.1', -1],
	['1.1', '1.0', 1],
	['1.0', '1.0', 0],
	['1.0', '1', 1],
	['1', '1.1', -1],
	['2.0.14-22.el7_0', '2.0.14.1-35.el7_6', -1],
	['', '', 0],
	['0:1', '1', 0],
	['0:1', '1:1', -1],
	['1:1', '2', 1],
	['1~RC1', '1', -1],
	['1~RC1', '1', -1],
	['1~RC1-1', '1-0', -1],
	['1~beta', '1~RC', 1],
	['1-1', '1-2', -1],
	['1.1-1', '1-1.1', 1],
	['1.1-1~a', '1.1-1', -1],
	['1.2.3-4', '1.2.3p1-2', -1],
	['1.2.3-4', '1.2.3+a-2', -1],
];
$ok = true;
foreach ($cases as $case) {
	list($a,$b,$expected) = $case;
	$result = rpmvercmp($a,$b);
	if ($result != $expected) {
		$ok = false;
		printf("rpmvercmp(%s, %s) = %d when %d expected\n", $a, $b, $result, $expected);
	}
}
if ($ok) echo "OK\n";
// Errors
var_dump(rpmvercmp());
var_dump(rpmvercmp("a"));
var_dump(rpmvercmp("a", "b", "c"));
?>
Done
--EXPECTF--
OK

Warning: rpmvercmp() expects exactly 2 parameters, 0 given in %s/002-rpmvercmp.php on line %d
NULL

Warning: rpmvercmp() expects exactly 2 parameters, 1 given in %s/002-rpmvercmp.php on line %d
NULL

Warning: rpmvercmp() expects exactly 2 parameters, 3 given in %s/002-rpmvercmp.php on line %d
NULL
Done
