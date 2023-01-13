#!/bin/bash

current_directory=$(pwd)

if [ -d /usr/local/include/google ]; 
    then 
            echo "Google Sparsehashmap Already Installed";
    else 
            echo "Installing Google Sparsehashmap"
            echo "Download Sparsehashmap library gz <https://github.com/sparsehash/sparsehash/releases/tag/sparsehash-2.0.4>"
            curl -X GET -L "https://github.com/sparsehash/sparsehash/archive/refs/tags/sparsehash-2.0.4.tar.gz" -o sparsehash-sparsehash-2.0.4.tar.gz
            echo "Extracting..."
            tar -xvf sparsehash-sparsehash-2.0.4.tar.gz  > /dev/null 2>&1
            cd sparsehash-sparsehash-2.0.4/

            echo "Configuring..."
            ./configure > /dev/null 2>&1
            echo "Make and Install..."
            make  -j2 > /dev/null 2>&1
            sudo make install > /dev/null 2>&1
            echo "Installation Complete"
            cd $current_directory
            rm -Rf sparsehash-sparsehash-2.0.4 
            echo
fi


# Configure the Extension
echo "Configuring Sparsehashmap Extension"
phpize && ./configure --enable-sparsehashmap

# Make and Install the extension
echo "Make and Install"
make 
sudo make install

echo "Adding Sparsehashmap .ini File"
echo "extension=sparsehashmap.so" | tee -a $(php-config --ini-dir)/sparsehashmap.ini

# Once complete, exit as success.
exit(0);