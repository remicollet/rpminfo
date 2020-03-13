--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpmdbinfo('doesntexistsinrpmdb'));
var_dump(rpmdbinfo('bash'));

$k = rpmdbinfo('kernel');
var_dump(count($k) > 0); // multiple kernels
$n = $k[0]['Name'] . '-' . $k[0]['Version'] . '-'  . $k[0]['Release'];

$k = rpmdbinfo($n); // single kernel with NEVR
var_dump(count($k) ==1);

?>
Done
--EXPECTF--
bool(false)
array(1) {
  [0]=>
  array(5) {
    ["Name"]=>
    string(4) "bash"
    ["Version"]=>
    string(%d) "%s"
    ["Release"]=>
    string(%d) "%s"
    ["Summary"]=>
    string(26) "The GNU Bourne Again shell"
    ["Arch"]=>
    string(%d) "%s"
  }
}
bool(true)
bool(true)
Done
