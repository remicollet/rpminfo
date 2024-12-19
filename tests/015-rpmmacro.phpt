--TEST--
Check for rpmdefine, rpmexpand, rpmexpandnumeric
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(is_dir(rpmexpand("%{_dbpath}")));

var_dump(rpmexpandnumeric("%__isa_bits") === PHP_INT_SIZE * 8);
var_dump(rpmexpandnumeric("0%{?fedora}%{?rhel}") > 0);

$name = "_my_test_macro_for_rpminfo_";
$val  = __FILE__;
var_dump(rpmexpand("%$name") === "%$name" );
var_dump(rpmdefine("$name $val"));
var_dump(rpmexpand("%$name") === __FILE__);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

