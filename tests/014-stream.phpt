--TEST--
Check for stream
--SKIPIF--
<?php
if (!extension_loaded("rpminfo")) print "skip";
if (version_compare(RPMVERSION, '4.13', 'lt')) print("skip librpm is older than 4.13");
?>
--FILE--
<?php 
$d = "rpm://" . __DIR__ . "/bidon.rpm#/usr/share/doc/bidon";
$n = "rpm://" . __DIR__ . "/bidon.rpm#/usr/share/doc/bidon/README";

echo "+ wrapper\n";
var_dump(in_array('rpm', stream_get_wrappers()));

echo "+ stat\n";
$s = stat($d); // S_ISDIR
var_dump($s['size'], $s['mode'] , $s['mode'] & 0040000 ? "OK" : "KO");
var_dump(file_exists($d), is_dir($d), is_file($d));
$s = stat($n); // S_ISREG
var_dump($s['size'], $s['mode'] , $s['mode'] & 0100000 ? "OK" : "KO");
var_dump(file_exists($n), is_dir($n), is_file($n));

echo "+ file\n";
var_dump($f = fopen($n, "r"));
$s = fstat($f);
var_dump($s['size'], $s['mode']);
var_dump(trim(fread($f, 10)));
var_dump(feof($f));
var_dump(trim(fread($f, 100)));
var_dump(feof($f));
fclose($f);

echo "+ stream\n";
var_dump(trim(file_get_contents($n)));

var_dump(file_get_contents(str_replace('README', 'TODO', $n)));
?>
Done
--EXPECTF--
+ wrapper
bool(true)
+ stat
int(0)
int(16877)
string(2) "OK"
bool(true)
bool(true)
bool(false)
int(29)
int(33188)
string(2) "OK"
bool(true)
bool(false)
bool(true)
+ file
resource(%d) of type (stream)
int(29)
int(33188)
string(10) "Mon Feb 12"
bool(false)
string(17) "13:27:47 CET 2018"
bool(true)
+ stream
string(28) "Mon Feb 12 13:27:47 CET 2018"

Warning: file_get_contents(%s/bidon.rpm#/usr/share/doc/bidon/TODO): Failed to open stream: operation failed in %s on line %d
bool(false)
Done
