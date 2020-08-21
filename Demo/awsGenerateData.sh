#!/bin/bash

DEBUG=0

if [ "${DEBUG}" -eq 1 ]
then
  set -x 
fi

DIR=$(dirname "$0")
ABS_DIR=$(readlink -f "${DIR}")   # absolute directory

SE_DIR="${ABS_DIR}/.."
DBGEN_DIR="${SE_DIR}/dbgen"
DBGEN="${DBGEN_DIR}/dbgen"
DISTS="${DBGEN_DIR}/dists.dss"

# Get the number of local disks
DISKS=($(find /mnt -type d -name "disk*"))
NUM_DISKS="${#DISKS[@]}"

if [ "${NUM_DISKS}" -eq 0 ]
then
  echo "Could not find any disks mounted as /mnt/disk<num>"
  exit 1
fi

echo "Found ${NUM_DISKS} disk(s): ${DISKS[*]}"

# Defaults
SF=10
CHUNKS="${NUM_DISKS}"
OFFSET=1

if [ $# -eq 0 ]
then
  echo "Usage:"
  echo "${0} -s <scale_factor> -chunks <total_chunks> -offset <first_chunk>"  
  exit 1
fi

while (( "$#" ))
do
  case $1 in
    -s)
      shift
      SF="$1"
      shift
      ;;

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
      echo "./${0} -s <scale_factor> -chunks <total_chunks> -offset <first_chunk>"
      echo "This will generate a chunk for each SSD on the machine, starting at <first_chunk>."
      ;;

    *)
      echo "Unknown option: $1"
      echo "./${0} -s <scale_factor> -chunks <total_chunks> -offset <first_chunk>"
      exit 1
      ;;
  esac
done

echo "Generating data for SF${SF} for chunks $(seq "${OFFSET}" $((OFFSET + ${#DISKS[@]} -1)) | xargs) of ${CHUNKS}"

index="${OFFSET}"

for disk in "${DISKS[@]}"
do
  # Generate the data in a background sub-shell.
  (
    mkdir -p "${disk}/tpch/sf${SF}"
    cd "${disk}/tpch/sf${SF}" || exit
    
    echo "$(date) : Generating chunk #${index} for customers table..."
    "${DBGEN}" -b "${DISTS}" -f -q -s "${SF}" -C "${CHUNKS}" -S "${index}" -T c || exit
    echo "$(date) : Generation of chunk #${index} for customers table complete."
    
    echo "$(date) : Generating chunk #${index} for orders table..."
    "${DBGEN}" -b "${DISTS}" -f -q -s "${SF}" -C "${CHUNKS}" -S "${index}" -T O || exit
    echo "$(date) : Generation of chunk #${index} for orders table complete."
        
    echo "$(date) : Generating chunk #${index} for lineitem table..."
    "${DBGEN}" -b "${DISTS}" -f -q -s "${SF}" -C "${CHUNKS}" -S "${index}" -T L || exit
    echo "$(date) : Generation of chunk #${index} for lineitem table complete."
    
    echo "$(date) : Generation of chunk #${index} complete for customer/orders/lineitem tables."
  ) &
  
  index=$((index + 1))
done

# wait until the background sub-shells have finished
wait

echo "Data generation complete. Refer to the following directories:"
for disk in "${DISKS[@]}"
do
  echo "  ${disk}/tpch/sf${SF}"
done

if [ "${DEBUG}" -eq 1 ]
then
  set +x
fi