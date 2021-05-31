#!/bin/bash

# Generates data for SF1, 10, 100 and 1000.

DIR=$(dirname "$0")

if [ $# -eq 0 ]
then
  echo "Usage:"
  echo "${0} -chunks <total_chunks> -offset <first_chunk>"  
  exit 1
fi

CHUNKS=2
OFFSET=1

while (( "$#" ))
do
  case $1 in
    -chunks)
      shift
      CHUNKS="$1"
      shift
      ;;

    -offset)
      shift
      OFFSET="$1"
      shift
      ;;

    --help)
      echo "./${0} -chunks <total_chunks> -offset <first_chunk>"
      echo "This will generate a chunk for each SSD on the machine, starting at <first_chunk>."
      ;;

    *)
      echo "Unknown option: $1"
      echo "./${0} -chunks <total_chunks> -offset <first_chunk>"
      exit 1
      ;;
  esac
done



"${DIR}/awsGenerateData.sh" -s 1    -chunks "${CHUNKS}" -offset "${OFFSET}"
"${DIR}/awsGenerateData.sh" -s 10   -chunks "${CHUNKS}" -offset "${OFFSET}"
"${DIR}/awsGenerateData.sh" -s 100  -chunks "${CHUNKS}" -offset "${OFFSET}"
"${DIR}/awsGenerateData.sh" -s 1000 -chunks "${CHUNKS}" -offset "${OFFSET}"

"${DIR}/awsRenameData.sh"
