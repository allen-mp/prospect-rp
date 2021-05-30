#!/bin/bash
pkill python3
nohup python3 -u ./main.py > log.txt &
tail -f log.txt
