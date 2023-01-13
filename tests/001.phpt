--TEST--
Indexed basic operation
--SKIPIF--
<?php if (!extension_loaded("sparsehashmap")) print "skip"; ?>
--FILE--
<?php
$shm = new Sparsehashmap();

$shm->insert("63-5039337", '{"id":1,"first_name":"Ilario","last_name":"Thickins"}');
$shm->insert("09-9903734", '{"id":2,"first_name":"Grange","last_name":"Wibberley"}');
$shm->insert("73-2415409", '{"id":11,"first_name":"Rene","last_name":"Bloxham"}');

var_dump($shm->exists("63-5039337"));

$shm->remove("63-5039337");

var_dump($shm->exists("63-5039337"));

var_dump($shm->as_array());

var_dump($shm->length());

var_dump($shm->read("09-9903734"));

var_dump($shm->read("63-5039337"));

$shm->flush();

var_dump($shm->as_array());

?>
--EXPECT--
bool(true)
bool(false)
array(2) {
  ["09-9903734"]=>
  string(54) "{"id":2,"first_name":"Grange","last_name":"Wibberley"}"
  ["73-2415409"]=>
  string(51) "{"id":11,"first_name":"Rene","last_name":"Bloxham"}"
}
int(2)
string(54) "{"id":2,"first_name":"Grange","last_name":"Wibberley"}"
string(0) ""
array(0) {
}
