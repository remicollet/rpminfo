--TEST--
Check for stream
--SKIPIF--
<?php
if (!extension_loaded("rpminfo")) print "skip";
if (version_compare(RPMVERSION, '4.13', 'lt')) print("skip librpm is older than 4.13");
?>
--FILE--
<?php 
$n = "rpm://" . __DIR__ . "/bidon.rpm#/usr/share/doc/bidon/README";

var_dump(in_array('rpm', stream_get_wrappers()));

var_dump($f = fopen($n, "r"));
$s = fstat($f);
var_dump($s['size'], $s['mode']);
var_dump(trim(fread($f, 10)));
var_dump(feof($f));
var_dump(trim(fread($f, 100)));
var_dump(feof($f));
fclose($f);

var_dump(trim(file_get_contents($n)));

var_dump(file_get_contents(str_replace('README', 'TODO', $n)));
?>
Done
--EXPECTF--
bool(true)
resource(%d) of type (stream)
int(29)
int(33188)
string(10) "Mon Feb 12"
bool(false)
string(17) "13:27:47 CET 2018"
bool(true)
string(28) "Mon Feb 12 13:27:47 CET 2018"

Warning: file_get_contents(%s/bidon.rpm#/usr/share/doc/bidon/TODO): Failed to open stream: operation failed in %s on line %d
bool(false)
Done
