--TEST--
Check for rpmaddtag parameter check
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpmaddtag(RPMTAG_INSTALLTIME));
try {
	var_dump(rpmaddtag(-1));
} catch (ValueError $e) {
	echo $e->getMessage();
}
?>
--EXPECTF--
bool(true)
%A Unkown rpmtag%A
