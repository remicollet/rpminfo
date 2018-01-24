/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2018 The PHP Group                                     |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_rpminfo.h"

#include <rpm/rpmlib.h>

/* If you declare any globals in php_rpminfo.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(rpminfo)
*/

/* True global resources - no need for thread safety here */
static int le_rpminfo;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("rpminfo.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_rpminfo_globals, rpminfo_globals)
    STD_PHP_INI_ENTRY("rpminfo.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_rpminfo_globals, rpminfo_globals)
PHP_INI_END()
*/
/* }}} */


ZEND_BEGIN_ARG_INFO_EX(arginfo_rpmvercmp, 0, 0, 2)
	ZEND_ARG_INFO(0, evr1)
	ZEND_ARG_INFO(0, evr2)
ZEND_END_ARG_INFO()

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string rpmcmpver(string evr1, string evr2)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(rpmvercmp)
{
	char *evr1, *evr2;
	size_t len1, len2;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &evr1, &len1, &evr2, &len2) == FAILURE) {
		return;
	}

	RETURN_LONG(rpmvercmp(evr1, evr2));
}

/* {{{ php_rpminfo_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_rpminfo_init_globals(zend_rpminfo_globals *rpminfo_globals)
{
	rpminfo_globals->global_value = 0;
	rpminfo_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(rpminfo)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(rpminfo)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(rpminfo)
{
#if defined(COMPILE_DL_RPMINFO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(rpminfo)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(rpminfo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "rpminfo support", "enabled");
	php_info_print_table_row(2, "Extension version", PHP_RPMINFO_VERSION);
	php_info_print_table_row(2, "RPM library version", RPMVERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ rpminfo_functions[]
 *
 * Every user visible function must have an entry in rpminfo_functions[].
 */
const zend_function_entry rpminfo_functions[] = {
	PHP_FE(rpmvercmp,	      arginfo_rpmvercmp)
	PHP_FE_END
};
/* }}} */

/* {{{ rpminfo_module_entry
 */
zend_module_entry rpminfo_module_entry = {
	STANDARD_MODULE_HEADER,
	"rpminfo",
	rpminfo_functions,
	PHP_MINIT(rpminfo),
	PHP_MSHUTDOWN(rpminfo),
	PHP_RINIT(rpminfo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(rpminfo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(rpminfo),
	PHP_RPMINFO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RPMINFO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(rpminfo)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
