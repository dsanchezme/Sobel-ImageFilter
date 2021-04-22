#!/bin/sh
set -e
make practicaHilos
for file in images/*.jpg
do
    times_file='times/times_'$(basename "$file" | sed 's/\(.*\)\..*/\1/')'.txt'
    out_file='images_sobel/'$(basename "$file" | sed 's/\(.*\)\..*/\1/')'_sobel.'${file##*.}
    echo $out_file
    echo "" > $times_file

    for num_threads in $@
    do
        echo "--------------------------------------------">> $times_file
        echo "TIMES FOR" ${file##*/} "WITH" $num_threads "THREADS" >> $times_file
        echo "--------------------------------------------">> $times_file
        echo "" >> $times_file
        for i in {1..10}
        do
            ./practicaHilos $file $out_file $num_threads >> $times_file
        done
        echo "" >> $times_file

    done
    # echo "TIMES FOR " '[^\/]+$'$file " WITH " $1 "THREADS" >> 'test.txt'
    # echo "I found a file named " $file >> 'test.txt'
done