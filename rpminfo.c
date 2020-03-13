/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include <fcntl.h>
#include <rpm/rpmdb.h>
#include <rpm/rpmio.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmts.h>

#include "php_rpminfo.h"

ZEND_DECLARE_MODULE_GLOBALS(rpminfo)

static rpmts rpminfo_getts(rpmVSFlags flags) {
	if (!RPMINFO_G(ts)) {
		rpmReadConfigFiles(NULL, NULL);
		RPMINFO_G(ts) = rpmtsCreate();
	}
	if (RPMINFO_G(ts)) {
		(void)rpmtsSetVSFlags(RPMINFO_G(ts), flags);
	}
	return RPMINFO_G(ts);
}

static void rpm_header_to_zval(zval *return_value, Header h, zend_bool full)
{
	HeaderIterator hi;
	rpmTagVal tag;
	rpmTagType type;
	const char *val;

	array_init(return_value);
	hi = headerInitIterator(h);
	while ((tag=headerNextTag(hi)) != RPMTAG_NOT_FOUND) {
		switch (tag) {
			case RPMTAG_NAME:
			case RPMTAG_VERSION:
			case RPMTAG_RELEASE:
			case RPMTAG_EPOCH:
			case RPMTAG_ARCH:
			case RPMTAG_SUMMARY:
				break;
			default:
				if (!full) {
					continue;
				}
		}

		//printf("Tag: %-30s  Type: %8lx - %8lx - %8lx\n", rpmTagGetName(tag), (long)rpmTagGetTagType(tag), (long)rpmTagGetType(tag), (long)rpmTagGetReturnType(tag));
		type = rpmTagGetTagType(tag);
		switch (type) {
			case RPM_STRING_TYPE:
			case RPM_I18NSTRING_TYPE:
				val = headerGetString(h, tag);
				if (val) {
					add_assoc_string(return_value, rpmTagGetName(tag), headerGetAsString(h, tag));
				} else {
					add_assoc_null(return_value, rpmTagGetName(tag));
				}
				break;
			case RPM_CHAR_TYPE:
			case RPM_INT8_TYPE:
				add_assoc_long(return_value, rpmTagGetName(tag), (zend_long)headerGetNumber(h, tag));
				break;
			case RPM_INT16_TYPE:
				if (rpmTagGetReturnType(tag) == RPM_ARRAY_RETURN_TYPE) {
					struct rpmtd_s keys;
					if (headerGet(h, tag, &keys, HEADERGET_MINMEM)) {
						uint16_t *key;
						zval tmp;

						array_init(&tmp);
						rpmtdInit(&keys);
						while (rpmtdNext(&keys)>=0) {
							key = rpmtdGetUint16(&keys);
							add_next_index_long(&tmp, (zend_long)*key);
						}
						add_assoc_zval(return_value, rpmTagGetName(tag), &tmp);
					} else {
						add_assoc_null(return_value, rpmTagGetName(tag));
					}
				} else {
					add_assoc_long(return_value, rpmTagGetName(tag), (zend_long)headerGetNumber(h, tag));
				}
				break;
			case RPM_INT32_TYPE:
				if (rpmTagGetReturnType(tag) == RPM_ARRAY_RETURN_TYPE) {
					struct rpmtd_s keys;
					if (headerGet(h, tag, &keys, HEADERGET_MINMEM)) {
						uint32_t *key;
						zval tmp;

						array_init(&tmp);
						rpmtdInit(&keys);
						while (rpmtdNext(&keys)>=0) {
							key = rpmtdGetUint32(&keys);
							add_next_index_long(&tmp, (zend_long)*key);
						}
						add_assoc_zval(return_value, rpmTagGetName(tag), &tmp);
					} else {
						add_assoc_null(return_value, rpmTagGetName(tag));
					}
				} else {
					add_assoc_long(return_value, rpmTagGetName(tag), (zend_long)headerGetNumber(h, tag));
				}
				break;
			case RPM_INT64_TYPE:
				if (rpmTagGetReturnType(tag) == RPM_ARRAY_RETURN_TYPE) {
					struct rpmtd_s keys;
					if (headerGet(h, tag, &keys, HEADERGET_MINMEM)) {
						uint64_t *key;
						zval tmp;

						array_init(&tmp);
						rpmtdInit(&keys);
						while (rpmtdNext(&keys)>=0) {
							key = rpmtdGetUint64(&keys);
							add_next_index_long(&tmp, (zend_long)*key);
						}
						add_assoc_zval(return_value, rpmTagGetName(tag), &tmp);
					} else {
						add_assoc_null(return_value, rpmTagGetName(tag));
					}
				} else {
					add_assoc_long(return_value, rpmTagGetName(tag), (zend_long)headerGetNumber(h, tag));
				}
				break;
			case RPM_STRING_ARRAY_TYPE:
				{
					struct rpmtd_s keys;
					if (headerGet(h, tag, &keys, HEADERGET_MINMEM)) {
						const char *key;
						zval tmp;

						array_init(&tmp);
						while ((key = rpmtdNextString(&keys))) {
							add_next_index_string(&tmp, key);
						}
						add_assoc_zval(return_value, rpmTagGetName(tag), &tmp);
					} else {
						add_assoc_null(return_value, rpmTagGetName(tag));
					}
				}
				break;
			default:
				val = headerGetAsString(h, tag);
				if (val) {
					add_assoc_string(return_value, rpmTagGetName(tag), headerGetAsString(h, tag));
				} else {
					add_assoc_null(return_value, rpmTagGetName(tag));
				}
		}
	}
	headerFreeIterator(hi);
	if (full) {
		add_assoc_bool(return_value, "IsSource", headerIsSource(h));
	}
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_rpminfo, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, full)
	ZEND_ARG_INFO(1, error)
