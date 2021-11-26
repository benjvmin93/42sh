#!/bin/sh

test=./42sh
ref=dash
it=0
success=0

file_if=/testsuite/test_if
file_echo=/testsuite/test_echo


echo "===============TEST FOR ECHO=============="
while read expr; do
    testres=$(${test} ${expr})
    codetest=$?

    refres=$(${ref} ${expr})
    coderef=$?
    echo "TEST[${it}]: ${expr}"
    if [ "$testres" != "$refres" ]; then
        echo "FAILED"
    elif [ "$codetest" -ne "$coderef" ]; then
        echo "FAILED code ($codetest != $coderef)"
    else
        success=$(($success + 1))
        echo "SUCCESS"
    fi
    it=$(($it + 1))
done < $file_echo


echo "===============TEST FOR IF=============="
while read expr; do
    testres=$(${test} ${expr})
    codetest=$?

    refres=$(${ref} ${expr})
    coderef=$?
    echo "TEST[${it}]: ${expr}"
    if [ "$testres" != "$refres" ]; then
        echo "FAILED"
    elif [ "$codetest" -ne "$coderef" ]; then
        echo "FAILED code ($codetest != $coderef)"
    else
        success=$(($success + 1))
        echo "SUCCESS"
    fi
    it=$(($it + 1))
done < $file_if


echo "success : $success"
echo "failed : $(($it - $success))"
echo "all test : $it"
