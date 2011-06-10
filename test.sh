#!/bin/bash

CMD=./lispkit

for f in `ls tests/execute-??`
do
  echo "Running function test $f"
  $CMD $f $f-args
  if [ $? -ne 0 ]; then exit -1; fi;
done

for f in `ls tests/compile-??`
do
  echo "Running compiler test $f"
  if [ ! -f compiler.ascii ]; then echo "no compiler" && exit -1; fi;
  $CMD compiler.ascii $f
  if [ $? -ne 0 ]; then exit -1; fi;
done

$CMD compiler.ascii compiler.txt.ascii

