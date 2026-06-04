#!/bin/bash

status=true
out_file=$(mktemp)

./gauss AB.csv >"${out_file}"

if cmp -s X.csv "${out_file}"; then
  echo Solver ok!
else
  echo Solver failed! >&2
  status=false
fi

rm -f "${out_file}"

${status}
