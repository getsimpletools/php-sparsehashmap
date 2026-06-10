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
extern zend_object_iterator* php_sparsehashmap_iterator(zend_class_entry *ce, zval *sparsehashmap, int by_ref);

/* {{{
	===================================
			ZEND HANDLER FUNCTIONS
	===================================
}}} */


/* {{{ */
void php_sparsehashmap_free_storage(zend_object *object)
{
    php_sparsehashmap_t *obj = php_sparsehashmap_fetch_object(object);
    delete obj->shm;
    obj->shm = nullptr;
    
    zend_object_std_dtor(object);
} /* }}} */


static HashTable *php_sparsehashmap_gc(zend_object *object, zval **table, int *n) /* {{{ */ {
	php_sparsehashmap_t *obj = php_sparsehashmap_fetch_object(object);
	HashTable *ht = zend_std_get_properties(&obj->std);

	if(obj->shm)
		obj->shm->gc();

	*table = nullptr;
	*n = 0;
	return ht;
}


/* {{{ */
zend_object* php_sparsehashmap_create(zend_class_entry *ce) {
	php_sparsehashmap_t *intern = (php_sparsehashmap_t*) ecalloc(1, sizeof(php_sparsehashmap_t));
	zend_object_std_init(&intern->std, ce);

	intern->std.handlers = &php_sparsehashmap_handlers;

	return &intern->std;
}/* }}} */


/* {{{ */
static zend_object* php_sparsehashmap_clone(zend_object *object) {
php_sparsehashmap_t *pi = php_sparsehashmap_fetch_object(object);
	
	// 1. Delegate allocation to your official class factory handler
	zend_object *new_obj = pi->std.ce->create_object(pi->std.ce);
	php_sparsehashmap_t *cl = php_sparsehashmap_fetch_object(new_obj);

	// 2. Mirror properties, attributes, and internal Zend flags safely
	zend_objects_clone_members(&cl->std, &pi->std);

	// 3. Duplicate the backing map using the C++ copy constructor
	if (pi->shm) {
		cl->shm = new SparseHashMap(*pi->shm);
	}

	return new_obj;
} /* }}} */


/* {{{ */
static HashTable* php_sparsehashmap_dump(zend_object *object, int *is_temp) {
    php_sparsehashmap_t *pi = php_sparsehashmap_fetch_object(object);
    
    if (!pi->shm) {
        // Handle null case gracefully
        *is_temp = 1;
        return zend_array_dup(zend_std_get_properties(&pi->std));
    }

    int _size = pi->shm->zlength();
    int _memory = pi->shm->memory();

    zval size, memory;
    ZVAL_LONG(&size, _size);
    ZVAL_LONG(&memory, _memory);

    *is_temp = 1;
    HashTable *ht = zend_array_dup(zend_std_get_properties(&pi->std));

    zend_string *key_size   = zend_string_init("size", sizeof("size")-1, 0);
    zend_string *key_memory = zend_string_init("memory_usage", sizeof("memory_usage")-1, 0);

    zend_hash_add_new(ht, key_size, &size);
    zend_hash_add_new(ht, key_memory, &memory);

    zend_string_release(key_size);
    zend_string_release(key_memory);

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
		"properties on Sparsehashmap objects are not allowed"); \
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


	zh = zend_get_std_object_handlers();	

	php_sparsehashmap_handlers = *zh;
	
	php_sparsehashmap_handlers.free_obj = php_sparsehashmap_free_storage; 		/* Class Destructor Func OK */
	php_sparsehashmap_handlers.get_gc   = php_sparsehashmap_gc;					/* Garbage Collection Func OK */
	php_sparsehashmap_handlers.clone_obj = php_sparsehashmap_clone; 			/* Clone Item Func OK */
	php_sparsehashmap_handlers.get_debug_info = php_sparsehashmap_dump;			/* `var_dump()` Func OK */

	php_sparsehashmap_handlers.read_property = php_sparsehashmap_property_read;		
	php_sparsehashmap_handlers.write_property = php_sparsehashmap_property_write;
	php_sparsehashmap_handlers.has_property = php_sparsehashmap_property_exists;
	php_sparsehashmap_handlers.unset_property = php_sparsehashmap_property_unset;

	php_sparsehashmap_handlers.get_properties = nullptr;

	php_sparsehashmap_handlers.offset = XtOffsetOf(php_sparsehashmap_t, std);
} /* }}} */

