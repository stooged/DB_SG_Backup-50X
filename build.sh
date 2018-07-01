#!/bin/bash
set -e
pushd tool
make
popd
pushd DB_SG_Backup
make
popd
mkdir -p bin
rm -f bin/DB_SG_Backup.bin
cp DB_SG_Backup/DB_SG_Backup.bin bin/DB_SG_Backup.bin
mkdir -p html_payload
tool/bin2js bin/DB_SG_Backup.bin > html_payload/payload.js
FILESIZE=$(stat -c%s "bin/DB_SG_Backup.bin")
PNAME=$"DB_SG_Backup"
cp exploit.template html_payload/DB_SG_Backup.html
sed -i -f - html_payload/DB_SG_Backup.html << EOF
s/#NAME#/$PNAME/g
s/#BUF#/$FILESIZE/g
s/#PAY#/$(cat html_payload/payload.js)/g
EOF
rm -f DB_SG_Backup/DB_SG_Backup.bin
rm -f html_payload/payload.js
