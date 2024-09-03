dnl config.m4 for extension rpminfo

PHP_ARG_ENABLE(rpminfo, whether to enable rpminfo support,
   [  --enable-rpminfo        Enable rpminfo support])

if test "$PHP_RPMINFO" != "no"; then

  PKG_CHECK_MODULES([LIBRPM], [rpm >= 4.13])

  PHP_EVAL_LIBLINE($LIBRPM_LIBS, RPMINFO_SHARED_LIBADD)
  PHP_EVAL_INCLINE($LIBRPM_CFLAGS)

  PHP_SUBST(RPMINFO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(rpminfo, rpminfo.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