ZEND_END_ARG_INFO()

/* {{{ proto array rpminfo(string path [, bool full [, string &$error])
   Retrieve information from a RPM file */
PHP_FUNCTION(rpminfo)
{
	char *path, *e_msg;
	size_t len, e_len=0;
	zend_bool full = 0;
	zval *error = NULL;
	FD_t f;
	Header h;
	rpmts ts = rpminfo_getts(_RPMVSF_NODIGESTS | _RPMVSF_NOSIGNATURES | RPMVSF_NOHDRCHK);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "p|bz", &path, &len, &full, &error) == FAILURE) {
		return;
	}
	if (error) {
		ZVAL_DEREF(error);
		zval_dtor(error);
		ZVAL_NULL(error);
	}

	f = Fopen(path, "r");
	if (f) {
		int rc;

		rc = rpmReadPackageFile(ts, f, "rpminfo", &h);
		if (rc == RPMRC_OK || rc == RPMRC_NOKEY || rc == RPMRC_NOTTRUSTED) {
			rpm_header_to_zval(return_value, h, full);
			if (h) {
				headerFree(h);
			}
			Fclose(f);
			return;

		} else if (rc == RPMRC_NOTFOUND) {
			e_len = spprintf(&e_msg, 0, "Can't read '%s': Argument is not a RPM file", path);
		} else if (rc == RPMRC_NOTFOUND) {
			e_len = spprintf(&e_msg, 0, "Can't read '%s': Error reading header from package", path);
		} else {
			e_len = spprintf(&e_msg, 0, "Can't read '%s': Unkown error", path);
		}

		Fclose(f);
	} else {
		e_len = spprintf(&e_msg, 0, "Can't open '%s': %s", path, Fstrerror(f));
	}
	if (e_len) {
		if (error) {
			ZVAL_STRINGL(error, e_msg, e_len);
		} else {
			php_error_docref(NULL, E_WARNING, "%s", e_msg);
		}
		efree(e_msg);
	}
	RETURN_FALSE;
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_rpmdbinfo, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, full)
ZEND_END_ARG_INFO()

/* {{{ proto array rpmdbinfo(string name [, bool full])
   Retrieve information from an installed RPM */
