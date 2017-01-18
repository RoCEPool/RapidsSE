#!/bin/bash

# Get the relative path to this file.
DIR=$(dirname "$0")

NOHUP_FILE="${DIR}/se.log"

#set the library path for the rapidsSE library
export LD_LIBRARY_PATH=./bin:$LD_LIBRARY_PATH

startSE=0
sizeArg=""

ArgCount=$#
print_usage() {
  echo "Usage: se.sh [--start [--size n] | --cli | --stop | --status]"
}

if [[ $ArgCount -eq 0 ]]
then
  print_usage
  exit 1
fi

# Parse any command line arguments
while [[ $# > 0 ]]
do
  arg="$1"

  case $arg in
    --start)
      startSE=1
      ;;

    --size)
      if [[ $ArgCount -ne 3 ]]
      then
        print_usage
        exit 1
      fi
      shift
      sizeArg="--size $1gb"
      ;;

    --cli)
      if [[ $ArgCount -gt 1 ]]
      then
        print_usage
        exit 1
      fi

      # start the se prompt shell
      bin/se --prompt --commands Commands
      ;;

    --stop)
      if [[ $ArgCount -gt 1 ]]
      then
        print_usage
        exit 1
      fi
      # stop se
      bin/se --stop
      ;;

    --status)
      if [[ $ArgCount -gt 1 ]]
      then
        print_usage
        exit 1
      fi
      # get the status of se
      bin/se --status
      ;;

    *)
      echo "Unknown option: $arg" | tee -a "$NOHUP_FILE"  # unknown option
      print_usage
      exit 1
      ;;
  esac
  shift
done

   if [ ${startSE} -eq 1 ]
   then

      if [[ $ArgCount > 3 ]]
      then
        print_usage
        exit 1
      fi

      # Remove the nohup.out file if it exists so we start clean and don't keep accumulating into this file.
      rm "$NOHUP_FILE" &>/dev/null
   
      # start se
      nohup bin/se  --start $sizeArg --commands Commands $* >> "$NOHUP_FILE" 2>&1 &
      # get the last pid and save it to a file
      PID=$! 

      echo "Rapids SE has been started as a background process (pid $PID)."
      echo "check $NOHUP_FILE for status."
   fi
