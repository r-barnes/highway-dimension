#!/bin/bash

set -e -u

DATA_DIR=data
GTFS_ZIP=gtfs.zip
SRC_DIR=src
IN_SUFFIX=.in
OUT_SUFFIX=.out
GTFS_CONVERTER=./gtfs-parser/gtfs-converter.pl
BIN_FILE=./"${SRC_DIR}"/main.out

usage() {
  echo "Usage: $0: [-t] [-c] [-d]

Arguments:
  -t     skip testing
  -c     skip compilation
  -d     skip downloading data
  "
}

skip_test_flag=0
skip_compilation_flag=0
skip_delete_flag=0
while getopts tdh argname; do
  case "${argname}" in
    t)
      skip_test_flag=1
      ;;
    c)
      skip_compilation_flag=1
      ;;
    d)
      skip_delete_flag=1
      ;;
    *|h)
      usage
      exit -1
      ;;
  esac
done

if [ "${skip_test_flag}" -ne 1 ]; then
  make -C "${SRC_DIR}" "test"
fi
if [ "${skip_compilation_flag}" -ne 1 ]; then
  make -C "${SRC_DIR}"
fi

feeds=$(find "${DATA_DIR}" -name "${GTFS_ZIP}")
echo "${feeds}"
exit 0

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

if [ "${skip_delete_flag}" -ne 1 ]; then
  make -C "${SRC_DIR}" clean-all
fi
