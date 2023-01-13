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
	#include "ext/spl/spl_iterators.h"
	#include "zend_exceptions.h"
	#include "zend_interfaces.h"

	#include "php_sparsehashmap.h"
}

#include "php_sparsehashmap_object.h"

/* {{{ CLASS POINTER */
zend_class_entry *Sparsehashmap_ce; /* }}} */

/* {{{ HANDLERS AND METHODS */
static zend_object_handlers php_sparsehashmap_handlers;
extern zend_function_entry sparsehashmap_methods[]; /* }}} */
extern zend_object_iterator* php_sparsehashmap_iterator(zend_class_entry *ce, zval *sparsehashmap, int by_ref);

/* {{{
	===================================
			ZEND HANDLER FUNCTIONS
	===================================
}}} */


/* {{{ */
void php_sparsehashmap_free_storage(zend_object *object)
{
    _php_sparsehashmap_t *obj = PHP_SPARSEHASHMAP_FETCH_FROM(object);
    delete static_cast<SparseHashMap *>(obj->shm);
    
    zend_object_std_dtor(object);
} /* }}} */


static HashTable *php_sparsehashmap_gc(zend_object *object, zval **table, int *n) /* {{{ */ {
	_php_sparsehashmap_t *obj = PHP_SPARSEHASHMAP_FETCH_FROM(object);
	HashTable *ht = zend_std_get_properties(&obj->std);
	obj->shm->gc();

	*table = NULL;
	*n = 0;
	return ht;
}


/* {{{ */
zend_object* php_sparsehashmap_create(zend_class_entry *ce) {
	_php_sparsehashmap_t *intern = (_php_sparsehashmap_t*) ecalloc(1, sizeof(_php_sparsehashmap_t));
	zend_object_std_init(&intern->std, ce);

	intern->std.handlers = &php_sparsehashmap_handlers;

	return &intern->std;
}/* }}} */


/* {{{ */
static zend_object* php_sparsehashmap_clone(zend_object *object) {
	_php_sparsehashmap_t *pi = PHP_SPARSEHASHMAP_FETCH_FROM(object);
	_php_sparsehashmap_t *cl = (_php_sparsehashmap_t*) ecalloc(1, sizeof(_php_sparsehashmap_t));
	SparseHashMap *_shm;

	zend_object_std_init(&cl->std, pi->std.ce);

	cl->std.handlers = &php_sparsehashmap_handlers;	

	_shm = new SparseHashMap();
	cl->shm = _shm;
	cl->shm->clone(pi->shm->getThisHashMap());

	return &cl->std;
} /* }}} */


/* {{{ */
static HashTable* php_sparsehashmap_dump(zend_object *object, int *is_temp) {
	_php_sparsehashmap_t *pi = PHP_SPARSEHASHMAP_FETCH_FROM(object);
	HashTable *ht, *props = zend_std_get_properties(&pi->std);
	int _size, _memory;
	zval size, memory;
	zend_string *size_resp, *memory_resp;

	_size = pi->shm->zlength();
	_memory = pi->shm->memory();

	ZVAL_LONG(&size, _size);
	ZVAL_LONG(&memory, _memory);

	*is_temp = 1;
	ht = zend_array_dup(props);
	
	size_resp = zend_string_init("Size of SparseHashMap", strlen("Size of SparseHashMap"), 0);
	memory_resp = zend_string_init("Size of String Memory (bits)", strlen("Size of String Memory (bits)"), 0);

	zend_hash_add_new(ht, size_resp, &size);
	zend_hash_add_new(ht, memory_resp, &memory);
	
	return ht;

} /* }}} */


/* {{{
	=======================================
		ZEND OBJECT HANDLER PROPERTIES
	=======================================
}}} */


/* {{{ */
#define PHP_SPARSEHASHMAP_NO_PROPERTIES() do { \
	zend_throw_exception_ex(NULL, 0, \
		"properties on Indexed objects are not allowed"); \
} while(0)

static zval* php_sparsehashmap_property_read(zend_object *object, zend_string *name, int type, void **cache_slot, zval *rv) {
	PHP_SPARSEHASHMAP_NO_PROPERTIES();
	return &EG(uninitialized_zval);
}

static zval* php_sparsehashmap_property_write(zend_object *object, zend_string *name, zval *value, void **cache_slot) {
	PHP_SPARSEHASHMAP_NO_PROPERTIES();
		return &EG(uninitialized_zval);
}

static int php_sparsehashmap_property_exists(zend_object *object, zend_string *name, int check_empty, void **cache_slot) {
	PHP_SPARSEHASHMAP_NO_PROPERTIES();
	return 0;
}

static void php_sparsehashmap_property_unset(zend_object *object, zend_string *name, void **cache_slot) {
	PHP_SPARSEHASHMAP_NO_PROPERTIES();
} /* }}} */


/* {{{
	=======================================
			SPARSEHASHMAP CLASS INIT
	=======================================
}}} */


/* {{{ */
void php_sparsehashmap_init(void)
{
	zend_class_entry ce;
	const zend_object_handlers *zh;
	
	/* `php_sparsehashmap_*` function callbacks` */

	INIT_CLASS_ENTRY(ce, "Sparsehashmap", sparsehashmap_methods);
	Sparsehashmap_ce = zend_register_internal_class(&ce);
	Sparsehashmap_ce->create_object = php_sparsehashmap_create;					/* Create Class Func OK */
	Sparsehashmap_ce->get_iterator = php_sparsehashmap_iterator;

	zend_class_implements(
		Sparsehashmap_ce, 0);

	zh = zend_get_std_object_handlers();	

	memcpy(&php_sparsehashmap_handlers, zh, sizeof(zend_object_handlers));
	
	php_sparsehashmap_handlers.free_obj = php_sparsehashmap_free_storage; 		/* Class Destructor Func OK */
	php_sparsehashmap_handlers.get_gc   = php_sparsehashmap_gc;					/* Garbage Collection Func OK */
	php_sparsehashmap_handlers.clone_obj = php_sparsehashmap_clone; 			/* Clone Item Func OK */
	php_sparsehashmap_handlers.get_debug_info = php_sparsehashmap_dump;			/* `var_dump()` Func OK */

	php_sparsehashmap_handlers.read_property = php_sparsehashmap_property_read;		
	php_sparsehashmap_handlers.write_property = php_sparsehashmap_property_write;
	php_sparsehashmap_handlers.has_property = php_sparsehashmap_property_exists;
	php_sparsehashmap_handlers.unset_property = php_sparsehashmap_property_unset;

	php_sparsehashmap_handlers.get_properties = NULL;

	php_sparsehashmap_handlers.offset = XtOffsetOf(_php_sparsehashmap_t, std);
} /* }}} */