PHP_FUNCTION(rpmdbinfo)
{
	char *name;
	size_t len;
	zend_bool full = 0;
	Header h;
	rpmdb db;
	rpmdbMatchIterator di;
	rpmts ts = rpminfo_getts(_RPMVSF_NODIGESTS | _RPMVSF_NOSIGNATURES | RPMVSF_NOHDRCHK);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "p|b", &name, &len, &full) == FAILURE) {
		return;
	}

	rpmtsOpenDB(ts, O_RDONLY);
	db = rpmtsGetRdb(ts);
	di = rpmdbInitIterator(db, RPMDBI_LABEL, name, len);
	if (!di) {
		// Not found
		rpmtsCloseDB(ts);
		RETURN_FALSE;
	}

	array_init(return_value);
	while ((h = rpmdbNextIterator(di)) != NULL) {
		zval tmp;
		rpm_header_to_zval(&tmp, h, full);
		add_next_index_zval(return_value, &tmp);
	}

	rpmdbFreeIterator(di);
	rpmtsCloseDB(ts);
}
/* }}} */

static unsigned char nibble(char c) {
	if (c >= '0' && c <= '9') {
		return (c - '0');
	}
	if (c >= 'a' && c <= 'f') {
		return (c - 'a') + 10;
	}
	if (c >= 'A' && c <= 'F') {
		return (c - 'A') + 10;
	}
    return 0;
}

static int hex2bin(const char *hex, char *bin, int len) {
	int i;

	for (i=0 ; (i+1)<len ; i+=2, hex+=2, bin++) {
		*bin = nibble(hex[0]) << 4 | nibble(hex[1]);
	}

	return i/2;
}

static int haveIndex(zend_long tag) {
	/*
		All DB indexes
		excepted RPMDBI_PACKAGES and RPMDBI_LABEL doesn't match any tag
	*/
	if (tag == RPMDBI_NAME ||
		tag == RPMDBI_BASENAMES ||
		tag == RPMDBI_GROUP ||
		tag == RPMDBI_REQUIRENAME ||
		tag == RPMDBI_PROVIDENAME ||
		tag == RPMDBI_CONFLICTNAME ||
		tag == RPMDBI_OBSOLETENAME ||
		tag == RPMDBI_TRIGGERNAME ||
		tag == RPMDBI_DIRNAMES ||
		tag == RPMDBI_INSTALLTID ||
		tag == RPMDBI_SIGMD5 ||
		tag == RPMDBI_SHA1HEADER ||
#ifdef HAVE_WEAKDEP
		tag == RPMDBI_FILETRIGGERNAME ||
		tag == RPMDBI_TRANSFILETRIGGERNAME ||
		tag == RPMDBI_RECOMMENDNAME ||
		tag == RPMDBI_SUGGESTNAME ||
		tag == RPMDBI_SUPPLEMENTNAME ||
		tag == RPMDBI_ENHANCENAME ||
#endif
		tag == RPMDBI_INSTFILENAMES) {
		return 1;
	}
	return 0;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_rpmdbsearch, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, rpmtag)
	ZEND_ARG_INFO(0, rpmmire)
	ZEND_ARG_INFO(0, full)
ZEND_END_ARG_INFO()

/* {{{ proto array rpmdbsearch(string pattern [, integer tag_name = RPMTAG_NAME [, integer mode = -1 [, bool full = 0]]])
   Search information from installed RPMs */
