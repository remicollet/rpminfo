<?php
/**
 * Minimal userland OO library
 **/
namespace Remi\RPM;

abstract class Common {
	protected $info = NULL;

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
		$info = rpminfo($path, true, $error);

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
		$info = rpmdbinfo($name, true);

		if (!$info) {
			throw new \RuntimeException("$name not found");
		} else if ($index < 0 || $index >= count($info)) {
			throw new \OutOfBoundsException("$index not in range");
		} else {
			$this->info = $info[$index];
		}
	}
}

