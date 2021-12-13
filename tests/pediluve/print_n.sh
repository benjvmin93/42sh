numb=$#
if [ $numb -ne 0 ]
then
    recu=$1
    recu2=$recu
    numb=$((numb-1))
    if [ $recu -ge 0 ]
    then
        while [ $numb -ne 0 ]
        do
            shift
            while [ $recu -ne 0 ]
            do
                echo "$1"
                recu=$(($recu-1))
            done
            recu=$recu2
            numb=$((numb-1))
        done
        exit 0
    else
        exit 2
    fi
else
    exit 1
fi

