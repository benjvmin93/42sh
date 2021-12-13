#!/bin/sh

GREEN="\e[42m"
RED="\e[41m"
END="\e[0m"
BLINK="\e[5m"
LINE="\e[1;4m"

test="../builddir/42sh"
i=0

testArgSuccess=0
testInputSuccess=0
testScriptSuccess=0

testArgFailed=0
testInputFailed=0
testScriptFailed=0

test_arg()
{
    printf "${LINE}Test as argument:${END} ./${test} -c \"$*\"\n\n"
        
        testArg=$(${test} -c "$*")
        testArgRes=$?

        refArg=$(${ref} -c "$*")
        refArgRes=$?

        if [ "$testArg" != "$refArg" ]; then
            printf "${BLINK}${RED}[FAILED]${END}'\n\n"
            printf "\tgot: \n$testArg\n\texpected: \n$refArg\n\n"
            testArgFailed=$(($testArgFailed + 1))
        elif [ "$testArgRes" != "$refArgRes" ]; then
            printf "[${BLINK}${RED}FAILED code ($testArgRes != $refArgRes)${END}]\n\n"
            printf "\tgot: $testArgRes\n\texpected: $refArgRes\n\n"
            testArgFailed=$(($testArgFailed + 1))
        else
            testArgSuccess=$(($testArgSuccess + 1))
            printf "[${BLINK}${GREEN}SUCCESS${END}]\n\n"
        fi
}

test_input()
{
    echo "${LINE}Test as input:${END} ./${test} < \"$*\"\n\n"
        testInput="$(${test} < expr)"
        testInputRes=$?

        refInput="$(${ref} < expr)"
        refInputRes=$?
        
        if [ "$testInput" != "$refInput" ]; then
            
            printf "${BLINK}${RED}[FAILED]${END}\n\n" && printf "\tgot: \n$testInput\n\texpected: \n$refInput\n\n"
            testInputFailed=$(($testInputFailed + 1))
        elif [ "$testInputRes" != "$refInputRes" ]; then
            printf "[${BLINK}${RED}FAILED code ($testInputRes != $refInputRes)${END}]\n\n"
            printf "\tgot: $testInputRes\n\texpected: $refInputRes\n\n"
            testInputFailed=$(($testInputFailed + 1))
        else
            testInputSuccess=$(($testInputSuccess + 1))
            printf "[${BLINK}${GREEN}SUCCESS${END}]\n\n"
        fi
}

test_script()
{
    printf "${LINE}Test as file script:${END} ./${test} $1\n\n"
        
        testScript="$(${test} $1)"
        testScriptRes=$?

        refScript="$(${ref} $1)"
        refScriptRes=$?
        
        if [ "$testScript" != "$refScript" ]; then
            printf "${BLINK}${RED}[FAILED]${END}'\n\n" && printf "\tgot: \n$testScript\n\texpected: \n$refScript\n\n"
            testScriptFailed=$(($testScriptFailed + 1))
        elif [ "$testScriptRes" != "$refScriptRes" ]; then
            printf "[${BLINK}${RED}FAILED code ($testScriptRes != $refScriptRes)${END}]\n\n"
            printf "\tgot: $testScriptRes\n\texpected: $refScriptRes\n\n"
            testScriptFailed=$(($testScriptFailed + 1))
        else
            testScriptSuccess=$(($testScriptSuccess + 1))
            printf "[${BLINK}${GREEN}SUCCESS${END}]\n\n"
        fi
}

execute_test()
{
    if [ "$1" = "test_echo" ]; then
        ref=bash
    else
        ref=dash
    fi
    
    while read expr; do
        echo "===================================================================================================================================================================="
        echo "TEST[${i}]:\n"

        test_arg $expr

        echo ${expr} > expr 
        test_input $expr

        rm expr 
        
        echo "===================================================================================================================================================================="
        i=$(($i + 1))
    done < $1
}






echo "
██   ██ ██████  ███████ ██   ██ 
██   ██      ██ ██      ██   ██ 
███████  █████  ███████ ███████ 
     ██ ██           ██ ██   ██ 
     ██ ███████ ███████ ██   ██                        
"
if [ $# -eq 0 ]; then
    echo "Unique test: Usage: ./unique_test.sh [FILE]"
    exit;
fi

echo "==============================================================================TEST START============================================================================="


execute_test $1;
test_script $1;

rm -f foo bar expr


echo "
██   ██ ██████  ███████ ██   ██ 
██   ██      ██ ██      ██   ██ 
███████  █████  ███████ ███████ 
     ██ ██           ██ ██   ██ 
     ██ ███████ ███████ ██   ██                        
"
echo "==============================================================================END OF TEST============================================================================="

printf "\n${LINE}TOTAL FAILED:${END}\n"
printf "\tTest as arg failed: $testArgFailed/$i\n"
printf "\tTest as input failed: $testInputFailed/$i\n"
printf "\tTest as script failed: $testScriptFailed/1\n\n"

printf "${LINE}TOTAL SUCCESS:${END}\n"
printf "\tTests as arg succeed: ${testArgSuccess}/${i}\n"
printf "\tTests as input succeed: ${testInputSuccess}/${i}\n"
printf "\tTests as script succeed: ${testScriptSuccess}/1\n"
