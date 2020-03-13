--TEST--
Check for rpmdbinfo function on NEVR
--SKIPIF--
<?php
if (!extension_loaded("rpminfo")) print "skip";
if (!rpmdbinfo('kernel')) print "skip kernel not installed";
?>
--FILE--
<?php 
$k = rpmdbinfo('kernel');
var_dump(count($k) > 0); // multiple kernels
$n = $k[0]['Name'] . '-' . $k[0]['Version'] . '-'  . $k[0]['Release'];

$k = rpmdbinfo($n); // single kernel with NEVR
var_dump(count($k) ==1);

?>
Done
--EXPECTF--
bool(true)
bool(true)
Done
