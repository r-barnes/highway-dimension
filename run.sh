#!/bin/bash

set -e -u

DATA_DIR=data
SRC_DIR=src
IN_SUFFIX=.in
OUT_SUFFIX=.out
BIN_FILE=./"${SRC_DIR}"/main.out

make -C "${SRC_DIR}"

for city_dir in "${DATA_DIR}"/*; do
  if [ ! -d "${city_dir}" ]; then
    continue
  fi

  city_name=$(basename "${city_dir}")
  echo "Calculating highway dimension for ${city_name}."
  "${BIN_FILE}" "${city_dir}"/"${city_name}".in "${city_dir}"/"${city_name}".out
done

# TODO: do not delete flag
make -C "${SRC_DIR}" clean-all
