#!/bin/bash

for f in `ls tests/execute-??`
do
  echo "Running function test $f"
  ./lispkit $f $f-args
done

for f in `ls tests/compile-??`
do
  echo "Running compiler test $f"
  ./lispkit compiler.code $f
done

