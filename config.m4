dnl config.m4 for extension rpminfo

PHP_ARG_ENABLE(rpminfo, whether to enable rpminfo support,
   [  --enable-rpminfo        Enable rpminfo support])

if test "$PHP_RPMINFO" != "no"; then

  PKG_CHECK_MODULES([LIBRPM], [rpm >= 4.11.3])

  PHP_EVAL_LIBLINE($LIBRPM_LIBS, RPMINFO_SHARED_LIBADD)
  PHP_EVAL_INCLINE($LIBRPM_CFLAGS)

  AC_MSG_CHECKING(for rpm >= 4.13)
  PKG_CHECK_EXISTS([rpm >= 4.13],
    AC_DEFINE(HAVE_ARCHIVE, 1, [ Archive reader since RPM 4.13 ])
    AC_DEFINE(HAVE_WEAKDEP, 1, [ Indexes on weak dependency field since RPM 4.13 ])
    AC_MSG_RESULT([yes]), AC_MSG_RESULT([no])
  )

  PHP_SUBST(RPMINFO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(rpminfo, rpminfo.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
