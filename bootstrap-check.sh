#!/usr/bin/env bash -x

#  Po = m(Co, Ps)
#  Co = m(Co, Cs)

LISPKIT=./lispkit

function m {
  Co=$1
  Ps=$2
  Po=$3
  $LISPKIT $Co $Ps | fmt -w 80 > $Po
}

function d {
  diff -y -d $1 $2
  if [ $? -ne 0 ]; then
    echo "ERROR: $1 and $2 are unstable"
    exit 1;
  fi
}

cat compiler.lisp | tr '\n' ' ' | fmt -w 80 > compiler1.lisp
cat compiler.secd | tr '\n' ' ' | fmt -w 80 > compiler1.secd

m "compiler1.secd" "compiler1.lisp" "compiler1.secd.out"
d "compiler1.secd" "compiler1.secd.out"
echo "1 OK"
m "compiler1.secd" "compiler2.lisp" "compiler2.secd"
m "compiler2.secd" "compiler2.lisp" "compiler2.secd.out"
d "compiler2.secd" "compiler2.secd.out"
echo "2 OK"
m "compiler2.secd" "compiler3.lisp" "compiler3.secd"
m "compiler3.secd" "compiler3.lisp" "compiler3.secd.out"
d "compiler3.secd" "compiler3.secd.out"
echo "3 OK"

