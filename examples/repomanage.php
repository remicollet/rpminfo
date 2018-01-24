#!/usr/bin/php
<?php

extension_loaded("rpminfo") or die("rpminfo extension missing");

for ($i=1 ; $i<$_SERVER['argc'] ; $i++) {
	switch($_SERVER['argv'][$i]) {
		case '-o':
		case '--old':
			if (isset($old)) {
				die("--old option incompatible with --new\n");
			}
			$old = true;
			break;
		case '-n':
		case '--new':
			if (isset($new)) {
				die("--new option incompatible with --old\n");
			}
		case '-k':
		case '--keep':
			if (!isset($_SERVER['argv'][$i+1])) {
				die("Missing argument for --keep option\n");
			}
			$keep = (int)$_SERVER['argv'][$i+1];
			$i++;
			break;
		default:
			$rpms = $_SERVER['argv'][$i];
	}
}
if (!isset($old)) {
	die("Missing --old or --new option\n");
}
if (!isset($keep)) {
	$keep = 1;
}
if (!isset($rpms)) {
	die("Missing rpm directory argument\n");
}
if (is_dir($rpms)) {
} else {
	die("$rpms is not a directory\n");
}

$tree = [];
$handle = popen('rpm -qp --qf "%{NAME} %{EPOCHNUM} %{VERSION} %{RELEASE} %{ARCH} %{NEVR}\n" ' . $rpms . "/*.rpm", "r");
while ($line = fgets($handle)) {
	$tab = explode(' ', trim($line));
	if (count($tab) == 6) {
		$tree[$tab[4]][$tab[0]][] = [
			'name' => $tab[0],
			'path' => $tab[5],
			'evr'  => "${tab[1]}:${tab[2]}-${tab[3]}",
		];
	} else {
		echo "Ignore $line\n";
	}
}
foreach($tree as $arch => $subtree) {
	foreach ($subtree as $name => $versions) {
		if (count($versions) > $keep) {
			if ($old) {
				usort($versions, function($a, $b) { return -rpmvercmp($a['evr'], $b['evr']);} );
			} else {
				usort($versions, function($a, $b) { return rpmvercmp($a['evr'], $b['evr']);} );
			}
			for ($i = $keep ; $i < count($versions) ; $i++) {
				echo $versions[$i]['path'] . "\n";
			}
		}
	}
}

