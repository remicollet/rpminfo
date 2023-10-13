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
array(5) {
  ["Name"]=>
  string(5) "bidon"
  ["Version"]=>
  string(1) "1"
  ["Release"]=>
  string(11) "2.fc37.remi"
  ["Summary"]=>
  string(5) "Bidon"
  ["Arch"]=>
  string(6) "x86_64"
}
Done
