/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "Oblique.h"

#include "Version.h"

extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

extern Args  gxArgs;

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  //ASSERT(1 == 0);

//  printf("sizeof(RowSingle)   %lld\n", (i8)sizeof(RowSingle));
//  printf("sizeof(Table)       %lld\n", (i8)sizeof(Table));
//  printf("sizeof(Index)       %lld\n", (i8)sizeof(Index));
//  printf("sizeof(Dic)         %lld\n", (i8)sizeof(Dic));
//  printf("sizeof(Token)       %lld\n", (i8)sizeof(Token));
//  printf("sizeof(TokenVec)    %lld\n", (i8)sizeof(TokenVec));
//  printf("sizeof(Session)     %lld\n", (i8)sizeof(Session));

//  printf("sizeof(Column)       %lld\n", (i8)sizeof(Column));
//  printf("sizeof(LinkedBuffer) %lld\n", (i8)sizeof(LinkedBuffer));
//  printf("sizeof(HeapBuffer)   %lld\n", (i8)sizeof(HeapBuffer));
//  printf("sizeof(s32)          %lld\n", (i8)sizeof(s32));
//  printf("sizeof(s4K)          %lld\n", (i8)sizeof(s4K));
//  printf("sizeof(ShortCut)     %lld\n", (i8)sizeof(ShortCut));

//  s32 sString;
//  
//  sString.pString()->vCopy("abcd"_Stz);
//  printf("  %s\n",sString.azChar());
//  sString.pString()->vCopy("012345678901234567890123456789abcdefghijk"_Stz);
//  printf("  %s\n",sString.azChar());

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  gsBuildGitHash  = (Stz*)C_BUILD_GIT_HASH;
  gsBuildDate     = (Stz*)C_BUILD_DATE;
  gsBuildHostName = (Stz*)C_BUILD_HOSTNAME;
  gsBuildUserName = (Stz*)C_BUILD_USERNAME;
  gsBuildBranch   = (Stz*)C_BUILD_BRANCH;
  
  int nCmdLineArgs = 0;
  Stz* szaCmdLineArg[128];
  
  if (argc) {
    for (i8 i_Arg = 0; i_Arg < argc; i_Arg++)
      szaCmdLineArg[i_Arg] = (Stz*)argv[i_Arg];
    nCmdLineArgs = argc;
  }
  else {
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  try {
    
    // force bamboo build
    
    Process::vEntry();
    
    // parse the command line arguments
    if (! gxArgs.bParse(nCmdLineArgs,szaCmdLineArg))
      Process::vExitFailure();
    
    // potentially run utilities
    if (Engine::bRunUtilities(argc,(Stz**)argv))
      Process::vExitSuccess();
    
    // execute
    Engine::vMainEntry();
  }
  
  catch (Exception xException) {
    
        fflush(stderr); fflush(stdout);
    printf("Main-level exception caught in func %s() file %s line %d\n",
           __func__,__FILE__,__LINE__);
        fflush(stderr); fflush(stdout);
    xException.vPrint();
        fflush(stderr); fflush(stdout);
    
    Process::vExitFailure();
  }

  return EXIT_SUCCESS;
}

/*
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
 Desktop/rapidsse/OS X/Build/Products/Debug
 
 ./SE --start --killPrev --verbose --subloaders 1 --size 10GB --name NODE0 --node NODE0 127.0.0.1 5550 --log

 ./SE --start --killPrev --verbose --subloaders 1 --size 10GB --name NODE1 --node NODE1 127.0.0.1 5551 --node NODE2 127.0.0.1 5552 --log --prompt --developer

 to run prompt:

 ./SE --prompt --name NODE2 --log --developer
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
 in /Users/ray/Desktop/rapidsse/OS X/RapidsSE.xcodeproj/project.xcworkspace/xcuserdata/rayh.xcuserdatad
 
 $ rm UserInterfaceState.xcuserstate
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 Seth Testing (debug):
 
 a) start SE with at least:
 
    --start --prompt --metadata --partitionsPerNode 4 --node NODE1
 
    NOTE: NOT TO run with --developer
 
 b) start the cluster
 
    se> _bootCluster
 
 c) For debug, in ../Seth run (for example, to run all tests):
 
    $ ./seth.sh
 
 d) For debug, in ../Seth run (for example, to run a specific set of tests):
 
    $ ./sethTest.sh tests/Tpch/customer.test
 
 e) To execute a testList file
 
    $ ./sethTest.sh -f tests/Tpch/testList
 
 f) To execute operator tests
 
    $ ./sethTest.sh -f tests/Operators/testList
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
 Seth Testing (release):
 
 a) start SE with at least:
 
    --start --prompt --metadata --partitionsPerNode 4 --node NODE1
 
 b) start the cluster
 
    se> _bootCluster;
 
 c) For release, in ../Seth run (for example, to run all tests):
 
    $ ./sethRel.sh
 
 d) For release, in ../Seth run (for example, to run a specific set of tests):
 
    $ ./sethTestRel.sh tests/Tpch/customer.test
 
 e) To execute a testList file
 
    $ ./sethTestRel.sh -f tests/Tpch/testList
 
 f) To execute operator tests
 
    $ ./sethTestRel.sh -f tests/Operators/testList
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 /tenants/boray/catalogs/rapidsse/schemas/public/tables
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
 -start
 --commands /users/ray/rapidsse/commands
 --command _bootCluster
 --prompt
 --metadata
 --killPrev
 --partitionsPerNode 1
 --name NODE0
 --verbose
 --developer
 --size 4GB
 --threads 32
 --log
 --noMunge
 --node NODE0 127.0.0.1 5541
 --node NODE1 127.0.0.1 5541
 --node NODE2 127.0.0.1 5552
 --node NODE3 127.0.0.1 5553
 --address 0
 --address 0x304800000000
 --amPath /rapids/ActivityLog.csv
 --bindThreads
 --command pwd
 --docol
 --dontFree
 --dummyThreads 64
 --entries 10
 --limitThread 3
 --maxLoadErrors 25
 --maxQueue 100
 --munged
 --periodicSleep 3
 --progress
 SHM
 --sleep 1
 --subIngesters 1
 --subloaders 1
 --touch
 --tracepoint
 --unittests
 --node node1 127.0.0.1 5401 --node node2 127.0.0.1 5402 --node node3 127.0.0.1 5403
 --node node4 127.0.0.1 5404 --node node5 127.0.0.1 5405 --node node6 127.0.0.1 5406
 --node node7 127.0.0.1 5407 --node node8 127.0.0.1 5408 --node node9 127.0.0.1 5409
 --node node10 127.0.0.1 5410 --node node11 127.0.0.1 5411 --node node12 127.0.0.1 5412


 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
