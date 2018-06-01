#!/bin/bash
pushd tool
make clean
popd
pushd DB_SG_Backup
make clean
popd
rm -f html_payload/DB_SG_Backup.html
rm -f bin/DB_SG_Backup.bin

