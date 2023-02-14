#!/bin/bash

echo "compilando paralelo..."
mpicc paralelo.c -O3 -o paralelo

for t in 1 2 4 8; do
    echo "t: $t"
    
    for i in $(seq 1 20); do
        mpirun -np $t ./paralelo entrada.txt 10 4 >> paralelo5.txt
        mpirun -np $t ./paralelo entrada2.txt 20 4 >> paralelo6.txt
        mpirun -np $t ./paralelo entrada3.txt 30 4  >> paralelo7.txt
        
    done

    grep seconds paralelo5.txt | cut -f1 -d" " > time_paralelo5.txt
    grep seconds paralelo6.txt | cut -f1 -d" " > time_paralelo6.txt
    grep seconds paralelo7.txt | cut -f1 -d" " > time_paralelo7.txt

    echo 'Tempo paralelo tamanho 10:'
    cat time_paralelo5.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo paralelo tamanho 20:'
    cat time_paralelo6.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo paralelo tamanho 30:'
    cat time_paralelo7.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'

done

echo "compilando serial..."
gcc serial.c -O3 -o serial

for i in $(seq 1 20); do
    ./serial entrada.txt 10 4 >> serial5.txt
    ./serial entrada2.txt 20 4 >> serial6.txt
    ./serial entrada3.txt 30 4 >> serial7.txt
done


    grep seconds serial5.txt | cut -f1 -d" " > time_serial5.txt
    grep seconds serial6.txt | cut -f1 -d" " > time_serial6.txt
    grep seconds serial7.txt | cut -f1 -d" " > time_serial7.txt

    echo '\n'
    echo 'Tempo serial tamanho 10:'
    cat time_serial5.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo serial tamanho 20:'
    cat time_serial6.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'
    echo 'Tempo serial tamanho 30:'
    cat time_serial7.txt | awk '{sum+=$1} END {print sum/20}'
    echo '\n'