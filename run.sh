#!/bin/bash

set -e -u

DATA_DIR=data
GTFS_ZIP=gtfs.zip
SRC_DIR=src

export INPUT_NAME='input.in'
export OUTPUT_NAME='output.res'

export GTFS_CONVERTER=./gtfs-parser/gtfs-converter.pl
DOWNLOADER=./data/downloader.pl
BIN_FILE=./"${SRC_DIR}"/main.out

usage() {
  echo "Usage: $0: [-t] [-c] [-b] [-d] [-z] [-h]

Arguments:
  -t     skip testing
  -c     skip compilation
  -b     skip deleting binary files
  -d     skip downloading data
  -z     skip unzipping gtfs.zip
  -h     print this help
  "
}

unzip_wrapper() {
  zipfile="${1}"
  destination="${2}"

  7z x -o"${destination}" "${zipfile}"
}
export -f unzip_wrapper

skip_test_flag=0
skip_compilation_flag=0
skip_delete_flag=0
skip_downloading_flag=0
skip_unzip_flag=0
while getopts tcbdzh argname; do
  case "${argname}" in
    t)
      skip_test_flag=1
      ;;
    c)
      skip_compilation_flag=1
      ;;
    b)
      skip_delete_flag=1
      ;;
    d)
      skip_downloading_flag=1
      ;;
    z)
      skip_unzip_flag=1
      ;;
    *|h)
      usage
      exit -1
      ;;
  esac
done
export skip_unzip_flag

if [ "${skip_test_flag}" -ne 1 ]; then
  make -C "${SRC_DIR}" "test"
fi
if [ "${skip_compilation_flag}" -ne 1 ]; then
  make -C "${SRC_DIR}"
fi
if [ "${skip_downloading_flag}" -ne 1 ]; then
  "${DOWNLOADER}"
fi

process_feed() {
  full_path="${1}"
  city_dir=$(dirname "${full_path}")

  if [ "${skip_unzip_flag}" -ne 1 ]; then
    unzip_wrapper "${full_path}" "${city_dir}"
  fi

  echo "Doing GTFS conversion for ${city_dir}."
  "${GTFS_CONVERTER}" "${city_dir}/${INPUT_NAME}" "${city_dir}/stops.txt" \
    "${city_dir}/stop_times.txt"
  if [ $? -ne 0 ]; then
    exit 2
  fi

}
export -f process_feed

find "${DATA_DIR}" -name "${GTFS_ZIP}" -ok bash -c 'process_feed "$0"' {} \;

calculate_hd() {
  fullpath="${1}"
  city_dir=$(dirname "${full_path}")

  echo "Calculating highway dimension for ${city_dir}."
  "${BIN_FILE}" "${city_dir}/${INPUT_NAME}" "${city_dir}/${OUTPUT_NAME}"
}

if [ "${skip_delete_flag}" -ne 1 ]; then
  make -C "${SRC_DIR}" clean-all
fi

exit 0
