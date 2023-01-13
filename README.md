
# php-sparsehashmap

## Installation

### Automatic Installation

Run the `install.sh` file to install the extension. 
```bash
./install.sh
```

This will automatically run through the below processes. 

###  Manual Installation

####  Pre-Requisite Install (Google SparseHashMap Library)
Run the below pre-install script to check if you have the Sparsehashmap Library installed, and install it if you don't.

```bash
./pre-install-check.sh 
```
or manually run through the steps
```bash
# Extract the bundled Sparsehashmap Package
tar -xvf sparsehash-sparsehash-2.0.4.tar.gz

# Change Directory to the uncompressed folder
cd sparsehash-sparsehash-2.0.4

# Configure and make the library
./configure && make

# Install the library
sudo make install
```

#### Extension Installation 

```bash
# Configure the Extension
phpize && ./configure --enable-sparsehashmap

# Make and Install the extension
make && sudo make install

# Add the Sparsehashmap .ini File to the PHP INI directory
echo "extension=sparsehashmap.so" | tee -a $(php-config --ini-dir)/sparsehashmap.ini
```

### Confirm Installation

You should now see  `sparsehashmap` listed in your php modules command
`php -m | grep sparsehashmap`


## Quick Start guide

Basic Usage of Class 


```php

// Create instance of Sparsehashmap
$sparsehashmap = new Sparsehashmap();

// Insert into Sparsehashmap
$sparsehashmap->insert("key", "value");

// Check Key exists
$sparsehashmap->exists("key");

// Read the value of Key
$sparsehashmap->read("key");

// Remove Key/Value from Sparsehashmap
$sparsehashmap->remove("key")

// Check the size of Sparsehashmap
$sparsehashmap->length();

// Remove all data from Sparsehashmap
$sparsehashmap->flush();

// Get Sparsehashmap as array
$array = $sparsehashmap->to_array();

```

See `php_tests/test.php` for a simple usage example.

