--TEST--
Check for rpminfo presence
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; ?>
--FILE--
<?php 
echo "rpminfo extension is available";
?>
--EXPECT--
rpminfo extension is available
