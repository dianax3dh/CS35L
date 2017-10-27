#!/bin/bash

#might not work for newline filenames

#takes in an argument directory
#1 is a built in variable  
#the second command line argument
d=$1

#where to store all found files --> create an array
declare -a filesArray  #creates an array
declare -a filesReg
let ind=0  #index for the array 

#find all regular files under d directory 
#ls -a finds directory entries that begin with a dot
OIFS="$IFS" #deals with spaces in filename
IFS=$'\n'
#gets all files starting with "."
#searches through the d directory 

for i in `ls -a $d | sort -z`
do

	if [ ! -r "$d/$i" ]
	then
		echo "File not readable" $d/$i
	fi
	#if is a regular file with . then add to array
	if [[ -f "$d/$i" ]] && [[ "$i" == .* ]] 
	then 
	#puts each regular file in array by index
		filesArray[$ind]+="$d/$i"  	
		let ind=ind+1
	fi
done
IFS="$OIFS"

OIFS="$IFS"
IFS=$'\n'
#gets the rest of the files that don't start with "."
#searches through the d directory 
for i in `ls -a $d | sort -z`			
do
	if [ ! -r "$d/$i" ]
	then
		echo "File not readable" $d/$i
	fi
	#if is a regular file with . then add to array
	if [[ -f "$d/$i" ]] && [[ "$i" != .* ]] 
	then 
	#puts each regular file in array by index
		filesReg[$ind]+="$d/$i"  
		let ind=ind+1
	fi
done
IFS="$OIFS"

fullArray=("${filesArray[@]}" "${filesReg[@]}")


OIFS="$IFS"
IFS=$'\n'
for ((j=0; j < ${#fullArray[@]}; j++))
do 
	f1=${fullArray[$j]}
	for ((k=$j+1; k < ${#fullArray[@]}; k++)) 
	do
		f2=${fullArray[$k]}
		#compares the two files content
		if cmp -s "$f1" "$f2" 
		then 
			#remove and hardlink
			rm $f2
			ln $f1 $f2
		fi
	done
done
IFS="$OIFS"


