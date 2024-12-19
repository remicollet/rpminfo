/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: dc980a56084190700162f5a8c70b54fcdbc30ced */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmaddtag, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, rpmtag, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmdbinfo, 0, 1, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, nevr, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, full, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmdbsearch, 0, 1, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, rpmtag, IS_LONG, 0, "RPMTAG_NAME")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, rpmmire, IS_LONG, 0, "-1")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, full, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpminfo, 0, 1, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, full, _IS_BOOL, 0, "false")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(1, error, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_rpmvercmp, 0, 2, MAY_BE_LONG|MAY_BE_BOOL)
	ZEND_ARG_TYPE_INFO(0, evr1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, evr2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, operator, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmgetsymlink, 0, 2, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmexpand, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmexpandnumeric, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_rpmdefine, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, macro, IS_STRING, 0)
ZEND_END_ARG_INFO()


ZEND_FUNCTION(rpmaddtag);
ZEND_FUNCTION(rpmdbinfo);
ZEND_FUNCTION(rpmdbsearch);
ZEND_FUNCTION(rpminfo);
ZEND_FUNCTION(rpmvercmp);
ZEND_FUNCTION(rpmgetsymlink);
ZEND_FUNCTION(rpmexpand);
ZEND_FUNCTION(rpmexpandnumeric);
ZEND_FUNCTION(rpmdefine);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(rpmaddtag, arginfo_rpmaddtag)
	ZEND_FE(rpmdbinfo, arginfo_rpmdbinfo)
	ZEND_FE(rpmdbsearch, arginfo_rpmdbsearch)
	ZEND_FE(rpminfo, arginfo_rpminfo)
	ZEND_FE(rpmvercmp, arginfo_rpmvercmp)
	ZEND_FE(rpmgetsymlink, arginfo_rpmgetsymlink)
	ZEND_FE(rpmexpand, arginfo_rpmexpand)
	ZEND_FE(rpmexpandnumeric, arginfo_rpmexpandnumeric)
	ZEND_FE(rpmdefine, arginfo_rpmdefine)
	ZEND_FE_END
};
