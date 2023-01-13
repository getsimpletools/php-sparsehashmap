#!/bin/bash

if [ -d /usr/local/include/google ]; 
    then 
            echo "Google Sparsehashmap Already Installed";
    else 
            current=$(pwd)
            echo "Installing Google Sparsehashmap"
            echo "Extracting..."
            tar -xvf sparsehash-sparsehash-2.0.4.tar.gz  > /dev/null 2>&1
            cd sparsehash-sparsehash-2.0.4/

            echo "Configuring..."
            ./configure > /dev/null 2>&1
            echo "Make and Install..."
            make  -j2 > /dev/null 2>&1
            sudo make install > /dev/null 2>&1
            echo "Installation Complete"
            cd $current
            rm -Rf sparsehash-sparsehash-2.0.4 
            echo
fi
exit 0;
