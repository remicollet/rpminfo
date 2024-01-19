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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include <fcntl.h>
#include <rpm/rpmdb.h>
#include <rpm/rpmio.h>
#include <rpm/rpmlib.h>
#include <rpm/rpmts.h>

#include "php_rpminfo.h"

#include "rpminfo_arginfo.h"

#ifdef HAVE_ARCHIVE
struct php_rpm_stream_data_t {
	FD_t        gzdi;
	Header      h;
	rpmfiles    files;
	rpmfi       fi;
	php_stream *stream;
};

#define STREAM_DATA_FROM_STREAM() \
	struct php_rpm_stream_data_t *self = (struct php_rpm_stream_data_t *) stream->abstract;
#endif

ZEND_DECLARE_MODULE_GLOBALS(rpminfo)

static rpmts rpminfo_getts(void) {
	if (!RPMINFO_G(ts)) {
		rpmReadConfigFiles(NULL, NULL);
		RPMINFO_G(ts) = rpmtsCreate();
	}
	if (RPMINFO_G(ts)) {
		(void)rpmtsSetVSFlags(RPMINFO_G(ts), _RPMVSF_NODIGESTS | _RPMVSF_NOSIGNATURES | RPMVSF_NOHDRCHK);
	}
	return RPMINFO_G(ts);
}

static rpmdb rpminfo_getdb(void) {
	if (!RPMINFO_G(db)) {
		rpmts ts = rpminfo_getts();

		rpmtsOpenDB(ts, O_RDONLY);
		RPMINFO_G(db) = rpmtsGetRdb(ts);
	}
	return RPMINFO_G(db);
}

static void rpm_header_to_zval(zval *return_value, Header h, zend_bool full)
{
	HeaderIterator hi;
	rpmTagVal tag;
	rpmTagType type;
	const char *val;
	int i;

	array_init(return_value);
	hi = headerInitIterator(h);
	while ((tag=headerNextTag(hi)) != RPMTAG_NOT_FOUND) {
		if (!full) {
			switch (tag) {
				case RPMTAG_NAME:
				case RPMTAG_VERSION:
				case RPMTAG_RELEASE:
				case RPMTAG_EPOCH:
				case RPMTAG_ARCH:
				case RPMTAG_SUMMARY:
					/* Always present tags */
					break;
				default:
					/* Additional tags */
					for (i=0 ; i<RPMINFO_G(nb_tags) ; i++) {
						if (tag == RPMINFO_G(tags)[i]) {
							break;
						}
					}
					if (i==RPMINFO_G(nb_tags)) {
						continue;
					}
			}
		}

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
	rpmts ts = rpminfo_getts();

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "p|bz", &path, &len, &full, &error) == FAILURE) {
		RETURN_THROWS();
	}
	if (error) {
		ZVAL_DEREF(error);
		zval_dtor(error);
		ZVAL_NULL(error);
	}
	if (php_check_open_basedir(path)) {
		RETURN_NULL();
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
	RETURN_NULL();
}
/* }}} */

/* {{{ proto array rpmdbinfo(string nevr [, bool full])
   Retrieve information from an installed RPM */
