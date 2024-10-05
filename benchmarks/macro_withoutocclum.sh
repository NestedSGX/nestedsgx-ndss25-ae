#!/bin/bash
outputs="$1"
cd realworldapps

echo "hashjoin"
cd hashjoin && ./hashjoin.sh $outputs && cd ..

echo "sqlite"
cd sqlite-native && ./prepare.sh && ./build.sh && ./sqlite.sh $outputs