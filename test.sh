#!/bin/sh

CMD=./lispkit
COMPILER_SRC=./compiler.lisp
COMPILER_OBJ=./compiler.secd

for f in `ls tests/execute-??`
do
  echo "Running function test $f ..."
  $CMD $f $f-args
  if [ $? -ne 0 ]; then exit -1; fi;
done

for f in `ls tests/compile-??`
do
  echo "Running compiler test $f ..."
  if [ ! -f $COMPILER_OBJ ]; then echo "no compiler" && exit -1; fi;
  $CMD $COMPILER_OBJ $f
  if [ $? -ne 0 ]; then exit -1; fi;
done

echo "Bootstrapping ..."
$CMD $COMPILER_OBJ $COMPILER_SRC

