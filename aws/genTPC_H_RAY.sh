#!/bin/bash
 
# Note: dbgen seems to corrupt data when we generate parallelised
#       chunks for all tables at once. However if we do it for
#       each table individually it seems ok.
 
SF=10
OUTPUT_DIRS=("/Users/ray/aws_sim/mnt/disk1/tpch" "/Users/ray/aws_sim/mnt/disk2/tpch" "/Users/ray/aws_sim/mnt/disk3/tpch" "/Users/ray/aws_sim/mnt/disk4/tpch")
DBGEN="/Users/ray/rapidsse/dbgen/dbgen_MAC/dbgen"
DISTS="$(dirname ${DBGEN})/dists.dss"
 
 
NUM_CHUNKS="${#OUTPUT_DIRS[@]}"
CHUNK_ARR=( $(seq -s ' ' 1 "${NUM_CHUNKS}") )
 
CHUNK_TABLES=("customer" "lineitem" "orders" "part" "partsupp" "supplier")
NON_CHUNK_TABLES=("nation" "region")
 
declare TABLE_PARAM
TABLE_PARAM["customer"]="c"
TABLE_PARAM["lineitem"]="L"
TABLE_PARAM["orders"]="O"
TABLE_PARAM["part"]="P"
TABLE_PARAM["partsupp"]="S"
TABLE_PARAM["supplier"]="s"
TABLE_PARAM["nation"]="n"
TABLE_PARAM["region"]="r"
 
# Make the directories
for dir in "${OUTPUT_DIRS[@]}"; do
  mkdir -p "${dir}"
done
 
# Generate the data for all the chunkable tables
for table in "${CHUNK_TABLES[@]}"; do
  echo
  echo "Generating SF${SF} data for the ${table} table"
  echo
  for i in "${CHUNK_ARR[@]}"; do
    pushd "$(pwd)" > /dev/null
    cd "${OUTPUT_DIRS[$((i-1))]}"
   "${DBGEN}" -q -f -s "${SF}" -b "${DISTS}" -C "${NUM_CHUNKS}" -S "${i}"  -T "${TABLE_PARAM[${table}]}" &
    popd > /dev/null
  done
 
  # Wait for all chunks of the table to be generated
  wait
done
 
# Generate the data for all the non-chunkable tables
for table in "${NON_CHUNK_TABLES[@]}"; do
  echo
  echo "Generating SF${SF} data for the ${table} table"
  echo
  pushd "$(pwd)" > /dev/null
  cd "${OUTPUT_DIRS[0]}"
  "${DBGEN}" -q -f -s "${SF}" -b "${DISTS}" -T "${TABLE_PARAM[${table}]}"
  popd > /dev/null
done