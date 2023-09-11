<?php

/** @generate-function-entries */

function rpmaddtag(int $rpmtag): bool {}

function rpmdbinfo(string $nevr, bool $full = false): Array|null {}

function rpmdbsearch(string $pattern, int $rpmtag = RPMTAG_NAME, int $rpmmire = -1, bool $full = false): Array|null {}

function rpminfo(string $path, bool $full = false, ?string &$error = null): Array|null {}

function rpmvercmp(string $evr1, string $evr2, ?string $operator = null): int|bool {}


