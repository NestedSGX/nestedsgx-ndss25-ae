#!/bin/bash
outputs=$1
cd server && ./server.sh

cd ../client && ./client.sh $outputs