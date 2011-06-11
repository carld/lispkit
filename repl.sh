#! /bin/bash

while read -p '> ' line;
do
  echo $line | ./lispkit compiler.ascii > .compiled
  cat .compiled
  if [[ ${PIPESTATUS[1]} -eq 0 ]]; then
    echo "enter arguments"
    read -p '> ' args
    echo $args | ./lispkit .compiled
    rm .compiled
  fi;
done

echo

exit 0

