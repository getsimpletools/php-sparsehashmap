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
@description: Interface to Google Sparse Hashmap C++ Implementation
@reference: https://github.com/sparsehash/sparsehash

*/

#include "sparsehashmap.h"
#include<string>
#include<string_view>

/**
 * SparseHashMap class
 */

/**
 *  C++ constructor
 */
SparseHashMap::SparseHashMap()
{
    _shash.set_deleted_key("");
}

/**
 *  Insert operation
 *  This method inserts key value into sparsehashmap
 *  @param  string     <key> <value>
 *  @return void
 */
void SparseHashMap::insert(const std::string_view& key, const std::string_view& value)
{
    _shash[std::string(key)] = std::string(value);
}

/**
 *  Read operation
 *  This method gets one parameter holding the key
 *  @param  key     Key to read
 *  @return string  Value to return
 */
std::string SparseHashMap::read(std::string_view key) const
{
    auto it = _shash.find(std::string(key));
    if (it != _shash.end()) {
        return it->second;
    }

    return {};
}

/**
 *  Read operation
 *  This method checks that a key exists in the
 *  sparsehashmap object.
 *  @param  key   Key to read
 *  @return int   Boolean True / False response
 */
bool SparseHashMap::exists(std::string_view key) const
{
    return _shash.find(std::string(key)) != _shash.end();
}

/**
 *  Remove operation
 *  This method removes a key from the instance
 *  of the sparsehashmap object.
 *  @param  string     Key
 *  @return void
 */
void SparseHashMap::remove(std::string_view key)
{
    _shash.erase(std::string(key));
}

/**
 *  Length operation
 *  This method returns size of the sparsehashmap
 *  @return int size
 */
std::size_t SparseHashMap::length() const noexcept
{
    return _shash.size();
}

/**
 *  zLength operation
 *  This method returns size of the sparsehashmap
 *  @return int size
 */
zend_long SparseHashMap::zlength() const noexcept
{
    return static_cast<zend_long>(length());
}


/**
 *  Flush operation
 *  This method removes all from the hash map
 */
void SparseHashMap::flush()
{
    _shash.clear();
}

/**
 * GC operation
 * Force the hashtable to resize to the smallest possible size.
 */
void SparseHashMap::gc()
{
  _shash.resize(0);
}

/**
 * GetThisHashMap Operation
 * Return a copy of the current sparsehashmap
 *  @return _SparseHashMap _shash;
*/
const _SparseHashMap& SparseHashMap::getThisHashMap() const noexcept
{
  return _shash;
}

/**
 * C++ Copy Constructor
 * Directly copy-constructs the underlying sparse_hash_map to avoid 
 * deleted-key layout collision bugs during raw copy assignments.
 */
SparseHashMap::SparseHashMap(const SparseHashMap& other)
    : _shash(other._shash)
{
    // Transient loop iterator states do not need to be duplicated
}

/**
 * Clone Operation
 * Copy from another instance into sparsehashmap
*/
void SparseHashMap::clone(const _SparseHashMap& _from)
{
  _shash = _from;
}


/**
 *  Memory operation
 *  Iterate over SparseHashMap, calculating size of
 *  keys and values. Return Sum size.
 *  @return int memory
 */
std::size_t SparseHashMap::memory() const
{

  std::size_t total = 0;

  for (const auto& pair : _shash) 
  {
      total += pair.first.size();
      total += pair.second.size();
  }

  return total;
}


/**
 *  Iteratable Object
 *  Create hashtable of sparsehashmap keys/values
 *  and return as iteratable hashtable.
 *  
 *  @return HashTable ht
 */

HashTable* SparseHashMap::iterated_object()
{

  zval array, value_zv;
  array_init(&array);
  HashTable *ht = Z_ARRVAL(array);
  
  //for (_SparseHashMapIterator it = _shash.begin(); it != _shash.end(); ++it) 
  for (const auto& [key, value] : _shash) 
  {

    // const auto& key = it->first;
    // const auto& value = it->second;

    zend_string *_value = zend_string_init(value.data(), value.size(), 0);
    ZVAL_STR(&value_zv, _value);
    zend_hash_str_add(ht, key.data(), key.size(), &value_zv);

  }

  return ht;
}


/**
 *  Iterator Initialiser
 *  Initialise global iterator for external
 *  calls.
 *  @return void
 */

void SparseHashMap::iterated_init()
{
  _iterator = _shash.begin();
  iterator_size = _shash.size();
}



/**
 *  Iterator size operation
 *  This method returns size of the sparsehashmap
 *  being iterated over.
 *  @return str data
 */

std::size_t SparseHashMap::iterated_size() const noexcept
{
  return iterator_size;
}

/**
 *  Iterator Current Data operation
 *  This method returns the current data value being pointed to
 *  by the iterator
 *  @return str data
 */

std::string_view SparseHashMap::current_data() const
{
    if(_iterator != const_cast<_SparseHashMap&>(_shash).end()){
        return _iterator->second;
    }

    return {};
}

/**
 *  Iterator Current Key operation
 *  This method returns the current key value being pointed to
 *  by the iterator
 *  @return str key
 */

std::string_view SparseHashMap::current_key() const
{
    if(_iterator != const_cast<_SparseHashMap&>(_shash).end()){
        return _iterator->first;
    }
    return {};
}

/**
 *  Iterator Increment
 *  This method increments the current position of the
 *  currently assigned iterator
 *
 */
void SparseHashMap::iterated_next()
{
    if(_iterator != _shash.end()) {
        ++_iterator;
    }
}
