#!/bin/sh

if [ "$#" -eq 0 ] ; then
	startDir="./"
else
	startDir="$1"
fi
convertFolder="CONVERT"

recursive(){
	for file in $(ls "$1"); do
		fullFile="$1/$file"
		[ "$file" = "." -o "$file" = ".." ] && continue
		if [ -d "$fullFile" ] ; then
			recursive "$fullFile"
		elif [ -f "$fullFile" ] ; then
			extension="${file##*.}"
			filename="${file%.*}"
			if [ -f "$fullFile" -a "$extension" = "BMP" ] ; then
				newFolder="$1/$convertFolder"
				mkdir -p "$newFolder"
				newFile="$newFolder/$filename"
				convert "$fullFile" "$newFile""_Original.jpg" #640x480
				convert "$fullFile" -crop 528x480+0+0 "$newFile""_Cutted.jpg"
				convert "$fullFile" -crop 499x379+10+19 "$newFile""_StrongCutted.jpg"
				echo  "$newFile"
			fi
		fi
	done
}

recursive "$startDir"
