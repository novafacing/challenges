#!/bin/bash

docker compose up rings_of_saturn-build --build 
cp -av ../dist/* ../solve/
sudo chmod -R 777 ../solve/*
pwninit --bin ../solve/rings_of_saturn_dbg --libc ../solve/libc.so.6

