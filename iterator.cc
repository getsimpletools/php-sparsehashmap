/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2022 The PHP Group                                |
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

/* $Id$ */

/* {{{ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sparsehashmap_object.h"

/* {{{ */
typedef struct _php_sparsehashmap_iterator_t {
	zend_object_iterator it;
	zval sparsehashmap;
	zval zdata;
	zend_long pos;
	zend_long shm_size;
} php_sparsehashmap_iterator_t; /* }}} */

/* {{{ */
static void php_sparsehashmap_iterator_dtor(php_sparsehashmap_iterator_t *iterator) {
	// Initialise Iterator Object
	php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
	// Free the current sparsehashmap class zval
	zval_ptr_dtor(&io->sparsehashmap);
	// Free the current data from the data zval
	zval_ptr_dtor(&io->zdata);

}

static int php_sparsehashmap_iterator_validate(php_sparsehashmap_iterator_t *iterator) {
	// Initialise Iterator Object
	php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;

	// check that the current position is not greater than the size of
	// the SparseHashMap Iterator.
	return (io->pos < (io->shm_size -1)) ? SUCCESS: FAILURE;
}

static zval* php_sparsehashmap_iterator_current_data(php_sparsehashmap_iterator_t *iterator) {
	// Initialise Iterator Object
	php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;

	// Return address to the current zdata in the Iterator Object
	return &io->zdata;
}

static void php_sparsehashmap_iterator_current_key(php_sparsehashmap_iterator_t *iterator, zval *key) {
	// Initialise Iterator Object
	php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
	zend_string *_key;

	// Retrieve the current key from the SparseHashMap Iterator
	_php_sparsehashmap_t *sp = PHP_SPARSEHASHMAP_FETCH_FROM(Z_OBJ(io->sparsehashmap));
	std::string current_key = sp->shm->current_key();
	_key = zend_string_init(current_key.c_str(), strlen(current_key.c_str()), 0);

	// Assign the current key to the `key` variable
	ZVAL_STR(key, _key);
}

static void php_sparsehashmap_iterator_move_forward(php_sparsehashmap_iterator_t *iterator) {
	zend_string *_data;

	// Initialise Iterator Object
	php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;

	// Increment the internal position and the SparseHashMap Iterator position
	_php_sparsehashmap_t *sp = PHP_SPARSEHASHMAP_FETCH_FROM(Z_OBJ(io->sparsehashmap));
	sp->shm->iterated_next();
	io->pos++;

	// Clear the current data from the data zval
	zval_ptr_dtor(&io->zdata);

	// Assign new current data in the iterator object zdata variable
	std::string current_data = sp->shm->current_data();
	_data = zend_string_init(current_data.c_str(), strlen(current_data.c_str()), 0);
	ZVAL_STR(&io->zdata, _data);

}

static zend_object_iterator_funcs php_sparsehashmap_iterator_funcs = {
	(void (*)(zend_object_iterator *))		php_sparsehashmap_iterator_dtor,
	(int (*)(zend_object_iterator *))		php_sparsehashmap_iterator_validate,
	(zval* (*)(zend_object_iterator *))		php_sparsehashmap_iterator_current_data,
	(void (*)(zend_object_iterator *, zval *))	php_sparsehashmap_iterator_current_key,
	(void (*)(zend_object_iterator *))		php_sparsehashmap_iterator_move_forward
}; /* }}} */

/* {{{ */
zend_object_iterator* php_sparsehashmap_iterator(zend_class_entry *ce, zval *sparsehashmap, int by_ref) {
	zend_string *_data;

	// Initialise Iterator Object
	php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*) emalloc(sizeof(php_sparsehashmap_iterator_t));
	zend_iterator_init((zend_object_iterator*) io);

	io->pos = 0;
	io->it.funcs = &php_sparsehashmap_iterator_funcs;
	ZVAL_COPY(&io->sparsehashmap, sparsehashmap);

	// Initialise SparseHashMap Iterator and pre-assign initial values.
	_php_sparsehashmap_t *sp = PHP_SPARSEHASHMAP_FETCH_FROM(Z_OBJ(io->sparsehashmap));
	
	// The iterator
	sp->shm->iterated_init();
	// The size of the SparseHashMap
	io->shm_size = sp->shm->iterated_size();

	// The first VALUE of the SparseHashMap
	std::string current_data = sp->shm->current_data();
	_data = zend_string_init(current_data.c_str(), strlen(current_data.c_str()), 0);
	ZVAL_STR(&io->zdata, _data);

	return (zend_object_iterator*) io;
} /* }}} */
