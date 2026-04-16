<?php
/*
  +----------------------------------------------------------------------+
  | rpminfo extension for PHP                                            |
  +----------------------------------------------------------------------+
  | SPDX-FileCopyrightText: Copyright (c) Remi Collet <remi@php.net>     |
  +----------------------------------------------------------------------+
  | This source file is subject to the Modified BSD License that is      |
  | bundled with this package in the file LICENSE, and is available      |
  | through the WWW at <https://opensource.org/license/BSD-3-Clause>.    |
  |                                                                      |
  | SPDX-License-Identifier: BSD-3-Clause                                |
  +----------------------------------------------------------------------+
*/

/** @generate-function-entries */

function rpmaddtag(int $rpmtag): bool {}

function rpmdbinfo(string $nevr, bool $full = false): Array|null {}

function rpmdbsearch(string $pattern, int $rpmtag = RPMTAG_NAME, int $rpmmire = -1, bool $full = false): Array|null {}

function rpminfo(string $path, bool $full = false, ?string &$error = null): Array|null {}

function rpmvercmp(string $evr1, string $evr2, ?string $operator = null): int|bool {}

function rpmgetsymlink(string $path, string $name): string|null {}

function rpmexpand(string $text): string {}

function rpmexpandnumeric(string $text): int {}

function rpmdefine(string $macro): bool {}