PHP_FUNCTION(rpmdbinfo)
{
	char *name;
	size_t len;
	zend_bool full = 0;
	Header h;
	rpmdb db;
	rpmdbMatchIterator di;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "p|b", &name, &len, &full) == FAILURE) {
		RETURN_THROWS();
	}

	db = rpminfo_getdb();
	di = rpmdbInitIterator(db, RPMDBI_LABEL, name, len);
	if (!di) {
		/* Not found */
		RETURN_NULL();
	}

	array_init(return_value);
	while ((h = rpmdbNextIterator(di)) != NULL) {
		zval tmp;
		rpm_header_to_zval(&tmp, h, full);
		add_next_index_zval(return_value, &tmp);
	}

	rpmdbFreeIterator(di);
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

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|llb", &name, &len, &crit, &mode, &full) == FAILURE) {
		RETURN_THROWS();
	}
	if (rpmTagGetType(crit) == RPM_NULL_TYPE) {
		zend_argument_value_error(2, "Unkown rpmtag");
		RETURN_THROWS();
	}
	if (mode != RPMMIRE_DEFAULT &&
		mode != RPMMIRE_STRCMP &&
		mode != RPMMIRE_REGEX &&
		mode != RPMMIRE_GLOB &&
		mode != -1) {
		zend_argument_value_error(3, "Unkown rpmmire");
		RETURN_THROWS();
	}

	if (crit == RPMTAG_PKGID) {
		if (len != 32) {
			zend_argument_value_error(1, "Bad length for PKGID, 32 expected");
			RETURN_THROWS();
		}
		len = hex2bin(name, MD5, len);
		name = MD5;
	} else if (crit == RPMTAG_HDRID) {
		if (len != 40) {
			zend_argument_value_error(1, "Bad length for HDRID, 40 expected");
			RETURN_THROWS();
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

	db = rpminfo_getdb();
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
				RETURN_NULL();
			}
		}
	}
	if (!di) {
		/* Not found */
		RETURN_NULL();
	}

	array_init_size(return_value, rpmdbGetIteratorCount(di));
	while ((h = rpmdbNextIterator(di)) != NULL) {
		zval tmp;
		rpm_header_to_zval(&tmp, h, full);
		add_next_index_zval(return_value, &tmp);
	}

	rpmdbFreeIterator(di);
}
/* }}} */

/* {{{ proto int rpmcmpver(string evr1, string evr2)
   Compare 2 RPM EVRs (epoch:version-release) strings */
PHP_FUNCTION(rpmvercmp)
{
	char *in_evr1, *evr1, *v1, *r1;
	char *in_evr2, *evr2, *v2, *r2;
	char *p, empty[] = "";
	char *op = NULL;
	long e1, e2, r;
	size_t len1, len2, oplen;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|s", &in_evr1, &len1, &in_evr2, &len2, &op, &oplen) == FAILURE) {
		RETURN_THROWS();
	}
	evr1 = estrdup(in_evr1);
	evr2 = estrdup(in_evr2);

	/* Epoch */
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
		r = -1;
	} else if (e1 > e2) {
		r = 1;
	} else {
		/* Version */
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
		if (!r) {
			/* Release*/
			r = rpmvercmp(r1, r2);
		}
	}
	efree(evr1);
	efree(evr2);

	if (!op) {
		RETURN_LONG(r);
	}

	if (!strcmp(op, "<") || !strcmp(op, "lt")) {
		RETURN_BOOL(r < 0);
	}
	if (!strcmp(op, "<=") || !strcmp(op, "le")) {
		RETURN_BOOL(r <= 0);
	}
	if (!strcmp(op, ">") || !strcmp(op, "gt")) {
		RETURN_BOOL(r > 0);
	}
	if (!strcmp(op, ">=") || !strcmp(op, "ge")) {
		RETURN_BOOL(r >= 0);
	}
	if (!strcmp(op, "==") || !strcmp(op, "=") || !strcmp(op, "eq")) {
		RETURN_BOOL(r == 0);
	}
	if (!strcmp(op, "!=") || !strcmp(op, "<>") || !strcmp(op, "ne")) {
		RETURN_BOOL(r != 0);
	}

	zend_argument_value_error(3, "must be a valid comparison operator");
	RETURN_THROWS();
}
/* }}} */

/* {{{ proto int rpmaddtag(int tag)
   add a tag in the default set */
PHP_FUNCTION(rpmaddtag)
{
	int i;
	zend_long tag;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &tag) == FAILURE) {
		RETURN_THROWS();
	}

	if (rpmTagGetType(tag) == RPM_NULL_TYPE) {
		zend_argument_value_error(1, "Unkown rpmtag");
		RETURN_THROWS();
	}

	if (RPMINFO_G(tags)) {
		for (i=0 ; i<RPMINFO_G(nb_tags) ; i++) {
			if (RPMINFO_G(tags)[i] == tag) {
				RETURN_BOOL(0);
			}
		}
		if (RPMINFO_G(nb_tags) == RPMINFO_G(max_tags)) {
			RPMINFO_G(max_tags) += 16;
			RPMINFO_G(tags) = erealloc(RPMINFO_G(tags), RPMINFO_G(max_tags) * sizeof(rpmTagVal));
		}
	} else {
		RPMINFO_G(max_tags) = 16;
		RPMINFO_G(tags) = emalloc(RPMINFO_G(max_tags) * sizeof(rpmTagVal));
	}
	RPMINFO_G(tags)[RPMINFO_G(nb_tags)++] = tag;

	RETURN_BOOL(1);
}
/* }}} */

