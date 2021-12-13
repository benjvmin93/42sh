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
refshell=bash
it=0
oldit=0
success=0


nb_echo=0
nb_for=0
nb_if=0
nb_redir=0
nb_variables=0
nb_while=0
nb_andor=0

fail_echo=0
fail_for=0
fail_if=0
fail_redir=0
fail_variables=0
fail_while=0
fail_andor=0

FILES="../tests/dir_unique_tests/*"

for f in $FILES; do
    if [ "$f" != "../tests/test.sh" -a "$f" != "../tests/unique_test.sh" ]; then
        echo "==================================================$f=================================================="
        echo ""
        while read expr; do
            echo "-----------"
            printf "TEST[${it}]:\n"
            testres=$(${test} -c "${expr}")
            codetest=$?

            if [ "$f" = "../tests/test_echo" ]; then
                refres=$(${refshell} -c "${expr}")
            else
                refres=$(${ref} -c "${expr}")
            fi

            coderef=$?
            if [ "$testres" != "$refres" ]; then
                printf "[${BLINK}${RED}FAILED${END}] ${expr}\n"
                echo ""
                printf "\tgot: \n%b\n\texpected: \n%b\n" "$testres" "$refres"
                echo ""

                if [ "$f" = "../tests/test_echo" ]; then
                    fail_echo=$(($fail_echo + 1))
                elif [ "$f" = "../tests/test_for" ]; then
                    fail_for=$(($fail_for + 1))
                elif [ "$f" = "../tests/test_if" ]; then
                    fail_if=$(($fail_if + 1))
                elif [ "$f" = "../tests/test_redir" ]; then
                    fail_redir=$(($fail_redir + 1))
                elif [ "$f" = "../tests/test_variables" ]; then
                    fail_variables=$(($fail_variables + 1))
                elif [ "$f" = "../tests/test_while" ]; then
                    fail_while=$(($fail_while + 1))
                elif [ "$f" = "../tests/test_andor" ]; then
                    fail_andor=$(($fail_andor + 1))
                fi
            elif [ "$codetest" -ne "$coderef" ]; then
                printf "[${BLINK}${RED}FAILED code ($codetest != $coderef)${END}] ${expr}\n"
                echo ""
                printf "\tgot: %b\n\texpected: %b\n" "$codetest" "$coderef"
                echo ""
                if [ "$f" = "../tests/test_echo" ]; then
                    fail_echo=$(($fail_echo + 1))
                elif [ "$f" = "../tests/test_for" ]; then
                    fail_for=$(($fail_for + 1))
                elif [ "$f" = "../tests/test_if" ]; then
                    fail_if=$(($fail_if + 1))
                elif [ "$f" = "../tests/test_redir" ]; then
                    fail_redir=$(($fail_redir + 1))
                elif [ "$f" = "../tests/test_variables" ]; then
                    fail_variables=$(($fail_variables + 1))
                elif [ "$f" = "../tests/test_while" ]; then
                    fail_while=$(($fail_while + 1))
                elif [ "$f" = "../tests/test_andor" ]; then
                    fail_andor=$(($fail_andor + 1))
                fi
            else
                success=$(($success + 1))
                printf "[${BLINK}${GREEN}SUCCESS${END}] ${expr}\n"
            fi
            it=$(($it + 1))
        done < $f
        if [ "$f" = "../tests/test_echo" ]; then
            nb_echo=$(($it - $oldit))
        elif [ "$f" = "../tests/test_for" ]; then
            nb_for=$(($it - $oldit))
        elif [ "$f" = "../tests/test_if" ]; then
            nb_if=$(($it - $oldit))
        elif [ "$f" = "../tests/test_redir" ]; then
            nb_redir=$(($it - $oldit))
        elif [ "$f" = "../tests/test_variables" ]; then
            nb_variables=$(($it - $oldit))
        elif [ "$f" = "../tests/test_while" ]; then
            nb_while=$(($it - $oldit))
        elif [ "$f" = "../tests/test_andor" ]; then
            nb_andor=$(($it - $oldit))
        fi
        oldit=$it
    fi
    echo
done 

echo "======================================================END OF TEST======================================================"
echo ""
if [ "$success" -eq "$it" ]; then
    printf "${BLINK}${GREEN}Perfect${END}\n"
else
    printf "fail echo: %b / %b\n" "$fail_echo" "$nb_echo"
    printf "fail for: %b / %b\n" "$fail_for" "$nb_for"
    printf "fail if: %b / %b\n" "$fail_if" "$nb_if"
    printf "fail redir: %b / %b\n" "$fail_redir" "$nb_redir"
    printf "fail variables: %b / %b\n" "$fail_variables" "$nb_variables"
    printf "fail while: %b / %b\n" "$fail_while" "$nb_while"
    printf "fail andor: %b / %b\n" "$fail_andor" "$nb_andor"
    echo
    printf "${LINE}${GREEN}Success${END} : $success\n"
    printf "${LINE}${RED}Failed${END} : $(($it - $success))\n"
fi
echo ""
printf "${LINE}All test${END} : $it\n"
