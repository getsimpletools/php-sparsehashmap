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

#ifndef PHP_SPARSEHASHMAP_H
#define PHP_SPARSEHASHMAP_H

extern "C" {

	extern zend_module_entry sparsehashmap_module_entry;
	#define phpext_sparsehashmap_ptr &sparsehashmap_module_entry;

	#define PHP_SPARSEHASHMAP_EXTNAME	"sparsehashmap"
	#define PHP_SPARSEHASHMAP_EXTVER	"1.0.0"


	#ifdef ZTS
		#include "TSRM.h"
	#endif

	#if defined(ZTS) && defined(COMPILE_DL_SPARSEHASHMAP)
		ZEND_TSRMLS_CACHE_EXTERN();
	#endif

}

#endif /* PHP_SPARSEHASHMAP_H */