if [ $# -eq 1 ]
then
    total=1
    i=1
    while [ $i -le $1 ]
    do
        total=$(( total * i ))
        i=$((i+1))
    done
    echo $total
    exit 0
else
    exit 1
fi
