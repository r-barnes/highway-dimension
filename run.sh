#!/bin/bash

set -e -u

PROPERTIES_FILE=properties.sh
DATA_DIR=data
SRC_DIR=src
IN_SUFFIX=.in
OUT_SUFFIX=.out
GTFS_CONVERTER=./gtfs-parser/gtfs-converter.pl
BIN_FILE=./"${SRC_DIR}"/main.out

if [ ! -f "${PROPERTIES_FILE}" ]; then
  echo "No property file found."
  exit 1
fi

# TODO: skip flag
make -C "${SRC_DIR}" "test"
make -C "${SRC_DIR}"

### GTFS conversion

for city_dir in "${DATA_DIR}"/*; do
  if [ ! -d "${city_dir}" ]; then
    continue
  fi

  city_name=$(basename "$city_dir")
  # echo "Doing a GTFS conversion for ${city_name}."
  # "${GTFS_CONVERTER}" "${city_dir}/${city_name}${IN_SUFFIX}" \
  #   "${city_dir}"/stops.txt "${city_dir}"/stop_times.txt
  echo "Calculating highway dimension for ${city_name}."
  "${BIN_FILE}" "${city_dir}"/"${city_name}${IN_SUFFIX}" \
    "${city_dir}"/"${city_name}${OUT_SUFFIX}"
done

# TODO: do not delete flag
make -C "${SRC_DIR}" clean-all
