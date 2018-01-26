--TEST--
Check for rpminfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpminfo(__DIR__ . "/bidon.rpm"));
// Errors
var_dump(rpminfo(__DIR__ . "/missing.rpm"));
var_dump(rpminfo(__FILE__));
?>
Done
--EXPECTF--
array(4) {
  ["Name"]=>
  string(5) "bidon"
  ["Version"]=>
  string(1) "1"
  ["Release"]=>
  string(11) "1.fc25.remi"
  ["Arch"]=>
  string(6) "x86_64"
}

Warning: rpminfo(): Can't open '%s/tests/missing.rpm': No such file or directory in %s/003-rpminfo.php on line %d
bool(false)

Warning: rpminfo(): Can't read '%s/tests/003-rpminfo.php': Argument is not a RPM file in %s/003-rpminfo.php on line %d
bool(false)
Done
