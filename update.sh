#!/usr/bin/env bash

cd $(dirname ${BASH_SOURCE[0]})

echo "DELETING OLD ULTIMMC, PRESS ENTER TO CONFIRM OR CTRL+C TO EXIT!"
read -r
rm -r bin
rm UltimMC
echo "TRIED TO DELETE OLD ULTIMMC SUCCESSFULLY!"
echo "DOWNLOADING NEW ULTIMMC..."
curl -fsSLo update.zip https://nightly.link/AfoninZ/UltimMC/workflows/main/develop/mmc-cracked-lin64.zip
echo "DOWNLOADED AS update.zip FILE"
echo "APPLYING UPDATE..."
unzip update.zip
mv UltimMC exzip
mv exzip/* .
echo "CLEANING UP..."
rm -r exzip update.zip
