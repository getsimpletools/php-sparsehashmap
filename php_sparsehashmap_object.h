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

#pragma once

#ifndef PHP_SPARSEHASHMAP_OBJECT_H
#define PHP_SPARSEHASHMAP_OBJECT_H

#ifndef __CPPSPARSEHASH__
#include "sparsehashmap.h"
#endif /* __CPPSPARSEHASH__ */


/* {{{ Object Structs */
struct php_sparsehashmap_object {
	SparseHashMap *shm = nullptr;
	zend_object std;
}; /* }}} */

using php_sparsehashmap_t = php_sparsehashmap_object;

/* {{{ GET CLASS FROM ZEND */
/*
#define PHP_SPARSEHASHMAP_FETCH_FROM(obj) \
    ((php_sparsehashmap_t*)(((char*)(obj)) - XtOffsetOf(php_sparsehashmap_t, std)))

    == REINTERPRET TO CPP ==
*/  

static inline php_sparsehashmap_t* php_sparsehashmap_fetch_object(zend_object* obj)
{
  return reinterpret_cast<php_sparsehashmap_t*>(
      reinterpret_cast<char*>(obj)
      - XtOffsetOf(php_sparsehashmap_t, std)
    );
}
// Allow CONST support too.
static inline const php_sparsehashmap_t* php_sparsehashmap_fetch_object(const zend_object* obj)
{
    return reinterpret_cast<const php_sparsehashmap_t*>(
        reinterpret_cast<const char*>(obj)
        - XtOffsetOf(php_sparsehashmap_t, std)
    );
}

/* }}} */

/*
#define PHP_SPARSEHASHMAP_FETCH(z)			PHP_SPARSEHASHMAP_FETCH_FROM(Z_OBJ_P(z)); 
  
    == REINTERPRET TO CPP ==
*/

static inline php_sparsehashmap_t* php_sparsehashmap_fetch(zval* zv)
{
  return php_sparsehashmap_fetch_object(Z_OBJ_P(zv));
}


/* {{{ MODULE INIT FUNCTION */
void php_sparsehashmap_init(void); /* }}} */



#endif /* PHP_SPARSEHASHMAP_OBJECT_H */