PHP_FUNCTION(rpmdbsearch)
{
	char MD5[16];
	rpm_tid_t tid;
	char *name;
	size_t len;
	zend_long crit = RPMTAG_NAME;
	zend_long mode = -1;
	zend_bool full = 0;
	Header h;
	rpmdb db;
	rpmdbMatchIterator di;
	int useIndex = 1;
	rpmts ts = rpminfo_getts(_RPMVSF_NODIGESTS | _RPMVSF_NOSIGNATURES | RPMVSF_NOHDRCHK);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|llb", &name, &len, &crit, &mode, &full) == FAILURE) {
		return;
	}

	if (crit == RPMTAG_PKGID) {
		if (len != 32) {
			php_error_docref(NULL, E_WARNING, "Bad length for PKGID, 32 expected");
			RETURN_FALSE;
		}
		len = hex2bin(name, MD5, len);
		name = MD5;
	} else if (crit == RPMTAG_HDRID) {
		if (len != 40) {
			php_error_docref(NULL, E_WARNING, "Bad length for HDRID, 40 expected");
			RETURN_FALSE;
		}
	} else if (crit == RPMTAG_INSTALLTID) {
		tid = atol(name);
		name = (char *)&tid;
		len = sizeof(tid);
	} else if (crit == RPMTAG_INSTFILENAMES) {
		/* use input parameters */
	} else {
		/* Faster mode if index exists and name is not a pattern */
		useIndex = haveIndex(crit) && mode < 0;
	}

	rpmtsOpenDB(ts, O_RDONLY);
	db = rpmtsGetRdb(ts);
	if (useIndex) {
		/* Simple criterion using index */
		di = rpmdbInitIterator(db, crit, name, len);
	} else {
		/* query all packages */
		di = rpmdbInitIterator(db, RPMDBI_PACKAGES, NULL, 0);
		/* add criterion */
		if (di) {
			if (rpmdbSetIteratorRE(di, crit, (mode<0 ? RPMMIRE_DEFAULT : mode), name)) {
				php_error_docref(NULL, E_WARNING, "Can't set filter");
				rpmtsCloseDB(ts);
				RETURN_FALSE;
			}
		}
	}
	if (!di) {
		// Not found
		rpmtsCloseDB(ts);
		RETURN_FALSE;
	}

	array_init_size(return_value, rpmdbGetIteratorCount(di));
	while ((h = rpmdbNextIterator(di)) != NULL) {
		zval tmp;
		rpm_header_to_zval(&tmp, h, full);
		add_next_index_zval(return_value, &tmp);
	}

	rpmdbFreeIterator(di);
	rpmtsCloseDB(ts);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_rpmvercmp, 0, 0, 2)
	ZEND_ARG_INFO(0, evr1)
	ZEND_ARG_INFO(0, evr2)
ZEND_END_ARG_INFO()

/* {{{ proto int rpmcmpver(string evr1, string evr2)
   Compare 2 RPM EVRs (epoch:version-release) strings */
