#!/bin/bash
#//////////////////////////////////////////////////////
#       Created by George Bogdan Oprea
#       Checker Tema 1 - Structuri de date
#       Algoritmi de planificare pe procesor
#       Facultatea de Automatica si Calculatoare
#       Anul Universitar 2016-2017, Seria CC
#/////////////////////////////////////////////////////
rm -rf out
result1=0
result2=0
result3=0
result4=0
bonus=0
ZERO=0
make clean > /dev/null 2>&1
make > /dev/null 2>&1
sleep 1
BEST=110

#Cerinta 1 - Compresia fisierelor
echo "Cerinta 1"
punctaje=(1 1 1 2 2 3)
for i in {0..5}
do
    let "j = $i + 1"
    inputFile="FCFS/in"$j
    referenceFile="FCFS/ref"$j
    outputFile="FCFS/out"$j
    ./planificator $inputFile $outputFile > /dev/null 2>&1
    diff $outputFile $referenceFile > /dev/null 2>&1
    EXIT_CODE=$?
    rm -rf $stpOut > /dev/null 2>&1
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Cerinta_1_$j ................................................. PASS"
        result=$((result+punctaje[$i]))
        result1=$((result1+punctaje[$i]))
    else
        echo "Cerinta_1_$j ................................................. FAIL"
    fi
done

#Cerinta 2
echo
echo "Cerinta 2"
punctaje=(1 2 2 3 2 5 5)
for i in {0..6}
do
    let "j = $i + 7"
    inputFile="SJF/in"$j
    referenceFile="SJF/ref"$j
    outputFile="SJF/out"$j
    ./planificator $inputFile $outputFile > /dev/null 2>&1
    diff $outputFile $referenceFile > /dev/null 2>&1
    EXIT_CODE=$?
    rm -rf $stpOut > /dev/null 2>&1
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Cerinta_2_$j ................................................. PASS"
        result=$((result+punctaje[$i]))
        result2=$((result2+punctaje[$i]))
    else
        echo "Cerinta_2_$j ................................................. FAIL"
    fi
done

#Cerinta 3
echo
echo "Cerinta 3"
punctaje=(2 3 5 5 5 5 5)
for i in {0..6}
do
    let "j = $i + 14"
    inputFile="RR/in"$j
    referenceFile="RR/ref"$j
    outputFile="RR/out"$j
    ./planificator $inputFile $outputFile > /dev/null 2>&1
    diff $outputFile $referenceFile > /dev/null 2>&1
    EXIT_CODE=$?
    rm -rf $stpOut > /dev/null 2>&1
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Cerinta_3_$j ................................................. PASS"
        result=$((result+punctaje[$i]))
        result3=$((result3+punctaje[$i]))
    else
        echo "Cerinta_3_$j ................................................. FAIL"
    fi
done

#Cerinta 4
echo
echo "Cerinta 4"
punctaje=(1 2 3 4 5 5 5 5)
for i in {0..7}
do
    let "j = $i + 21"
    inputFile="PP/in"$j
    referenceFile="PP/ref"$j
    outputFile="PP/out"$j
    ./planificator $inputFile $outputFile > /dev/null 2>&1
    diff $outputFile $referenceFile > /dev/null 2>&1
    EXIT_CODE=$?
    rm -rf $stpOut > /dev/null 2>&1
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Cerinta_4_$j ................................................. PASS"
        result=$((result+punctaje[$i]))
        result4=$((result4+punctaje[$i]))
    else
        echo "Cerinta_4_$j ................................................. FAIL"
    fi
done

#Bonus
echo
echo "Bonus"
punctaje=(5 5 10)
for i in {0..2}
do
    let "j = $i + 29"
    inputFile="Bonus/in"$j
    referenceFile="Bonus/ref"$j
    outputFile="Bonus/out"$j
    ./planificator $inputFile $outputFile > /dev/null 2>&1
    diff $outputFile $referenceFile > /dev/null 2>&1
    EXIT_CODE=$?
    rm -rf $stpOut > /dev/null 2>&1
    if [ $EXIT_CODE -eq $ZERO ]
    then
        echo "Bonus_$j ................................................. PASS"
        result=$((result+punctaje[$i]))
        bonus=$((bonus+punctaje[$i]))
    else
        echo "Bonus_$j ................................................. FAIL"
    fi
done

echo
echo "Cerinta 1 : "$result1
echo "Cerinta 2 : "$result2
echo "Cerinta 3 : "$result3
echo "Cerinta 4 : "$result4
echo "Bonus     : "$bonus
if [ "$result" -eq $BEST ];
then
        echo "Felicitari! Ai punctajul maxim: "$BEST"p! :)"
else
        echo "Ai acumulat "$result"p din maxim 110p! :("
fi

rm -rf FCFS/out* SJF/out* RR/out* PP/out* Bonus/out* > /dev/null 2>&1


