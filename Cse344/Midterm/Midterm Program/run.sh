#!/bin/bash
for ((i = 0; i < 5; i++))
do
  ./client -s "/tmp/ServerFIFO" -o "data.csv" &
  sleep .1
  ./client -s "/tmp/ServerFIFO" -o "data2.csv" &
  sleep .1
done