# defines
SE_PATH=~/master/rapidsse/Linux
BASIC_DUMMY_THREADS=200
SETH_PATH=~/master/rapidsse/Seth
MASTER=~/master
CURRENT=~/master/current

# export the LIBRARY PATH
export LD_LIBRARY_PATH=$SE_PATH/bin

# for no of nodes
for nodes in {1..4}
do

  echo "NODES $nodes"

  # for no of partitions
  for partitions in {1..4}
  do

    echo "PARTITIONS $partitions"

    # cp cluster file
    cp $SETH_PATH/tests/Connector/StressTesting/cluster.config${nodes}N_${partitions}P $MASTER/cluster.config

    #install rdp
    cd $CURRENT
    cp ../cluster.config cfg
    cp ../zk.config cfg
    ./bootstrapper.sh -a install -i ../$1

    # copy new SE
    $MASTER/cp.sh

    # stop RDP
    cd $CURRENT
    ./bootstrapper.sh -a stop

    # cleanup previous run
    ./bootstrapper.sh -a remove_logs
    rm /tmp/*

    # start RDP
    ./bootstrapper.sh -a start

    for iteration in {1..50}
    do
      echo "########################################################"
      echo "ITERATION $iteration NODES $nodes PARTITIONS $partitions"
      echo "########################################################"

      # start an SE with dummy threads
      let DUMMY_THREADS=$(( $BASIC_DUMMY_THREADS + $(( $iteration % 100 )) ))
      cd $SE_PATH

      echo "Starting $DUMMY_THREADS dummy threads"
      bin/rapidsse --name NODE10 --node NODE10 127.0.0.1 6666 --start --munged --dummyThreads $DUMMY_THREADS &

      # get the last pid and save it
      DUMMYSE_PID=$!

      cd $SETH_PATH

      //echo "Starting RDP Seth Tests....."
      echo "InsertSelect tests"
      ./dbth.sh -f tests/Connector/InsertSelect/testList

      # kill any dummyThread of SE's that are running
      kill -9 $DUMMYSE_PID

    done

    cd $CURRENT
    ./bootstrapper.sh -a stop

  done

done
