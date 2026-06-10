--TEST--
SparseHashMap Performance & Memory Benchmark
--SKIPIF--
<?php if (!extension_loaded("sparsehashmap")) print "skip"; ?>
--FILE--
<?php

ini_set('memory_limit', '4G');
const NUM_ITEMS = 900_000;

echo "=== SparseHashMap Benchmark ===\n";
echo "Items: " . number_format(NUM_ITEMS) . "\n\n";

$shm = new Sparsehashmap();

// ------------------- 1. INSERT BENCHMARK -------------------
$start_time = microtime(true);
$start_mem  = memory_get_usage(true);   // Use real usage

for ($i = 0; $i < NUM_ITEMS; $i++) {
    $key   = sprintf("%09d-%s", $i, substr(md5($i), 0, 8));
    $value = json_encode([
        'id'    => $i,
        'name'  => "User " . $i,
        'email' => "user{$i}@example.com",
        'data'  => str_repeat("x", 48)
    ], JSON_UNESCAPED_SLASHES);
    $shm->insert($key, $value);
}

$insert_time = microtime(true) - $start_time;
$insert_mem  = memory_get_usage(true) - $start_mem;

echo "Insert:  " . number_format($insert_time, 4) . " sec | Memory: " . 
     number_format($insert_mem / 1024 / 1024, 2) . " MB\n";

// ------------------- 2. LOOKUP BENCHMARK -------------------
$start_time = microtime(true);
$found = 0;

for ($i = 0; $i < NUM_ITEMS; $i += 3) {
    $key = sprintf("%09d-%s", $i, substr(md5($i), 0, 8));
    if ($shm->exists($key)) {
        $shm->read($key);
        $found++;
    }
}

$lookup_time = microtime(true) - $start_time;
echo "Lookup:  " . number_format($lookup_time, 4) . " sec (" . number_format($found) . " hits)\n";

// ------------------- 3. STATS -------------------
echo "Final Size: " . number_format($shm->length()) . " items\n";
echo "Internal Memory: " . number_format($shm->memory() / 1024 / 1024, 2) . " MB (strings only)\n";
echo "PHP Memory: " . number_format(memory_get_usage(true) / 1024 / 1024, 2) . " MB\n";

// ------------------- 4. ITERATION (to_array) - Conditional -------------------
if ($shm->length() <= 100_000) {
    $start_time = microtime(true);
    $arr = $shm->to_array();
    $iter_time = microtime(true) - $start_time;
    echo "to_array(): " . number_format($iter_time, 4) . " sec (" . count($arr) . " items)\n";
} else {
    echo "to_array(): Skipped (too large - would consume excessive memory)\n";
}

// Cleanup
$shm->flush();
echo "\nBenchmark completed.\n";
?>
--EXPECT--
=== SparseHashMap Benchmark ===
Items: 900,000