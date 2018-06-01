#!/bin/bash
set -e
pushd tool
make
popd
pushd DB_SG_Backup
make
popd
rm -f bin/DB_SG_Backup.bin
cp DB_SG_Backup/DB_SG_Backup.bin bin/DB_SG_Backup.bin
tool/bin2js bin/DB_SG_Backup.bin > html_payload/payload.js
sed "s/###/$(cat html_payload/payload.js)/" exploit.template > html_payload/DB_SG_Backup.html
rm -f DB_SG_Backup/DB_SG_Backup.bin
rm -f html_payload/payload.js
