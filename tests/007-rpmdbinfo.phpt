--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpmdbinfo('doesntexistsinrpmdb'));
var_dump(rpmdbinfo('bash'));
?>
Done
--EXPECTF--
bool(false)
array(1) {
  [0]=>
  array(4) {
    ["Name"]=>
    string(4) "bash"
    ["Version"]=>
    string(%d) "%s"
    ["Release"]=>
    string(%d) "%s"
    ["Arch"]=>
    string(%d) "%s"
  }
}
Done
