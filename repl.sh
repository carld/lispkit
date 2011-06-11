#! /bin/bash

while echo "enter program (ctrl-d to finish)" && read -p '> ' line;
do
  echo $line | ./lispkit compiler.ascii > .compiled
  cat .compiled
  if [[ ${PIPESTATUS[1]} -eq 0 ]]; then
    echo "now enter argument"
    read -p '> ' args
    echo $args | ./lispkit .compiled
    rm .compiled
  fi;
done

echo
echo "Thanks for playing!"

exit 0