#ifdef HAVE_ARCHIVE
static ssize_t php_rpm_ops_read(php_stream *stream, char *buf, size_t count)
{
	ssize_t n = -1;
	STREAM_DATA_FROM_STREAM();

	if (self) {
		n = rpmfiArchiveRead(self->fi, buf, count);
		if (n == 0 || n < (ssize_t)count) {
			stream->eof = 1;
		}
	}
	return n;
}

static void php_rpm_ops_free(struct php_rpm_stream_data_t *self, int close_handle)
{
	if (self) {
		if (close_handle) {
			Fclose(self->gzdi);
			rpmfilesFree(self->files);
			rpmfiFree(self->fi);
			headerFree(self->h);
		}
		efree(self);
	}
}

static int php_rpm_ops_close(php_stream *stream, int close_handle)
{
	STREAM_DATA_FROM_STREAM();

	php_rpm_ops_free(self, close_handle);
	stream->abstract = NULL;

	return EOF;
}

static int php_zip_ops_stat(php_stream *stream, php_stream_statbuf *ssb)
{
	STREAM_DATA_FROM_STREAM();

	if (self) {
		struct stat s[2]; /* librpm may use different size (32-bit) */
		int rc;
		rc = rpmfiStat(self->fi, 0, s);
		memcpy(&ssb->sb, s, sizeof(ssb->sb));
		return rc;
	}
	return -1;
}

const php_stream_ops php_stream_rpmio_ops = {
	NULL, /* write */
	php_rpm_ops_read,
	php_rpm_ops_close,
	NULL, /* flush */
	"rpm",
	NULL, /* seek */
	NULL, /* cast */
	php_zip_ops_stat,
	NULL  /* set_option */
};

static struct php_rpm_stream_data_t *php_stream_rpm_finder(const char *path, int want_content)
{
	size_t path_len;
	zend_string *file_basename;
	char file_dirname[MAXPATHLEN];
	char *fragment;
	size_t fragment_len;
	struct php_rpm_stream_data_t *self = NULL;
	FD_t fdi;
	FD_t gzdi;
	int rc;
	Header h;
	char rpmio_flags[80];
	const char *compr;
	rpmfiles files;
	rpmfi fi;
	rpmts ts = rpminfo_getts();

	fragment = strchr(path, '#');
	if (!fragment) {
		return NULL;
	}
	if (strncasecmp("rpm://", path, 6) == 0) {
		path += 6;
	}
	fragment_len = strlen(fragment);
	if (fragment_len < 1) {
		return NULL;
	}
	path_len = strlen(path);
	if (path_len >= MAXPATHLEN) {
		return NULL;
	}
	memcpy(file_dirname, path, path_len - fragment_len);
	file_dirname[path_len - fragment_len] = '\0';
	file_basename = php_basename(path, path_len - fragment_len, NULL, 0);
	fragment++;
	if (php_check_open_basedir(file_dirname)) {
		zend_string_release_ex(file_basename, 0);
		return NULL;
	}
	fdi = Fopen(file_dirname, "r.ufdio");
	if (Ferror(fdi)) {
		zend_string_release_ex(file_basename, 0);
		return NULL;
	}
	rc = rpmReadPackageFile(ts, fdi, "rpm2cpio", &h);
	if (rc != RPMRC_OK && rc != RPMRC_NOKEY && rc != RPMRC_NOTTRUSTED) {
		zend_string_release_ex(file_basename, 0);
		Fclose(fdi);
		return NULL;
	}

