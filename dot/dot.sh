#! /bin/bash

for file in ./dot/*.dot ; do 
	echo "dot -Tsvg -o $file.svg $file"
	dot -Tsvg -o $file.svg $file
done
