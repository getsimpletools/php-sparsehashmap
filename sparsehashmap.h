/*

Copyright (c) 2005, Google Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of copyright holders nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.


@author: Ciaran Moore <https://github.com/ciaran-moore>
@description: CPP interface to Google Sparse Hashmap C++ Implementation
@reference: https://github.com/sparsehash/sparsehash

*/

#ifndef __CPPSPARSEHASH__
#define __CPPSPARSEHASH__

extern "C" {
	#include "php.h"
}

#include <fstream>
#include <iostream>
#include <stdbool.h>
#include <sparsehash/sparse_hash_map> //https://github.com/sparsehash/sparsehash

// Define the sparse hash map type
typedef google::sparse_hash_map<std::string, std::string> _SparseHashMap;
typedef google::sparse_hash_map<std::string, std::string>::iterator _SparseHashMapIterator;

class SparseHashMap
{
	private:
	    /**
	     *  The internal Sparsehashmap var
	     *  @var    google::sparse_hash_map
	     */
	    _SparseHashMap _shash;
	    _SparseHashMapIterator _it;
	    int _it_size;

	public:
	/**
	 *  C++ Function Prototypes
	 */
					SparseHashMap();
	virtual	 		~SparseHashMap() {_shash.clear();};
	void 			insert(std::string arg1, std::string arg2);
	std::string 	read(std::string key);
	bool 	 		exists(std::string key);
	void 			remove(std::string key);
	int 	 		length();
	int 	 		zlength();
	void 			flush();
	void 			gc();
	_SparseHashMap 	getThisHashMap();
	void			clone(_SparseHashMap _from);
	int 			memory();
	HashTable* 		iterated_object();
	void			iterated_init();
	int 			iterated_size();
	std::string 	current_data();
	std::string 	current_key();
	void 			iterated_next();
};

#endif /* __CPPSPARSEHASH__ */