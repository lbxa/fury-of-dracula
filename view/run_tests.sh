#!/bin/bash

scriptStart=$(date +%s)
Blue=$(tput setaf 4)
Red=$(tput setaf 1)
Green=$(tput setaf 2)
Reset=$(tput sgr0)

testTotal=3
testCount=1
testPass=0

# Set bash time format
TIMEFORMAT='%3Us'

echo "$Blue""[$testCount/$testTotal] ./testDraculaView$Reset"
if ./testDraculaView; then
    echo "$Green""Passing.$Reset"
    testPass=$((testPass+1))
else
  echo "$Red""Failed.$Reset"
fi
testCount=$((testCount+1))

echo "$Blue""[$testCount/$testTotal] ./testGameView$Reset"
if ./testGameView; then
    echo "$Green""Passing.$Reset"
    testPass=$((testPass+1))
else
  echo "$Red""Failed.$Reset"
fi
testCount=$((testCount+1))

echo "$Blue""[$testCount/$testTotal] ./testHunterView$Reset"
if ./testHunterView; then
    echo "$Green""Passing.$Reset"
    testPass=$((testPass+1))
else
  echo "$Red""Failed.$Reset"
fi

echo
echo "  🌈 SUMMARY RESULTS 🌈"
echo
echo "Tests Passed:     $testPass out of $testCount"
echo "Tests Completed:  $testCount out of $testTotal"
echo "Time Taken:       $(($(date +%s) - scriptStart))s"

errorCode=0
if [ $testPass -lt $testCount ]; then
  echo "$Red""Tests Failed:     $((testCount - testPass)) out of $testTotal$Reset"
  errorCode=1
fi

if [ $testCount -lt "$testTotal" ]; then
  echo "$Red""Not all tests could be completed.$Reset"
  errorCode=1
fi

if [ $errorCode -ne 0 ]; then exit $errorCode; fi