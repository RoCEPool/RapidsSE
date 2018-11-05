export SE_PATH=~/master/rapidsse/Linux
export LD_LIBRARY_PATH=$SE_PATH
BASIC_DUMMY_THREADS=200
SETH_PATH=~/master/rapidsse/Seth
MASTER=~master

# for no of nodes
for nodes in {1..4}
do

  # for no of partitions
  for partitions in {1..4}
  do

  # cp cluster file
  cp $SETH_PATH/tests/Connector/StressTesting/cluster.config${nodes}N_${partitions}P $Master/cluster.config

  #install rdp
  ./run.sh $1

    for i in {1..100}
    do
      echo "Welcome $i times"

      # stop , cleanup and start RDP
      cd ~/master/current
      ./bootstrapper.sh -a stop
      ./bootstrapper.sh -a remove_logs
      rm /tmp/*

      # kill any dummyThread SE's that are running
      killall rapidsse

      ./bootstrapper.sh -a start

      # start an SE with dummy threads
      let DUMMY_THREADS=$(( $BASIC_DUMMY_THREADS + $(( $i % 100 )) ))
      echo "Starting $DUMMY_THREADS dummy threads"
      cd $SE_PATH
      bin/rapidsse --name NODE10 --node NODE10 127.0.0.1 6666 --start --munged --dummyThreads $DUMMY_THREADS &

      cd $SETH_PATH
#      for j in {1..10}
      do
      echo "Regression Iteration $j of $i"
        ./dbth.sh -f tests/Connector/Regression/testList
      done

    done
    ./bootstrapper.sh -a stop
  done

done
*/
