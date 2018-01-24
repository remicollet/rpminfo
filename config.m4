dnl config.m4 for extension rpminfo

PHP_ARG_ENABLE(rpminfo, whether to enable rpminfo support,
   [  --enable-rpminfo        Enable rpminfo support])

if test "$PHP_RPMINFO" != "no"; then
  dnl Write more examples of tests here...

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

  AC_MSG_CHECKING(for librpm)
  if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists rpm; then
    if $PKG_CONFIG rpm --atleast-version 4; then
      LIBRPM_CFLAGS=`$PKG_CONFIG rpm --cflags`
      LIBRPM_LIBDIR=`$PKG_CONFIG rpm --libs`
      LIBRPM_VERSON=`$PKG_CONFIG rpm --modversion`
      AC_MSG_RESULT(from pkgconfig: version $LIBRPM_VERSON)
    else
      AC_MSG_ERROR(system librpm is too old)
    fi
  else
    AC_MSG_ERROR(pkg-config not found)
  fi
  PHP_EVAL_LIBLINE($LIBRPM_LIBDIR, RPMINFO_SHARED_LIBADD)
  PHP_EVAL_INCLINE($LIBRPM_CFLAGS)

  PHP_SUBST(RPMINFO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(rpminfo, rpminfo.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
