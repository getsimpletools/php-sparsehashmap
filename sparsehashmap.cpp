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
#include<sstream>

/**
 * SparseHashMap class
 */

/**
 *  C++ constructor
 */
SparseHashMap::SparseHashMap(){_shash.set_deleted_key("");}

/**
 *  Insert operation
 *  This method inserts key value into sparsehashmap
 *  @param  string     arg1<key> arg2<value>
 *  @return void
 */
void SparseHashMap::insert(std::string arg1, std::string arg2)
{
    _shash[arg1] = arg2;
}

/**
 *  Read operation
 *  This method gets one parameter holding the key
 *  @param  key     Key to read
 *  @return string  Value to return
 */
std::string SparseHashMap::read(std::string key)
{
    return _shash[key];
}

/**
 *  Read operation
 *  This method checks that a key exists in the
 *  sparsehashmap object.
 *  @param  key   Key to read
 *  @return int   Boolean True / False response
 */
bool SparseHashMap::exists(std::string key)
{
    if (_shash.find(key) == _shash.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 *  Remove operation
 *  This method removes a key from the instance
 *  of the sparsehashmap object.
 *  @param  string     Key
 *  @return void
 */
void SparseHashMap::remove(std::string key)
{
    _shash.erase(key);
}

/**
 *  Length operation
 *  This method returns size of the sparsehashmap
 *  @return int size
 */
int SparseHashMap::length()
{
    return (int) _shash.size();
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
_SparseHashMap SparseHashMap::getThisHashMap()
{
  return _shash;
}

/**
 * Clone Operation
 * Copy from another instance into sparsehashmap
*/
void SparseHashMap::clone(_SparseHashMap _from)
{
  _shash = _from;
}

/**
 *  Length operation
 *  This method returns size of the sparsehashmap
 *  @return int size
 */
int SparseHashMap::zlength()
{
    int size = (int) _shash.size();
    
    return size;
}

/**
 *  Memory operation
 *  Iterate over SparseHashMap, calculating size of
 *  keys and values. Return Sum size.
 *  @return int memory
 */
int SparseHashMap::memory()
{

  size_t total = 0;

  for (google::sparse_hash_map<std::string, std::string>::iterator it = _shash.begin(); it != _shash.end(); ++it) 
  {
      std::string resp = it->second;
      total += resp.size();

      std::string *first = const_cast<std::string*>(&it->first);
      total += first->size();
  }

  int _total = total;

  return _total;
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

  zval array, value;
  HashTable *ht;
  array_init(&array);
  ht = Z_ARRVAL(array);

  zend_string *_value;
  zlength();
  
  for (google::sparse_hash_map<std::string, std::string>::iterator it = _shash.begin(); it != _shash.end(); ++it) 
  {
    std::string second = it->second;
    std::string *first = const_cast<std::string*>(&it->first);


    _value = zend_string_init(second.c_str(), strlen(second.c_str()), 0);
    ZVAL_STR(&value, _value);
    zend_hash_str_add(ht, first->c_str(), strlen(first->c_str()), &value);

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
  _it = _shash.begin();
  _it_size = (int) _shash.size();
}



/**
 *  Iterator size operation
 *  This method returns size of the sparsehashmap
 *  being iterated over.
 *  @return str data
 */

int SparseHashMap::iterated_size()
{
  return _it_size;
}

/**
 *  Iterator Current Data operation
 *  This method returns the current data value being pointed to
 *  by the iterator
 *  @return str data
 */

std::string SparseHashMap::current_data()
{
  std::string data = _it->second;
  return data;
}

/**
 *  Iterator Current Key operation
 *  This method returns the current key value being pointed to
 *  by the iterator
 *  @return str key
 */

std::string SparseHashMap::current_key()
{
  std::string *first = const_cast<std::string*>(&_it->first);
  std::string key = *first;

  return key;
}

/**
 *  Iterator Increment
 *  This method increments the current position of the
 *  currently assigned iterator
 *
 */
void SparseHashMap::iterated_next()
{
  ++_it;
}
