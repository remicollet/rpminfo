--TEST--
Check for rpmdbinfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
var_dump(rpmdbsearch('notexists', RPMTAG_NAME));
try {
var_dump(rpmdbsearch('notexists', RPMTAG_NAME, 99));
} catch (ValueError $e) {
	echo $e->getMessage();
}
?>
--EXPECTF--
NULL
%A Unkown rpmmire%A
