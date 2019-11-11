/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "Oblique.h"

extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
  //ASSERT(1 == 0);

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
  
  try {
    
    Process::vEntry();
    
    // parse the command line arguments
    if (! gxArguments.bParse(argc,(Stz**)argv))
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
 
 b) start the cluster
 
    se> startCluster;
 
 c) For debug, in ../Seth run (for example, to run all tests):
 
    $ ./allTests
 
 d) For debug, in ../Seth run (for example, to run a specific set of tests):
 
    $ ./seth.sh tests/Tpch/customer.test

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
 Seth Testing (release):
 
 a) start SE with at least:
 
    --start --prompt --metadata --partitionsPerNode 4 --node NODE1
 
 b) start the cluster
 
    se> startCluster;
 
 c) For release, in ../Seth run (for example, to run all tests):
 
    $ ./allTestsRel
 
 d) For release, in ../Seth run (for example, to run a specific set of tests):
 
    $ ./sethRel.sh tests/Tpch/customer.test
 
 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
