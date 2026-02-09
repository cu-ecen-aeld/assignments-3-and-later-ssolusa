#!/bin/sh
# ssolusa; HW 3 Part 2; above was /bin/bash


# OVERVIEW: ./finder.sh . [searchstr]


echo ""
echo "FILE: finder.sh..."
echo ""


# Check if filesdir and searchstr are provided:
if [ $# != 2 ]; then
    echo "ERROR: Two inputs are required; \"filesdir\" and \"searchstr\"."
    echo "       Ex: ./finder.sh . bash"
    echo "       Exiting..."
    echo ""
    exit 1
fi


filesdir="$1"
searchstr="$2"


# cd $HOME/Documents/msGithubRepo/finder-app/                                          # ssolusa; remove for HW 3 Part 2


echo "filesdir : $filesdir"
echo "searchstr: $searchstr"
echo ""


# Check if filesdir exist at pwd:
if [ ! -d "$filesdir" ]; then
    echo "ERROR: Directory '$filesdir' NOT found in pwd."
    echo "       Exiting..."
    echo ""
    exit 1
else
    echo "Directory '$filesdir' found in pwd."
    echo ""
fi


# Count the number of files in filesdir and subdirectories:
numOfFiles=$(find "$filesdir" -type f | wc -l)
echo "numOfFiles: $numOfFiles"
echo ""


# Count the number of lines matching to searchstr:
if [ $numOfFiles == 0 ]; then
    numOfLines=0
else
    numOfLines=$(grep -R "$searchstr" "$filesdir" | wc -l)
fi
echo "numOfLines: $numOfLines"
echo ""


# Print summary of results:
echo "The number of files are $numOfFiles and the number of matching lines are $numOfLines."
echo ""
