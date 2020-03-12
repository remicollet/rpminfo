<?php
/**
 * Minimal userland OO library
 **/
namespace Remi\RPM;

abstract class Common {
	protected $info = NULL;

	protected function _dep(Array $names, Array $flags, Array $vers) {
		$ret = [];
		$signs = [
			RPMSENSE_EQUAL                    => ' = ',
			RPMSENSE_LESS                     => ' < ',
			RPMSENSE_GREATER                  => ' > ',
			RPMSENSE_LESS    | RPMSENSE_EQUAL => ' <= ',
			RPMSENSE_GREATER | RPMSENSE_EQUAL => ' >= ',
		];

		if (count($names) == count($vers) && count($vers) == count($flags)) {
			for ($i=0 ; $i<count($names) ; $i++) {
				if (isset($signs[$flags[$i] & 15])) {
					$ret[] = $names[$i] . $signs[$flags[$i] & 15] . $vers[$i];
				} else {
					$ret[] = $names[$i] . $vers[$i];
				}
			}
		}
		return $ret;
	}

	protected function _files() {
		$ret = [];
		for ($i=0 ; $i<count($this->info['Basenames']) ; $i++) {
			$ret[] = $this->info['Dirnames'][$this->info['Dirindexes'][$i]] . $this->info['Basenames'][$i];
		}
		return $ret;
	}

	public function __get($name) {
		switch ($name) {
			case 'EVR':
			case 'NEVR':
			case 'NEVRA':
				$ret = '';
				if (substr($name, 0, 1) === 'N') {
					$ret = $this->info['Name'] . '-';
				}
				if (isset($this->info['Epoch'])) {
					$ret .= $this->info['Epoch'] . ':';
				}
				$ret .= $this->info['Version'] . '-' . $this->info['Release'];

				if (substr($name, -1) === 'A') {
					$ret .= '.' . $this->info['Arch'];
				}
				return $ret;
			case 'Requires':
				if (isset($this->info['Requirename'])) {
					return $this->_dep($this->info['Requirename'], $this->info['Requireflags'], $this->info['Requireversion']);
				}
			case 'Conflicts':
				if (isset($this->info['Conflictname'])) {
					return $this->_dep($this->info['Conflictname'], $this->info['Conflictflags'], $this->info['Conflictversion']);
				}
			case 'Obsoletes':
				if (isset($this->info['Obsoletename'])) {
					return $this->_dep($this->info['Obsoletename'], $this->info['Obsoleteflags'], $this->info['Obsoleteversion']);
				}
			case 'Provides':
				if (isset($this->info['Providename'])) {
					return $this->_dep($this->info['Providename'], $this->info['Provideflags'], $this->info['Provideversion']);
				}
			case 'Files':
				if (isset($this->info['Basenames'])) {
					return $this->_files();
				}
			default:
				if (isset($this->info[$name])) {
					return $this->info[$name];
				}
				return NULL;
		}
	}
}

/**
 * From a RPM file
 *
 * $a = new \Remi\RPM\File(dirname(__DIR__) . '/tests/bidon.rpm');
 * var_dump($a->Vendor);
 * var_dump($a->NEVRA);
 **/
class File extends Common {

	public function __construct($path) {
		$info = \rpminfo($path, true, $error);

		if ($error) {
			throw new \RuntimeException($error);
		} else {
			$this->info = $info;
		}
	}
}

/**
 * From an installed RPM
 *
 * $a = new \Remi\RPM\Package('php');
 * var_dump($a->License);
 * var_dump($a->NEVRA);
 **/
class Package extends Common {

	public function __construct($name, $index=0) {
		$info = \rpmdbinfo($name, true);

		if (!$info) {
			throw new \RuntimeException("$name not found");
		} else if ($index < 0 || $index >= count($info)) {
			throw new \OutOfBoundsException("$index not in range");
		} else {
			$this->info = $info[$index];
		}
	}
}

/**
 * Find packages which provides something or own a file
 *
 * $a = \Remi\RPM\WhatProvides("php-redis");
 * print_r($a[0]->NEVR);
 *
 * $a = \Remi\RPM\WhatProvides("/usr/bin/php");
 * print_r($a[0]->NEVR);
 **/
function WhatProvides($crit) {
	if (file_exists($crit)) {
		$a = \rpmdbsearch($crit, RPMTAG_INSTFILENAMES);
	} else {
		$a = \rpmdbsearch($crit, RPMTAG_PROVIDES);
	}
	$r = [];
	if (is_array($a)) {
		foreach($a as $i) {
			$r[] = new Package($i['Name']);
		}
	}
	return $r;
}

/**
 * Find packages which requires something
 *
 * $a = \Remi\RPM\WhatRequires("php-common");
 * print_r($a[0]->NEVRA);
 **/
function WhatRequires($crit) {
	$a = \rpmdbsearch($crit, RPMTAG_REQUIRES);

	$r = [];
	if (is_array($a)) {
		foreach($a as $i) {
			$r[] = new Package($i['Name']);
		}
	}
	return $r;
}
/*
$a = new File(dirname(__DIR__).'/tests/bidon.rpm');
$a = new Package('phpunit7');
print_r($a->Requires);
*/

