<?php

$start = gettimeofday(true);

include "test_data.php";

$shm = new Sparsehashmap();

echo "\n\n ==== INSERT DATA ====\n\n";

foreach($mock_data as $key => $value)
{
	$shm->insert($key, $value);
}

echo "\n\n==== DATA EXISTS CHECK\n\n";

echo "Check Exists: 00-7781122 " . $shm->exists("00-7781122") . "\n";
echo "Check Exists: 25-6082137 " . $shm->exists("25-6082137") . "\n";
echo "Check Exists: 21-0802405 " . $shm->exists("21-0802405") . "\n";
echo "Check Exists: 21-0802405 " . $shm->exists("21-FALSE") . "\n";
echo "Check Exists: 26-3608774 " . $shm->exists("26-3608774") . "\n";
echo "Check Exists: 87-5800003 " . $shm->exists("87-5800003") . "\n";
echo "Check Exists: 50-1290999 " . $shm->exists("50-1290999") . "\n";
echo "Check Exists: 50-1290999 " . $shm->exists("50-1FALSE9") . "\n";
echo "Check Exists: 75-0550590 " . $shm->exists("75-0550590") . "\n";
echo "Check Exists: 55-8286874 " . $shm->exists("55-8286874") . "\n";
echo "Check Exists: 18-0682812 " . $shm->exists("18-0682812") . "\n";
echo "Check Exists: 06-4612047 " . $shm->exists("06-4612047") . "\n";
echo "Check Exists: 21-5126838 " . $shm->exists("21-5126838") . "\n";


echo "\n\n END DATA EXISTS CHECK ====\n\n";

echo "\n\n==== READ DATA CHECK\n\n";

echo "Read From: 00-7781122" . $shm->read("00-7781122") . "\n";
echo "Read From: 25-6082137" . $shm->read("25-6082137") . "\n";
echo "Read From: 21-0802405" . $shm->read("21-0802405") . "\n";
echo "Read From: 26-3608774" . $shm->read("26-3608774") . "\n";
echo "Read From: 87-5800003" . $shm->read("87-5800003") . "\n";
echo "Read From: 50-1290999" . $shm->read("50-1290999") . "\n";
echo "Read From: 75-0550590" . $shm->read("75-0550590") . "\n";
echo "Read From: 55-8286874" . $shm->read("55-8286874") . "\n";
echo "Read From: 18-0682812" . $shm->read("18-0682812") . "\n";
echo "Read From: 06-4612047" . $shm->read("06-4612047") . "\n";
echo "Read From: 21-5126838" . $shm->read("21-5126838") . "\n";

echo "\n\nEND READ DATA CHECK ====\n\n";

echo "\n\n==== CHECK SIZE OF HASHMAP\n\n";

echo "Size of Hashmap: " . $shm->length() ."\n";

echo "\n\nEND CHECK SIZE OF HASHMAP ====\n\n";

echo "\n\n==== VAR_DUMP OF HASHMAP\n\n";

var_dump($shm);

echo "\n\nEND VAR_DUMP OF HASHMAP ====\n\n";

echo "\n\n==== REMOVE ELEMENTS FROM HASHMAP ====\n\n";

echo "Remove Key: 00-7781122\n"; $shm->remove("00-7781122");
echo "Remove Key: 25-6082137\n"; $shm->remove("25-6082137");
echo "Remove Key: 21-0802405\n"; $shm->remove("21-0802405");
echo "Remove Key: 26-3608774\n"; $shm->remove("26-3608774");
echo "Remove Key: 87-5800003\n"; $shm->remove("87-5800003");
echo "Remove Key: 50-1290999\n"; $shm->remove("50-1290999");
echo "Remove Key: 75-0550590\n"; $shm->remove("75-0550590");
echo "Remove Key: 55-8286874\n"; $shm->remove("55-8286874");
echo "Remove Key: 18-0682812\n"; $shm->remove("18-0682812");
echo "Remove Key: 06-4612047\n"; $shm->remove("06-4612047");
echo "Remove Key: 21-5126838\n"; $shm->remove("21-5126838");


echo "\n\n==== RE-CHECK SIZE OF HASHMAP\n\n";

echo "Size of Hashmap: " . $shm->length() ."\n";

echo "\n\nEND RE-CHECK SIZE OF HASHMAP ====\n\n";


echo "\n\n==== CHECK FOR EACH ITERATOR\n\n";
$counter = 0;
foreach($shm as $key => $value)
{
	if ($counter % 200 == 0) 
	{
		echo "Key: " . $key . "\n";
		echo "Value: " . $value . "\n"; 
	}
	
	++$counter;
}

echo "\n\nEND CHECK FOREACH ITERATOR ====\n\n";


echo "\n\n==== VAR_DUMP OF HASHMAP\n\n";

var_dump($shm);

echo "\n\nEND VAR_DUMP OF HASHMAP ====\n\n";


echo "\n\n==== FLUSH HASHMAP ====\n\n";

$shm->flush();

echo "\n\n==== RE-CHECK SIZE OF HASHMAP\n\n";

echo "Size of Hashmap: " . $shm->length() ."\n";

echo "\n\nEND RE-CHECK SIZE OF HASHMAP ====\n\n";

unset($shm);

$end = gettimeofday(true);

$taken = $end - $start;

echo "Test Complete in " . $taken . "ms\n\n";
exit;
