PHP_ARG_ENABLE(sparsehashmap,
  whether to enable sparsehashmap support,
  [ --enable-sparsehashmap     Enable sparsehashmap support])

if test "$PHP_SPARSEHASHMAP" != "no"; then

  PHP_REQUIRE_CXX()

  PHP_CXX_COMPILE_STDCXX(17, mandatory, PHP_SPARSEHASHMAP_STDCXX)

  SPARSEHASHMAP_COMMON_FLAGS="-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1"

  PHP_NEW_EXTENSION(
    sparsehashmap,
    php-sparsehashmap.cc \
    php-sparsehashmap_object.cc \
    iterator.cc \
    sparsehashmap.cpp,
    $ext_shared,
    ,
    "$SPARSEHASHMAP_COMMON_FLAGS $PHP_SPARSEHASHMAP_STDCXX",
    cxx
  )
fi