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

#ifndef HAVE_SPARSEHASHMAP_OBJECT_H
#define HAVE_SPARSEHASHMAP_OBJECT_H

#ifndef __CPPSPARSEHASH__
#include "sparsehashmap.h"
#endif /* __CPPSPARSEHASH__ */


/* {{{ Object Structs */
typedef struct _php_sparsehashmap_t {
	SparseHashMap *shm;
	zend_object std;
} _php_sparsehashmap_t; /* }}} */


/* {{{ GET CLASS FROM ZEND */
#define PHP_SPARSEHASHMAP_FETCH_FROM(obj)	((_php_sparsehashmap_t*) (((char*)obj) - XtOffsetOf(_php_sparsehashmap_t, std)))
#define PHP_SPARSEHASHMAP_FETCH(z)			PHP_SPARSEHASHMAP_FETCH_FROM(Z_OBJ_P(z)); /* }}} */


/* {{{ MODULE INIT FUNCTION */
void php_sparsehashmap_init(void); /* }}} */



#endif /* HAVE_SPARSEHASHMAP_OBJECT_H */