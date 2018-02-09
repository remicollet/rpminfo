--TEST--
Check for rpminfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; 
?>
--FILE--
<?php 
foreach ([__DIR__ . "/bidon.rpm", __DIR__ . "/bidon-src.rpm"] as $rpm) {
	echo "--- $rpm ---\n";
	$i = rpminfo($rpm, true);
	var_dump($i['Name']);
	var_dump($i['Description']);
	var_dump($i['Changelogtext']);
	var_dump($i['IsSource']);
}
?>
Done
--EXPECTF--
--- /work/GIT/rpminfo/tests/bidon.rpm ---
string(5) "bidon"
string(15) "A dummy package"
array(1) {
  [0]=>
  string(8) "- create"
}
bool(false)
--- /work/GIT/rpminfo/tests/bidon-src.rpm ---
string(5) "bidon"
string(15) "A dummy package"
array(1) {
  [0]=>
  string(8) "- create"
}
bool(true)
Done
