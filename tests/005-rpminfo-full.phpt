--TEST--
Check for rpminfo function
--SKIPIF--
<?php if (!extension_loaded("rpminfo")) print "skip"; 
?>
--FILE--
<?php 
foreach ([__DIR__ . "/bidon.rpm", __DIR__ . "/bidon-src.rpm"] as $rpm) {
	echo "--- " . basename($rpm) . " ---\n";
	$i = rpminfo($rpm, true);
	var_dump($i['Name']);
	var_dump($i['Description']);
	var_dump($i['Changelogtext']);
	var_dump($i['IsSource']);
	if (!$i['IsSource']) {
		var_dump($i['Obsoletename']);
		var_dump($i['Obsoleteflags']);
		var_dump($i['Obsoleteversion']);
	}
}
?>
Done
--EXPECTF--
--- bidon.rpm ---
string(5) "bidon"
string(15) "A dummy package"
array(2) {
  [0]=>
  string(14) "- add symlinks"
  [1]=>
  string(20) "- add some hardlinks"
}
bool(false)
array(1) {
  [0]=>
  string(6) "fooobs"
}
array(1) {
  [0]=>
  int(2)
}
array(1) {
  [0]=>
  string(1) "2"
}
--- bidon-src.rpm ---
string(5) "bidon"
string(15) "A dummy package"
array(1) {
  [0]=>
  string(8) "- create"
}
bool(true)
Done
