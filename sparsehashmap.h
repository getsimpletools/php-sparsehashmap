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

#pragma once
#ifndef CPPSPARSEHASH_H
#define CPPSPARSEHASH_H

extern "C" {
	#include "php.h"
}

#include <string>
#include <string_view>
#include <sparsehash/sparse_hash_map> //https://github.com/sparsehash/sparsehash

// Define the sparse hash map type
using _SparseHashMap = google::sparse_hash_map<std::string, std::string>;
using _SparseHashMapIterator = _SparseHashMap::iterator;

class SparseHashMap
{
	private:
        /**
        *  The internal Sparsehashmap var
        *  @var    google::sparse_hash_map
        */
        _SparseHashMap _shash;
        _SparseHashMapIterator _iterator{};
        std::size_t iterator_size = 0;

    public:
        /**
        *  C++ Function Prototypes
        */
                                 SparseHashMap();
                                 ~SparseHashMap() = default;
                                 
                                 SparseHashMap(const SparseHashMap& other);
        SparseHashMap&           operator=(const SparseHashMap&) = delete;
        
                                 SparseHashMap(SparseHashMap&&) noexcept = default;
        SparseHashMap&           operator=(SparseHashMap&&) noexcept = default;
        
        void                     insert(const std::string_view& key, const std::string_view& value);
        
        // Lookup methods - accept string_view for zero-copy from PHP
        
        std::string              read(std::string_view key) const;
        bool                     exists(std::string_view key) const;
        void                     remove(std::string_view key);
        
        std::size_t              length() const noexcept;
        zend_long                zlength() const noexcept;
        void                     flush();
        void                     gc();
        
        
        const _SparseHashMap&    getThisHashMap() const noexcept; //return reference
        void                     clone(const _SparseHashMap& _from);
        
        std::size_t              memory() const;
        
        HashTable*               iterated_object();
        void                     iterated_init();
        std::size_t              iterated_size() const noexcept;
        std::string_view         current_data() const;
        std::string_view         current_key() const;
        void                     iterated_next();
};

#endif /* CPPSPARSEHASH_H */