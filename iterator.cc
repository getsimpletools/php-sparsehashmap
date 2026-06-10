/*
  +----------------------------------------------------------------------+
  | PHP Version 8                                                        |
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
/*
  +----------------------------------------------------------------------+
  | PHP Version 8                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2023 The PHP Group                                |
  +----------------------------------------------------------------------+
  | Author: ciaran-moore                                                 |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_sparsehashmap_object.h"

/* {{{ Iterator Struct */
typedef struct _php_sparsehashmap_iterator_t {
    zend_object_iterator it;
    zval sparsehashmap;
    zval zdata;
    zend_long pos;
    zend_long shm_size;
} php_sparsehashmap_iterator_t; /* }}} */

/* {{{ Iterator Callbacks */

static void php_sparsehashmap_iterator_dtor(zend_object_iterator *iterator)
{
    php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
    zval_ptr_dtor(&io->sparsehashmap);
    zval_ptr_dtor(&io->zdata);
}

static zend_result php_sparsehashmap_iterator_validate(zend_object_iterator *iterator)
{
    php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
    return (io->pos < io->shm_size) ? SUCCESS : FAILURE;
}

static zval* php_sparsehashmap_iterator_current_data(zend_object_iterator *iterator)
{
    php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
    return &io->zdata;
}

static void php_sparsehashmap_iterator_current_key(zend_object_iterator *iterator, zval *key)
{
    php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
    php_sparsehashmap_t *sp = php_sparsehashmap_fetch_object(Z_OBJ(io->sparsehashmap));

    std::string_view current_key = sp->shm->current_key();
    zend_string *curr_key = zend_string_init(current_key.data(), current_key.size(), 0);
    ZVAL_STR(key, curr_key);
}

static void php_sparsehashmap_iterator_move_forward(zend_object_iterator *iterator)
{
    php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)iterator;
    php_sparsehashmap_t *sp = php_sparsehashmap_fetch_object(Z_OBJ(io->sparsehashmap));

    sp->shm->iterated_next();
    io->pos++;

    zval_ptr_dtor(&io->zdata);

    std::string_view current_data = sp->shm->current_data();
    zend_string *data = zend_string_init(current_data.data(), current_data.size(), 0);
    ZVAL_STR(&io->zdata, data);
}

static zend_object_iterator_funcs php_sparsehashmap_iterator_funcs = {
    php_sparsehashmap_iterator_dtor,
    php_sparsehashmap_iterator_validate,
    php_sparsehashmap_iterator_current_data,
    php_sparsehashmap_iterator_current_key,
    php_sparsehashmap_iterator_move_forward
}; /* }}} */

/* {{{ php_sparsehashmap_iterator */
zend_object_iterator* php_sparsehashmap_iterator(zend_class_entry *ce, zval *sparsehashmap, int by_ref)
{
    php_sparsehashmap_iterator_t *io = (php_sparsehashmap_iterator_t*)emalloc(sizeof(php_sparsehashmap_iterator_t));
    zend_iterator_init((zend_object_iterator*)io);

    io->pos = 0;
    io->it.funcs = &php_sparsehashmap_iterator_funcs;
    ZVAL_COPY(&io->sparsehashmap, sparsehashmap);

    // Use modern fetch helper
    php_sparsehashmap_t *sp = php_sparsehashmap_fetch_object(Z_OBJ_P(sparsehashmap));

    sp->shm->iterated_init();
    io->shm_size = static_cast<zend_long>(sp->shm->iterated_size());

    // Initial data
    std::string_view current_data = sp->shm->current_data();
    zend_string *data = zend_string_init(current_data.data(), current_data.size(), 0);
    ZVAL_STR(&io->zdata, data);

    return (zend_object_iterator*)io;
} /* }}} */