PHP_FUNCTION(rpmvercmp)
{
	char *in_evr1, *evr1, *v1, *r1;
	char *in_evr2, *evr2, *v2, *r2;
	char *p, empty[] = "";
	long e1, e2, r;
	size_t len1, len2;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &in_evr1, &len1, &in_evr2, &len2) == FAILURE) {
		return;
	}
	evr1 = estrdup(in_evr1);
	evr2 = estrdup(in_evr2);

	// Epoch
	p = strchr(evr1, ':');
	if (p) {
		v1 = p+1;
		*p=0;
		e1 = atol(evr1);
	} else {
		v1 = evr1;
		e1 = 0;
	}
	p = strchr(evr2, ':');
	if (p) {
		v2 = p+1;
		*p=0;
		e2 = atol(evr2);
	} else {
		v2 = evr2;
		e2 = 0;
	}
	if (e1 < e2) {
		RETVAL_LONG(-1);
	} else if (e1 > e2) {
		RETVAL_LONG(1);
	} else {
		// Version
		p = strchr(v1, '-');
		if (p) {
			r1 = p+1;
			*p = 0;
		} else {
			r1 = empty;
		}
		p = strchr(v2, '-');
		if (p) {
			r2 = p+1;
			*p = 0;
		} else {
			r2 = empty;
		}
		r = rpmvercmp(v1, v2);
		if (r) {
			RETVAL_LONG(r);
		} else {
			// Release
			r = rpmvercmp(r1, r2);
			RETVAL_LONG(r);
		}
	}
	efree(evr1);
	efree(evr2);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(rpminfo)
{
    const char *tagname;
    rpmtd names;

	REGISTER_STRING_CONSTANT("RPMVERSION", (char *)RPMVERSION, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("RPMSENSE_ANY",           RPMSENSE_ANY,           CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_LESS",          RPMSENSE_LESS,          CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_GREATER",       RPMSENSE_GREATER,       CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_EQUAL",         RPMSENSE_EQUAL,         CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_POSTTRANS",     RPMSENSE_POSTTRANS,     CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_PREREQ",        RPMSENSE_PREREQ,        CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_PRETRANS",      RPMSENSE_PRETRANS,      CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_INTERP",        RPMSENSE_INTERP,        CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_SCRIPT_PRE",    RPMSENSE_SCRIPT_PRE,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_SCRIPT_POST",   RPMSENSE_SCRIPT_POST,   CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_SCRIPT_PREUN",  RPMSENSE_SCRIPT_PREUN,  CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_SCRIPT_POSTUN", RPMSENSE_SCRIPT_POSTUN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_SCRIPT_VERIFY", RPMSENSE_SCRIPT_VERIFY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_FIND_REQUIRES", RPMSENSE_FIND_REQUIRES, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_FIND_PROVIDES", RPMSENSE_FIND_PROVIDES, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_TRIGGERIN",     RPMSENSE_TRIGGERIN,     CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_TRIGGERUN",     RPMSENSE_TRIGGERUN,     CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_TRIGGERPOSTUN", RPMSENSE_TRIGGERPOSTUN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_MISSINGOK",     RPMSENSE_MISSINGOK,     CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_RPMLIB",        RPMSENSE_RPMLIB,        CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_TRIGGERPREIN",  RPMSENSE_TRIGGERPREIN,  CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_KEYRING",       RPMSENSE_KEYRING,       CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMSENSE_CONFIG",        RPMSENSE_CONFIG,        CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("RPMMIRE_DEFAULT",        RPMMIRE_DEFAULT,        CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMMIRE_STRCMP",         RPMMIRE_STRCMP,         CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMMIRE_REGEX",          RPMMIRE_REGEX,          CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("RPMMIRE_GLOB",           RPMMIRE_GLOB,           CONST_CS | CONST_PERSISTENT);

	names = rpmtdNew();
	rpmTagGetNames(names, 1);
	while ((tagname = rpmtdNextString(names))) {
		zend_register_long_constant(tagname, strlen(tagname), rpmTagGetValue(tagname+7), CONST_CS | CONST_PERSISTENT, module_number);
	}
	rpmtdFree(names);

	return SUCCESS;
}
/* }}} */

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

/* {{{ PHP_GINIT_FUNCTION
 */
static PHP_GINIT_FUNCTION(rpminfo) /* {{{ */
{
#if defined(COMPILE_DL_SESSION) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	rpminfo_globals->ts = NULL;
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION
*/
PHP_GSHUTDOWN_FUNCTION(rpminfo)
{
	if (rpminfo_globals->ts) {
		rpmtsFree(rpminfo_globals->ts);
		rpminfo_globals->ts = NULL;
	}
}
/* }}} */

/* {{{ rpminfo_functions[]
 *
 * Every user visible function must have an entry in rpminfo_functions[].
 */
const zend_function_entry rpminfo_functions[] = {
	PHP_FE(rpmdbinfo,         arginfo_rpmdbinfo)
	PHP_FE(rpmdbsearch,       arginfo_rpmdbsearch)
	PHP_FE(rpminfo,           arginfo_rpminfo)
	PHP_FE(rpmvercmp,         arginfo_rpmvercmp)
	PHP_FE_END
};
/* }}} */

/* {{{ rpminfo_module_entry
 */
zend_module_entry rpminfo_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	"rpminfo",
	rpminfo_functions,
	PHP_MINIT(rpminfo),
	NULL,
	PHP_RINIT(rpminfo),
	NULL,
	PHP_MINFO(rpminfo),
	PHP_RPMINFO_VERSION,
	PHP_MODULE_GLOBALS(rpminfo),
	PHP_GINIT(rpminfo),
	PHP_GSHUTDOWN(rpminfo),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
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
