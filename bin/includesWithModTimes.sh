#!/bin/bash
outFile=$2
echo "{" > $outFile
for x in $(gfind $1 -iregex '.*\(\.c\|\.cpp\|\.h\|\.hpp\)'); do
	echo $(echo $(basename $x) | gsed 's/\./_/g') $(gstat -c %Y $x) >> $outFile;
done

for x in $(gfind $1 -iregex '.*\(\.c\|\.cpp\|\.h\|\.hpp\)'); do for y in $(echo $(egrep "#include" $x | grep -oE '["].*\.h.*["]') | sed '/^$/d'| gsed -e 's/\b"//g'  -e 's/"\b//g'); do 
	if [[ $(basename $x) == *.h*  ]] 
	then 
		echo $(echo $(basename $y) \-\> $(basename $x) | gsed 's/\./_/g')  >>$outFile;
	else
		echo $(echo $(basename $y) \-\> $(basename $x) | gsed 's/\./_/g')  >>$outFile;
	fi
	done ;
done
echo "}" >> $outFile



