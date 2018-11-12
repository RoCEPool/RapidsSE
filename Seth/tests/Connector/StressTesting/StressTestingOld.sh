export SE_PATH=~/master/rapidsse/Linux
export LD_LIBRARY_PATH=$SE_PATH/bin
BASIC_DUMMY_THREADS=200
SETH_PATH=~/master/rapidsse/Seth
MASTER=~/master

for iteration in {1..1000}
do

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
    cd ~/master/current
    cp ../cluster.config cfg
    cp ../zk.config cfg
    ./bootstrapper.sh -a install -i ../$1

    # copy new SE
    ~/master/cp.sh

    echo "########################################################"
    echo "ITERATION $iteration NODES $NODES PARTITIONS $PARTITIONS"
    echo "########################################################"

    //for i in {1..1}
      //do
        //echo "Welcome $i times"


        # stop , cleanup and start RDP
        cd ~/master/current
        ./bootstrapper.sh -a stop
        ./bootstrapper.sh -a remove_logs
        rm /tmp/*

        # kill any dummyThread SE's that are running
        killall rapidsse

        ./bootstrapper.sh -a start

        # start an SE with dummy threads
        let DUMMY_THREADS=$(( $BASIC_DUMMY_THREADS + $(( $iteration % 100 )) ))
        echo "Starting $DUMMY_THREADS dummy threads"
        cd $SE_PATH
        bin/rapidsse --name NODE10 --node NODE10 127.0.0.1 6666 --start --munged --dummyThreads $DUMMY_THREADS &

        cd $SETH_PATH

        echo "Regression Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Regression/testList

        rm /tmp/*
        cd ~/master/current
        ./bootstrapper.sh -a stop
        ./bootstrapper.sh -a remove_logs
        ./bootstrapper.sh -a start

        cd $SETH_PATH
        echo "InsertSelect Iteration $j of $i"
        ./dbth.sh -f tests/Connector/InsertSelect/testList

        rm /tmp/*
        cd ~/master/current
        ./bootstrapper.sh -a stop
        ./bootstrapper.sh -a remove_logs
        ./bootstrapper.sh -a start

        cd $SETH_PATH
        echo "Load Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Load/testList

        rm /tmp/*
        cd ~/master/current
        ./bootstrapper.sh -a stop
        ./bootstrapper.sh -a remove_logs
        ./bootstrapper.sh -a start

        cd $SETH_PATH
        echo "Select Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Select/testList
        echo "Table Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Table/testList
        echo "Insert Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Insert/testList

        rm /tmp/*
        cd ~/master/current
        ./bootstrapper.sh -a stop
        ./bootstrapper.sh -a remove_logs
        ./bootstrapper.sh -a start

        cd $SETH_PATH
        echo "Tpch Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Tpch/testList

      //done
      cd ~/master/current
      ./bootstrapper.sh -a stop
    done

  done
done
