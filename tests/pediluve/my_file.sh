for i;
do
    if [ -f "$i" ]
    then
        echo "$i: file"
    elif [ -d "$i" ]
    then
        echo "$i: directory"
    else
        echo "$i: unknown"
    fi
done