	compr = headerGetString(h, RPMTAG_PAYLOADCOMPRESSOR);
	snprintf(rpmio_flags, sizeof(rpmio_flags), "r.%s", compr ? compr : "gzip");
	gzdi = Fdopen(fdi, rpmio_flags);
	if (gzdi == NULL) {
		headerFree(h);
		Fclose(fdi);
		zend_string_release_ex(file_basename, 0);
		return NULL;
	}

	files = rpmfilesNew(NULL, h, 0, RPMFI_KEEPHEADER);
	fi = rpmfiNewArchiveReader(gzdi, files, RPMFI_ITER_READ_ARCHIVE);

	while((rc = rpmfiNext(fi)) >=0) {
		const char *fn = rpmfiFN(fi);
		/*
		printf("Name=%s, Size=%d, N=%d, mode=%d, reg=%d, content=%d, rdev=%d, inode=%d link=%s\n", fn,
			(int)rpmfiFSize(fi), (int)rpmfiFNlink(fi), (int)rpmfiFMode(fi),
			(int)S_ISREG(rpmfiFMode(fi)), (int)rpmfiArchiveHasContent(fi),
			(int)rpmfiFRdev(fi), (int)rpmfiFInode(fi), rpmfiFLink(fi));
		*/
		if (!strcmp(fn, fragment)) {
			if (want_content && S_ISREG(rpmfiFMode(fi)) && !rpmfiArchiveHasContent(fi)) {
				rpm_rdev_t rdev = rpmfiFRdev(fi);
				rpm_ino_t  inode = rpmfiFInode(fi);
				while((rc = rpmfiNext(fi)) >=0) {
					if (rdev == rpmfiFRdev(fi) && inode == rpmfiFInode(fi) && rpmfiArchiveHasContent(fi)) {
						break;
					}
				}
			}
			break;
		}
	}
	if (rc == RPMERR_ITER_END) {
		Fclose(gzdi);
		rpmfilesFree(files);
		rpmfiFree(fi);
		headerFree(h);
	} else {
		self = emalloc(sizeof(*self));
		self->gzdi   = gzdi;
		self->files  = files;
		self->fi     = fi;
		self->h      = h;
	}
	zend_string_release_ex(file_basename, 0);

	return self;
}

php_stream *php_stream_rpm_opener(php_stream_wrapper *wrapper,
											const char *path,
											const char *mode,
											int options,
											zend_string **opened_path,
											php_stream_context *context STREAMS_DC)
{
	struct php_rpm_stream_data_t *self;

	if (mode[0] != 'r') {
		return NULL;
	}
	self = php_stream_rpm_finder(path, 1);
	if (self) {
		if (opened_path) {
			*opened_path = zend_string_init(path, strlen(path), 0);
		}
		if (!S_ISREG(rpmfiFMode(self->fi)) || !rpmfiArchiveHasContent(self->fi)) {
			php_rpm_ops_free(self, 1);
		} else {
			return php_stream_alloc(&php_stream_rpmio_ops, self, NULL, mode);
		}
	}

	return NULL;
}

static int php_stream_rpm_stat(php_stream_wrapper *wrapper, const char *url, int flags,
								 php_stream_statbuf *ssb, php_stream_context *context)
{
	struct php_rpm_stream_data_t *self;
	int rc = -1;

	self = php_stream_rpm_finder(url, 0);
	if (self) {
		struct stat s[2]; /* librpm may use different size (32-bit) */
		rc = rpmfiStat(self->fi, 0, s);
		memcpy(&ssb->sb, s, sizeof(ssb->sb));
		php_rpm_ops_free(self, 1);
	}

	return rc;
}

static const php_stream_wrapper_ops rpm_stream_wops = {
	php_stream_rpm_opener,
	NULL,	/* close */
	NULL,	/* fstat */
	php_stream_rpm_stat,
	NULL,	/* opendir */
	"RPM wrapper",
	NULL,	/* unlink */
	NULL,	/* rename */
	NULL,	/* mkdir */
	NULL,	/* rmdir */
	NULL	/* metadata */
};

const php_stream_wrapper php_stream_rpm_wrapper = {
	&rpm_stream_wops,
	NULL,
	0 /* is_url */
};

