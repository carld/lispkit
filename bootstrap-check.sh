#!/usr/bin/env bash -x

#  Po = m(Co, Ps)
#  Co = m(Co, Cs)
#
#  Po = Program object
#  Co = Compiler object
#  Cs = Compiler source
#  Ps = Program source

LISPKIT=./lispkit
COMPILER=compiler.secd
SOURCE=compiler.lisp

function m {
  local Co=$1
  local Ps=$2
  local Po=$3
  $LISPKIT $Co $Ps | fmt -w 80 > $Po
}

function d {
  diff -y -d $1 $2
  if [ $? -ne 0 ]; then
    echo "ERROR: $1 and $2 are unstable"
    exit 1;
  fi
}

function bootstrap {
  local Co=$1
  local Ps=$2
  local Po=${Ps/.lisp/.secd}
  local CPo=${Po}.bootstrap
  m $Co $Ps $Po    # compile compiler with earlier compiler
  m $Po $Ps $CPo   # compile compiler with itself
  d $Po $CPo       # compare the results of above two
  echo "Bootstrap OK"
}

cat $COMPILER | tr '\n' ' ' | fmt -w 80 > compiler0.secd
cat $SOURCE | tr '\n' ' ' | fmt -w 80 > compiler1.lisp

for n in $(seq 0 2); do
  bootstrap compiler$n.secd compiler$((n + 1)).lisp
done
