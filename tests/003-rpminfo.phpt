--TEST--
Check for rpminfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpminfo(__DIR__ . "/bidon.rpm"));
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
Done
