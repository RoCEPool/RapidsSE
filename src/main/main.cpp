/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "Oblique.h"

#include "Version.h"

#include "emmintrin.h"


extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

extern Args gxArgs;
extern EnvFlags gxEnvFlags;

extern i8   gi_argc;
extern s256 gi_szaArgv[];

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
//  printf("sizeof(Object) %lld\n",       (i8)sizeof(Object));
//  printf("sizeof(Operand) %lld\n",      (i8)sizeof(Operand));
//  printf("sizeof(LinkedBuffer) %lld\n", (i8)sizeof(LinkedBuffer));
//  printf("sizeof(DataBuffer) %lld\n",   (i8)sizeof(DataBuffer));
//  printf("sizeof(Ent) %lld\n",          (i8)sizeof(Ent));
//  printf("sizeof(Ent_gen) %lld\n",      (i8)sizeof(Ent_gen));
//  printf("sizeof(Ent_int) %lld\n",      (i8)sizeof(Ent_int));
//  printf("sizeof(EntWrk) %lld\n",       (i8)sizeof(EntWrk));
//  printf("sizeof(Fact) %lld\n",         (i8)sizeof(Fact));
//  printf("sizeof(LogicUnit) %lld\n",    (i8)sizeof(LogicUnit));
//  printf("sizeof(Method) %lld\n",       (i8)sizeof(Method));
//  printf("sizeof(SP_Ent) %lld\n",       (i8)sizeof(SP_Ent));
//  printf("sizeof(SP_Key) %lld\n",       (i8)sizeof(SP_Key));
//  printf("sizeof(SP_Ref) %lld\n",       (i8)sizeof(SP_Ref));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  Qty xQty;
//  
//  xQty = Datums::xQtyTest();
//  printf("Qty isNull=%lld Value=%lld\n",(i8)xQty.m_bIsNull,xQty.m_iValue);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  size_t size = C_64KB;
//
//  void *prx;
//  char tmpfname[] = "/tmp/.tccrunXXXXXX";
//  int fd = mkstemp(tmpfname);
//  unlink(tmpfname);
//  ftruncate(fd, size);
//
//  prx = mmap (NULL, size, PROT_READ|PROT_EXEC, MAP_SHARED, fd, 0);
//    if (prx == MAP_FAILED)
//      printf("could not map memory");
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some word flip experiments
  
//  char acText[128];
//  char acFlipped[128];
//
//  StrLib::vCopy(acText,"1-URGENT");
//  printf("%s\n",acText);
//  StrLib::iWordFlip(acFlipped,acText);
//  StrLib::iWordFlipBack(acText,acFlipped);
//  printf("%s\n",acText);
//
//  StrLib::vCopy(acText,"2-HIGH");
//  printf("%s\n",acText);
//  StrLib::iWordFlip(acFlipped,acText);
//  StrLib::iWordFlipBack(acText,acFlipped);
//  printf("%s\n",acText);
//
//  StrLib::vCopy(acText,"3-MEDIUM");
//  printf("%s\n",acText);
//  StrLib::iWordFlip(acFlipped,acText);
//  StrLib::iWordFlipBack(acText,acFlipped);
//  printf("%s\n",acText);
//
//  StrLib::vCopy(acText,"4-NOT SPECIFIED***");
//  printf("%s\n",acText);
//  StrLib::iWordFlip(acFlipped,acText);
//  StrLib::iWordFlipBack(acText,acFlipped);
//  printf("%s\n",acText);
//
//  StrLib::vCopy(acText,"5-LOW");
//  printf("%s\n",acText);
//  StrLib::iWordFlip(acFlipped,acText);
//  StrLib::iWordFlipBack(acText,acFlipped);
//  printf("%s\n",acText);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Engine::vEnvFlags();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //ThirdParty::libtcc_rjh(0,argv);
  
  gsBuildGitHash  = (Stz*)C_BUILD_GIT_HASH;
  gsBuildDate     = (Stz*)C_BUILD_DATE;
  gsBuildHostName = (Stz*)C_BUILD_HOSTNAME;
  gsBuildUserName = (Stz*)C_BUILD_USERNAME;
  gsBuildBranch   = (Stz*)C_BUILD_BRANCH;
    
  gi_szaArgv[0] = (Stz*)argv[0];
  
  if (argc> 1) {
    for (i8 i_Arg = 0; i_Arg < argc; i_Arg++)
      gi_szaArgv[i_Arg] = (Stz*)argv[i_Arg];
    gi_argc = argc;
  }
  else {
    s32 sVarName;
    s256 sSeArgs;
    sVarName = "SE_ARGS"_Stz;
    if (! Env::bGet(sVarName,sSeArgs)) {
      sSeArgs =
        "--start "
        "--prompt "
        "--verbose "
        "--developer "
        "--killPrev "
        "--name NODE0 "
        "--node NODE0 127.0.0.1 5540 "
        //"--node NODE1 127.0.0.1 5541 "
        //"--node NODE2 127.0.0.1 5542 "
        //"--node NODE3 127.0.0.1 5543 "
        "--log "
        ""_Stz;
    }
    vTrace("Args: %s\n"_Stz,sSeArgs.szStz());
    
    gi_argc = Args::nBurstArgs(gi_szaArgv, sSeArgs);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  try {
    
    // force bamboo build
    
    Process::vEntry();
    
    // parse the command line arguments
    if (! gxArgs.bParse(gi_argc,gi_szaArgv))
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
 
 c) For debug, in ../seth run (for example, to run all tests):
 
    $ ./seth.sh
 
 d) For debug, in ../seth run (for example, to run a specific set of tests):
 
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
 
 c) For release, in ../seth run (for example, to run all tests):
 
    $ ./sethRel.sh
 
 d) For release, in ../seth run (for example, to run a specific set of tests):
 
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
