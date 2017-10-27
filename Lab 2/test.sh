#!/bin/bash
dir=$1

declare -a files

while read -r -d ''
do
	if [ -r "$REPLY" ]
	then
		files+=("$REPLY")
	else
		echo "not readable"
	fi
done < <(find $dir -maxdepth 1 -type f -name ".*" -print0 | sort -z)
echo ${files[@]}
echo "HELLO"

while read -r -d ''
do
	if [ -r "$REPLY" ]
	then
		files+=("$REPLY")
	else
		echo "not readable"
	fi
done < <(find $dir -maxdepth 1 -type f -not -name ".*" -print0 | sort -z)

# echo ${files[0]}
# echo ${files[1]}
# echo ${files[2]}
# echo ${files[3]}
# echo ${files[4]}
# echo ${files[5]}
# echo ${files[6]}
# echo ${files[7]}
# echo ${files[8]}
# echo ${files[9]}

for ((i=0; i < ${#files[@]}; i++))
do 
	file1=${files[$i]}

	for ((j=$i+1; j < ${#files[@]}; j++))
	do
		file2=${files[$j]}

		if cmp -s "$file1" "$file2"
		then 
			echo $file1
			echo $file2
			rm $file2
			ln $file1 $file2
		fi
	done
done

# #Used the skeleton of the sample code provided in the powerpoint                
# dir=$1
# RESULT=`ls -a $dir | sort`
# declare -a ARRAY
# let count=0

# for FILE in $RESULT
# do
#     if [ -r "$dir/$FILE" ] #Check if it's a readable file                       
#     then
#         echo "$dir/$FILE is readable."
#     else #Unreadable files are not treated as a duplicate of any file and the e\                                                                
#         echo "$dir/$FILE is NOT readable."
#     fi
    
# if [ ! -L "$dir/$FILE" ] #Check if it's not a symbolic link                 
#     then
#         if [ -f "$dir/$FILE" ] #Check if it's a regular file                    
#         then
#             echo "$dir/$FILE is a regular file."
#             ARRAY[$count]="$dir/$FILE"
#             let count=count+1
#         else #Unreadable files are silently ignored; including their symbolic l\                                                           
#             echo "$dir/$FILE is NOT a regular file."
#         fi
#     fi
# done

# echo "There are $count regular files."

# for i in "${ARRAY[@]}" #Given as a homework hint in the powerpoint, is updated \                                                                   
# do
#     for j in "${ARRAY[@]}"
#     do
#         cmp $i $j #Compare both files                                           

#         if [ $? == 0 ] #If both files are duplicates                            
#         then
#             rm $j #Remove the file that isn't lexiographically first            
#             ln $i $j #Replaces j with hard links to i                           
#         fi
#     done
# done





# for j in ${filesArr[@]}   #first file
# do 
# 	for k in ${filesArr[@]}  #second file 
# 	do 
# 		if cmp -s ${filesArr[$j]} ${filesArr[$k]} #compare two files; if both files same 
# 		then
# 			echo "files match"
# 		else
# 			echo "don't match"
# 		fi
# 	done 
# 	# let index=index+1
# 	# echo "j is" $j 
# 	# echo ${filesArr[$index]}
# 	# if cmp -s ${filesArr[$j]} ${filesArr[]} #compare two files; if both files same 
# 	# then
# 	# 	echo "files match"
# 	# else
# 	# 	echo "don't match"
# 	# fi
# done

# FILE=$1 
# while readarray filesArr < $1; 
# do 
# 	# if [ -f "$FILE" ]
# 	# then
# 		echo "$filesArr"
# 	# fi
# done < $1
#echo $filesArr
# dir=$1
# read -a filesArr < $1
# # echo `ls $dir`
# echo $filesArr



# echo ${fullArray[@]}
# echo ${fullArray[0]}
# echo ${fullArray[1]}
# echo ${fullArray[2]}
# echo ${fullArray[3]}
# echo ${fullArray[4]}
# echo ${fullArray[5]}
# echo ${fullArray[6]}
# echo ${fullArray[7]}
# echo ${fullArray[8]}
# echo ${fullArray[9]}
# echo ${filesReg[@]}




z