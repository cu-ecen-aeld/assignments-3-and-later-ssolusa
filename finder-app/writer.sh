#!/bin/sh
# ssolusa; above was /bash


# OVERVIEW: ./writer.sh writeToFile.txt \"Add this string\"


echo ""
echo "FILE: writer.sh..."
echo ""


# Check if writefile and writestr are provided:
if [ $# != 2 ]; then
    echo "ERROR: Two inputs are required; 'writefile' and 'writestr'."
    echo "       Ex: ./writer.sh writerTest.txt \"Add this string\""
    echo "       Exiting..."
    echo ""
    exit 1
fi


writefile="$1"
writestr="$2"


# cd $HOME/Documents/msGithubRepo/finder-app/


echo "writefile: $writefile"
echo "writestr : $writestr"
echo ""


# Create the directory, and check if it exists:
writeDir=$(dirname $writefile)
if [ ! -d $writeDir ]; then
    echo "Directory '$writeDir' DOES NOT exist; creating directory..."
    mkdir -p $writeDir
    echo "...DONE."
    echo ""
else
    echo "Directory '$writeDir' already exists; continuing..."
    echo ""
fi


# Create amd write writestr to new file:
echo "Writing user-input 'writestr' to file..."
echo -n "$writestr" > "$writefile"
echo "...DONE."
echo ""

