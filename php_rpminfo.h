/*
  +----------------------------------------------------------------------+
  | rpminfo extension for PHP                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) The PHP Group                                          |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Remi Collet <remi@php.net>                                   |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_RPMINFO_H
#define PHP_RPMINFO_H

extern zend_module_entry rpminfo_module_entry;
#define phpext_rpminfo_ptr &rpminfo_module_entry

#define PHP_RPMINFO_VERSION "0.5.2-dev"

#ifdef PHP_WIN32
#	define PHP_RPMINFO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_RPMINFO_API __attribute__ ((visibility("default")))
#else
#	define PHP_RPMINFO_API
#endif

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