/* {{{ proto array rpmgetsymlink(string path , string name)
   Retrieve soft link target of en entry */
PHP_FUNCTION(rpmgetsymlink)
{
	char *path, *name;
	const char *link;
	size_t plen, nlen;
	FD_t fdi;
	FD_t gzdi;
	int rc;
	Header h;
	char rpmio_flags[80];
	const char *compr;
	rpmfiles files;
	rpmfi fi;
	rpmts ts = rpminfo_getts();

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "pp", &path, &plen, &name, &nlen) == FAILURE) {
		RETURN_THROWS();
	}

	if (php_check_open_basedir(path)) {
		RETURN_NULL();
	}
	fdi = Fopen(path, "r.ufdio");
	if (Ferror(fdi)) {
		RETURN_NULL();
	}
	rc = rpmReadPackageFile(ts, fdi, "rpm2cpio", &h);
	if (rc != RPMRC_OK && rc != RPMRC_NOKEY && rc != RPMRC_NOTTRUSTED) {
		Fclose(fdi);
		RETURN_NULL();
	}

	compr = headerGetString(h, RPMTAG_PAYLOADCOMPRESSOR);
	snprintf(rpmio_flags, sizeof(rpmio_flags), "r.%s", compr ? compr : "gzip");
	gzdi = Fdopen(fdi, rpmio_flags);
	if (gzdi == NULL) {
		headerFree(h);
		Fclose(fdi);
		RETURN_NULL();
	}

	files = rpmfilesNew(NULL, h, 0, RPMFI_KEEPHEADER);
	fi = rpmfiNewArchiveReader(gzdi, files, RPMFI_ITER_READ_ARCHIVE);

	rc = rpmfiFindFN(fi, name);
	rpmfiSetFX(fi, rc); /* return value have change in 4.18 (from previous to new) */
	if (rc < 0
        || rpmfiFX(fi) != rc
		|| (link = rpmfiFLink(fi)) == NULL) {
		RETVAL_NULL();
	} else {
		RETVAL_STRING(link);
	}
	rpmfiFree(fi);
	rpmfilesFree(files);
	headerFree(h);
	Fclose(gzdi);
}
/* }}} */
#endif /* HAVE_ARCHIVE */


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

#ifdef HAVE_ARCHIVE
	php_register_url_stream_wrapper("rpm", &php_stream_rpm_wrapper);
#endif

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

	RPMINFO_G(nb_tags) = 0;
	RPMINFO_G(max_tags) = 0;
	RPMINFO_G(tags) = NULL;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(rpminfo)
{
	if (RPMINFO_G(ts)) {
		if (RPMINFO_G(db)) {
			rpmtsCloseDB(RPMINFO_G(ts));
			RPMINFO_G(db) = NULL;
		}
		rpmtsFree(RPMINFO_G(ts));
		RPMINFO_G(ts) = NULL;
	}

	if (RPMINFO_G(tags)) {
		efree(RPMINFO_G(tags));
		RPMINFO_G(nb_tags) = 0;
		RPMINFO_G(max_tags) = 0;
		RPMINFO_G(tags) = NULL;
	}

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
	php_info_print_table_row(2, "Author", PHP_RPMINFO_AUTHOR);
	php_info_print_table_row(2, "License", PHP_RPMINFO_LICENSE);
	php_info_print_table_row(2, "RPM library version", RPMVERSION);
#ifdef HAVE_ARCHIVE
	php_info_print_table_row(2, "RPM stream wrapper", "yes");
#else
	php_info_print_table_row(2, "RPM stream wrapper", "no");
#endif
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
	rpminfo_globals->db = NULL;
}
/* }}} */

/* {{{ rpminfo_module_entry
 */
zend_module_entry rpminfo_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	"rpminfo",
	ext_functions,
	PHP_MINIT(rpminfo),
	NULL,
	PHP_RINIT(rpminfo),
	PHP_RSHUTDOWN(rpminfo),
	PHP_MINFO(rpminfo),
	PHP_RPMINFO_VERSION,
	PHP_MODULE_GLOBALS(rpminfo),
	PHP_GINIT(rpminfo),
	NULL,
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
