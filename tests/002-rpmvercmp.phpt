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
	if ($result !== $expected) {
		$ok = false;
		printf("rpmvercmp(%s, %s) = %d when %d expected\n", $a, $b, $result, $expected);
	}
}

$cases = [
	['1', '2', '>',  false],
	['1', '2', 'gt', false],
	['1', '2', '>=', false],
	['1', '2', 'ge', false],
	['1', '1', '>=', true],
	['1', '1', 'ge', true],

	['1', '2', '<',  true],
	['1', '2', 'lt', true],
	['1', '2', '<=', true],
	['1', '2', 'le', true],
	['1', '1', '<=', true],
	['1', '1', 'le', true],

	['1', '1', '=', true],
	['1', '1', '==', true],
	['1', '1', 'eq', true],

	['1', '2', '=',  false],
	['1', '2', '==', false],
	['1', '2', 'eq', false],

	['1', '1', '!=', false],
	['1', '1', '<>', false],
	['1', '1', 'ne', false],

	['1', '2', '!=', true],
	['1', '2', '<>', true],
	['1', '2', 'ne', true],
];
foreach ($cases as $case) {
	list($a,$b,$op,$expected) = $case;
	$result = rpmvercmp($a,$b,$op);
	if ($result !== $expected) {
		$ok = false;
		printf("rpmvercmp(%s, %s, %s) = %s when %s expected\n",
			$a, $b, $op, $result ? "true" : "false", $expected ? "true" : "false");
	}
}

if ($ok) echo "OK\n";
?>
Done
--EXPECTF--
OK
Done
