PHP_ARG_ENABLE(sparsehashmap, whether to enable sparsehashmap support,
[  --enable-sparsehashmap           Enable sparsehashmap support])

if test "$PHP_INDEXED" != "no"; then

  	SPARSEHASHMAP_COMMON_FLAGS="-Wno-write-strings -D__STDC_LIMIT_MACROS -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1"
  	PHP_NEW_EXTENSION(sparsehashmap, php-sparsehashmap.cc php-sparsehashmap_object.cc iterator.cc, $ext_shared,,$SPARSEHASHMAP_COMMON_FLAGS,cxx)

  	PHP_SPARSEHASHMAP_CXX_SOURCES="sparsehashmap.cpp"
  	PHP_REQUIRE_CXX()
  	PHP_CXX_COMPILE_STDCXX(11, mandatory, PHP_SPARSEHASHMAP_STDCXX)
  	PHP_SPARSEHASHMAP_CXX_FLAGS="$SPARSEHASHMAP_COMMON_FLAGS $PHP_SPARSEHASHMAP_STDCXX"

	if test "$ext_shared" = "no"; then
	  PHP_ADD_SOURCES(PHP_EXT_DIR(sparsehashmap), $PHP_SPARSEHASHMAP_CXX_SOURCES, $PHP_SPARSEHASHMAP_CXX_FLAGS)
	else
	  PHP_ADD_SOURCES_X(PHP_EXT_DIR(sparsehashmap), $PHP_SPARSEHASHMAP_CXX_SOURCES, $PHP_SPARSEHASHMAP_CXX_FLAGS, shared_objects_sparsehashmap, yes)
	fi
fi