dnl config.m4 for extension rpminfo

PHP_ARG_ENABLE(rpminfo, whether to enable rpminfo support,
   [  --enable-rpminfo        Enable rpminfo support])

if test "$PHP_RPMINFO" != "no"; then
  dnl Write more examples of tests here...

  AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

  AC_MSG_CHECKING(for librpm)
  if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists rpm; then
    if $PKG_CONFIG rpm --atleast-version 4.11.3; then
      LIBRPM_CFLAGS=`$PKG_CONFIG rpm --cflags`
      LIBRPM_LIBDIR=`$PKG_CONFIG rpm --libs`
      LIBRPM_VERSON=`$PKG_CONFIG rpm --modversion`
      AC_MSG_RESULT(from pkgconfig: version $LIBRPM_VERSON)
      if $PKG_CONFIG rpm --atleast-version 4.13; then
        AC_DEFINE(HAVE_ARCHIVE, 1, [ Archive reader since RPM 4.13 ])
        AC_DEFINE(HAVE_WEAKDEP, 1, [ Indexes on weak dependency field since RPM 4.13 ])
      fi
    else
      AC_MSG_ERROR(system librpm is too old: version 4.11.3 required)
    fi
  else
    AC_MSG_ERROR(pkg-config not found)
  fi
  PHP_EVAL_LIBLINE($LIBRPM_LIBDIR, RPMINFO_SHARED_LIBADD)
  PHP_EVAL_INCLINE($LIBRPM_CFLAGS)

  PHP_SUBST(RPMINFO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(rpminfo, rpminfo.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
