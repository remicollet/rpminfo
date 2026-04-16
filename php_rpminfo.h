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

#ifndef PHP_RPMINFO_H
#define PHP_RPMINFO_H

extern zend_module_entry rpminfo_module_entry;
#define phpext_rpminfo_ptr &rpminfo_module_entry

#define PHP_RPMINFO_VERSION "1.2.2-dev"
#define PHP_RPMINFO_AUTHOR  "Remi Collet"
#define PHP_RPMINFO_LICENSE "BSD-3-Clause"

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(rpminfo)
	rpmts ts;			/* transaction set 	*/
	rpmdb db;			/* database			*/
	int nb_tags; 		/* stored tags 		*/
	int max_tags; 		/* allocated tags 	*/
	rpmTagVal *tags; 	/* tags storage 	*/
ZEND_END_MODULE_GLOBALS(rpminfo)

#define RPMINFO_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(rpminfo, v)

#if defined(ZTS) && defined(COMPILE_DL_RPMINFO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_RPMINFO_H */

