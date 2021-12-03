#!/bin/sh

printf "%b\n" '____________/\\\\\\_______/\\\\\\\\\\\\\\\\\\___________________/\\\\\\_________'
printf "%b\n" ' __________/\\\\\\\\\\_____/\\\\\\///////\\\\\\________________\/\\\\\\_________'
printf "%b\n" '  ________/\\\\\/\\\\\\____\\///______\\//\\\\\\_______________\\/\\\\\\_________'
printf "%b\n" '   ______/\\\\\\/\\/\\\\\\______________/\\\\\\/____/\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\_________'
printf "%b\n" '    ____/\\\\\\/__\\/\\\\\\___________/\\\\\\//_____\\/\\\\\\//////__\\/\\\\\\\\\\\\\\\\\\\\__'
printf "%b\n" '     __/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_____/\\\\\\//________\\/\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\/////\\\\\\_'
printf "%b\n" '      _\\///////////\\\\\\//____/\\\\\\/___________\\////////\\\\\\_\\/\\\\\\___\\/\\\\\\_'
printf "%b\n" '       ___________\\/\\\\\\_____/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\___\\/\\\\\\_'
printf "%b\n" '        ___________\\///_____\\///////////////__\\//////////__\\///____\\///__'

echo ""

GREEN="\e[42m"
RED="\e[41m"
END="\e[0m"
BLINK="\e[5m"
LINE="\e[1;4m"

test=../builddir/42sh
ref=dash
it=0
success=0

FILES="../tests/*"

for f in $FILES; do
    if [ "$f" != "../tests/test.sh" ]; then
        echo "===============$f=============="
        echo ""
        while read expr; do
            echo "-----------"
            printf "TEST[${it}]: ${expr}\n"
            testres=$(${test} -c "${expr}")
            #codetest=$?

            refres=$(${ref} -c "${expr}")
            #coderef=$?
                        if [ "$testres" != "$refres" ]; then
                printf "  ${BLINK}${RED}FAILED${END}\n"
                echo ""
                echo "      expected : \n$refres"
                echo "      got : $testres"
                echo ""
            #elif [ "$codetest" -ne "$coderef" ]; then
            #    printf "  ${BLINK}${RED}FAILED code ($codetest != $coderef)${END}\n"
            #    echo ""
            else
                success=$(($success + 1))
                printf "  ${BLINK}${GREEN}SUCCESS${END}\n"
                echo ""
                echo "      got : \n$testres"
                echo ""
            fi
            it=$(($it + 1))
        done < $f
    fi
done 

echo "===============END OF TEST=============="
echo ""
if [ "$success" -eq "$it" ]; then
    printf "${BLINK}${GREEN}Perfect${END}\n"
else
    printf "${LINE}${GREEN}Success${END} : $success\n"
    printf "${LINE}${RED}Failed${END} : $(($it - $success))\n"
fi
echo ""
printf "${LINE}All test${END} : $it\n"
