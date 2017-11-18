#!/bin/bash
export LC_ALL=C

commF=$1

frobString=""
unfrobString=""

#echo $1
#echo $LOCALE
for i in {0..255} #0 - 255 num decimal values in ASCII 
do 
	oct=`printf "%03o" $i`   #tr uses \oct & requires 3 digits   
	unfrobString=$unfrobString"\\"$oct

	octF=`printf "%03o" $((i^42))`
	frobString=$frobString"\\"$octF
done

#tr $frobString $unfrobString` | sort | tr `$unfrobString $frobString

if  [[ $commF == "-f" ]]
then
	tr $frobString $unfrobString | sort -f | tr $unfrobString $frobString
else
	tr $frobString $unfrobString | sort | tr $unfrobString $frobString
fi
