/*
  +----------------------------------------------------------------------+
  | PHP Version 8                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2023 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, spease send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: ciaran-moore                                                 |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
	#include "php.h"
	#include "zend_types.h"
	#include "ext/standard/info.h"
	#include "zend_exceptions.h"
	#include "zend_interfaces.h"

	#include "php_sparsehashmap.h"
}

#include "php_sparsehashmap_object.h"



/* {{{
	================================	
			ARGUMENT INFO
	================================
}}} */

/* {{{ */
ZEND_BEGIN_ARG_INFO_EX(SHM_insert_args, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 1)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
ZEND_BEGIN_ARG_INFO_EX(SHM_read_args, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
ZEND_BEGIN_ARG_INFO_EX(SHM_exists_args, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO() /* }}} */

/* {{{ */
ZEND_BEGIN_ARG_INFO_EX(SHM_remove_args, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO() /* }}} */
/* {{{ */

/* {{{ */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(SHM_length_args, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO(); /* }}} */

/* {{{ */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(SHM_memory_args, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO(); /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(SHM_to_array_args, 0, 0, IS_ARRAY, 1)
ZEND_END_ARG_INFO();

/* {{{ */
ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO(); /* }}} */

/* {{{
	================================	
			ARGUMENT METHODS
	================================
}}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, __construct)
{
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());
	intern->shm = new SparseHashMap();

} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, insert)
{
	zend_string *key, *value;
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &key, &value) != SUCCESS) {
		return;
	}

	intern->shm->insert(
		std::string_view(ZSTR_VAL(key), ZSTR_LEN(key)),
		std::string_view(ZSTR_VAL(value), ZSTR_LEN(value))
	);

} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, read)
{
	zend_string *key;

	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &key) != SUCCESS) {
		return;
	}

	std::string_view key_view(ZSTR_VAL(key), ZSTR_LEN(key));
	std::string resp = intern->shm->read(key_view);

	RETURN_STRINGL(resp.data(), resp.size());
	
} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, exists)
{
	zend_string *key;
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &key) != SUCCESS) {
		return;
	}

	std::string_view key_view(ZSTR_VAL(key), ZSTR_LEN(key));

	bool response = intern->shm->exists(key_view);

	RETURN_BOOL(response);
} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, remove)
{
	zend_string *key;
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &key) != SUCCESS) {
		return;
	}

	std::string_view key_view(ZSTR_VAL(key), ZSTR_LEN(key));
	intern->shm->remove(key_view);

} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, length)
{
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	auto length = intern->shm->length();

	RETURN_LONG(length);
} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, memory)
{
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	auto memory = intern->shm->memory();

	RETURN_LONG(memory);
} /* }}} */

/* {{{ */
PHP_METHOD(Sparsehashmap, flush)
{
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	intern->shm->flush();
	intern->shm->gc();
	
} /* }}} */


/* {{{ */
PHP_METHOD(Sparsehashmap, to_array)
{
	HashTable *array;
	php_sparsehashmap_t *intern = php_sparsehashmap_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	array = intern->shm->iterated_object();

	RETURN_ARR(array);
	
} /* }}} */


/* {{{
	===================================
			ZEND METHOD ENTRIES
	===================================
}}} */


/* {{{ */
extern const zend_function_entry sparsehashmap_methods[] = {
	PHP_ME(Sparsehashmap, __construct, arginfo_none, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Sparsehashmap, insert, SHM_insert_args, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, read, SHM_read_args, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, exists, SHM_exists_args, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, remove, SHM_remove_args, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, length, SHM_length_args, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, memory, SHM_memory_args, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, flush, arginfo_none, ZEND_ACC_PUBLIC)
	PHP_ME(Sparsehashmap, to_array, SHM_to_array_args, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


/* {{{
	=========================================
			SPARSEHASHMAP MODULE SETUP
	=========================================
}}} */

/* {{{ PHP_MINIT_FUNCTION 
*/
PHP_MINIT_FUNCTION(sparsehashmap)
{
	php_sparsehashmap_init();

	return SUCCESS;
} /* }}} */


/* {{{ PHP_RINIT_FUNCTION
*/


PHP_RINIT_FUNCTION(sparsehashmap)
{
	#if defined(COMPILE_DL_SPARSEHASHMAP) && defined(ZTS)
		ZEND_TSRMLS_CACHE_UPDATE();
	#endif

	return SUCCESS;
}

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(sparsehashmap)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "sparsehashmap support", "enabled");
	php_info_print_table_end();
} /* }}} */


/* sparsehashmap_module_entry
*/
zend_module_entry sparsehashmap_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_SPARSEHASHMAP_EXTNAME,
	NULL,
	PHP_MINIT(sparsehashmap),
	NULL,
	PHP_RINIT(sparsehashmap),
	NULL,
	PHP_MINFO(sparsehashmap),
	PHP_SPARSEHASHMAP_EXTVER,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */


#ifdef COMPILE_DL_SPARSEHASHMAP
#ifdef ZTS
	ZEND_TSRMLS_CACHE_DEFINE();
#endif
	ZEND_GET_MODULE(sparsehashmap)
#endif	
