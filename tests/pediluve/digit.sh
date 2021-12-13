for i;
do
    case "$i" in
        -[[:digit:]])
            echo "$i is a negative digit."
            ;;
        [[:digit:]])
            echo "$i is a positive digit."
            ;;
        +[[:digit:]])
            echo "$i is a positive digit."
            ;;
        *)
            echo "$i is not a digit."
            ;;
        esac
done
