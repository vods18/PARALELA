#!/bin/bash

echo "compilando paralelo..."
gcc chessP.c -fopenmp -O3 -o paralelo

for t in 1 2 4 8; do
    echo "t: $t"
    export OMP_NUM_THREADS=$t
    
    for i in $(seq 1 20); do
        ./paralelo 5 5 0 0 >> paralelo5.txt
        ./paralelo 6 6 0 0 >> paralelo6.txt
        ./paralelo 7 7 0 0 >> paralelo7.txt
        
    done

    grep seconds paralelo5.txt | cut -f1 -d" " > time_paralelo5.txt
    grep seconds paralelo6.txt | cut -f1 -d" " > time_paralelo6.txt
    grep seconds paralelo7.txt | cut -f1 -d" " > time_paralelo7.txt

    echo 'Tempo paralelo 5x5:'
    cat time_paralelo5.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo paralelo 6x6:'
    cat time_paralelo6.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo paralelo 7x7:'
    cat time_paralelo7.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'

done

echo "compilando serial..."
gcc chessS.c -O3 -o serial

for i in $(seq 1 20); do
    ./serial 5 5 0 0 >> serial5.txt
    ./serial 6 6 0 0 >> serial6.txt
    ./serial 7 7 0 0 >> serial7.txt
done


    grep seconds serial5.txt | cut -f1 -d" " > time_serial5.txt
    grep seconds serial6.txt | cut -f1 -d" " > time_serial6.txt
    grep seconds serial7.txt | cut -f1 -d" " > time_serial7.txt

    echo '\n'
    echo 'Tempo serial 5x5:'
    cat time_serial5.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo serial 6x6:'
    cat time_serial6.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo serial 7x7:'
    cat time_serial7.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'