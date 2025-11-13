# Unreleased

-

# Version 1.2.1 - 2025-09-25

- use `RPMTAG_SIGMD5` instead of `RPMTAG_PKGID` removed in RPM 6
- use `RPMTAG_SHA1HEADER` instead of `RPMTAG_HDRID` removed in RPM 6

# Version 1.2.0 - 2024-12-19

- add `rpmexpand`, `rpmexpandnumeric` to retrieve rpm macro value
- add `rpmdefine` to set rpm macro value

# Version 1.1.1 - 2024-09-03

- display author and license in phpinfo
- drop support for librpm < 4.13

# Version 1.1.0 - 2023-11-10

- check open_basedir restriction
- new function: `rpmgetsymlink(string $path, string $name): ?string`

# Version 1.0.1 - 2023-10-13

- fix stack smashing on 32-bit
- allow retrieval of hardlink content

# Version 1.0.0 - 2023-10-12

- implement rpm stream wrapper with librpm >= 4.13

# Version 0.7.0 - 2023-09-26

- add optional operator to `rpmcmpver` for consistency with version_compare
- drop support for PHP 7

# Version 0.6.0 - 2021-06-18

- generate arginfo from stub and add missing default values
- raise dependency on PHP 7.2
- raise exception on bad parameter value with PHP 8

# Version 0.5.1 - 2020-09-23

- split tests for PHP 7/8
- improve librpm example

# Version 0.5.0 - 2020-04-07

- add `rpmaddtag()` function

# Version 0.4.2 - 2020-03-25

- improve reflection with better parameter names
- speed optimization: open DB only once per request

# Version 0.4.1 - 2020-03-18

- fix build with RPM 4.12 (Fedora 21-22 only)
- add type hinting in reflection
- return `NULL` instead of `FALSE` on failure

# Version 0.4.0 - 2020-03-13

- improve search logic, use index when exists and no search mode
- add 'full' parameter to `rpmdbsearch`
- allow `rpmdbinfo` to search on NEVR (instead of name only)
- first "stable" release

# Version 0.3.1 - 2020-03-12

- allow search by Pkgid, Hdrid, Installtid with specific input
- fix search with various other tags (Version, ...)

# Version 0.3.0 - 2020-03-12

- add `rpmdbsearch` function to search packages using
  name, installed files, requires, provides...

# Version 0.2.3 - 2020-03-11

- fix gh#2 free allocated iterators to avoid "DB2053 Freeing read locks..." messages

# Version 0.2.2 - 2020-03-11

- Fix gh#1 `rpmvercmp()` incorrect comparison result

# Version 0.2.1 - 2018-02-12

- add summary in minimal information set
- retrieve array from metadata
- add `RPMSENSE_*` macros

# Version 0.2.0 - 2018-02-08

- new function:
  `array rpmdbinfo(string name [, bool full]);`

# Version 0.1.3 - 2018-02-08

- first pecl release
- `rpminfo()`: add option to retrieve error message instead of raising a warning

# Version 0.1.1 - 2018-01-26

- fix segfault in ZTS mode
- define `RPMVERSION` constant

# Version 0.1.0 - 2018-01-26
- first release with 2 functions:
  `int rpmvercmp(string evr1, string evr2);`
  `array rpminfo(string path [, bool full]);`
