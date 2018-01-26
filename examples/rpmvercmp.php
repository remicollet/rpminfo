#!/usr/bin/php
<?php

extension_loaded("rpminfo") or die("rpminfo extension missing");

if ($_SERVER['argc'] < 3) {
	die("usage rpmvercmp  evr1 evr2 [ evr3 ... ]\n");
}

$vers = $_SERVER['argv'];
unset($vers[0]);
usort($vers, 'rpmvercmp');
echo implode(' < ', $vers) . "\n";

