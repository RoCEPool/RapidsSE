/*============================================================================*/
/* main.cpp -- main entry                                                     */
/*============================================================================*/

#include <sys/time.h>
#include "BASE.h"

#include "Version.h"

#include "CmdLineArgs.hpp"

//#include "Heart.h"
#include "Engine.hpp"
#include "Mersenne.hpp"

// Since trace file is opened before the attach map is ready
// we use globals instead of saving it to the attach map
File*        gpTraceFile;
s256         gszTraceFilePath;
i8           giTraceFileSize;

extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

extern CmdLineArgs gxCmdLineArgs;
extern EnvFlags gxEnvFlags;

extern i8   gi_argc;
extern s256 gi_szaArgv[];

extern i8   giProgressPoint;

//#include <readline/readline.h>


#include "StreamDef.hpp"
#include "IndexDef.hpp"
#include "Key.hpp"
#include "Pay.hpp"
#include "Ent.hpp"
#include "RowSingle.hpp"
#include "EntWrk.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "TagConstants.h"
#include "RowBuffer.hpp"

#ifdef WITH_APPLE
  #define FN_OPT optnone
#elif defined(WITH_LINUX)
  #define FN_OPT optimize("O0")
#endif

static const i8 nIterations = 1000000000;

void __attribute__((FN_OPT)) vTest_rdtscp()
{
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

  #pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = Hardware::iReadTsc();
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "rdtscp", iTimeTaken, fTimePerCall);
}

void __attribute__((FN_OPT)) vTest_clock_gettime_realtime()
{
  struct timespec ts;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

  #pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = clock_gettime(CLOCK_REALTIME, &ts);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "clock_gettime(CLOCK_REALTIME)", iTimeTaken, fTimePerCall);
}

void __attribute__((FN_OPT)) vTest_clock_gettime_monotonic()
{
  struct timespec ts;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

  #pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = clock_gettime(CLOCK_MONOTONIC, &ts);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "clock_gettime(CLOCK_MONOTONIC)", iTimeTaken, fTimePerCall);
}

void __attribute__((FN_OPT)) vTest_clock_gettime_monotonic_raw()
{
  struct timespec ts;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

  #pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "clock_gettime(CLOCK_MONOTONIC_RAW)", iTimeTaken, fTimePerCall);
}

#ifdef WITH_APPLE
void __attribute__((FN_OPT)) vTest_clock_gettime_monotonic_raw_approx()
{
  struct timespec ts;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

#pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = clock_gettime(CLOCK_MONOTONIC_RAW_APPROX, &ts);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "clock_gettime(CLOCK_MONOTONIC_RAW_APPROX)", iTimeTaken, fTimePerCall);
}
#endif

#ifdef WITH_LINUX
void __attribute__((FN_OPT)) vTest_clock_gettime_realtime_coarse()
{
  struct timespec ts;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

#pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "clock_gettime(CLOCK_REALTIME_COARSE)", iTimeTaken, fTimePerCall);
}

void __attribute__((FN_OPT)) vTest_clock_gettime_monotonic_coarse()
{
  struct timespec ts;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

#pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "clock_gettime(CLOCK_MONOTONIC_COARSE)", iTimeTaken, fTimePerCall);
}
#endif

void __attribute__((FN_OPT)) vTest_gettimeofday()
{
  struct timeval tv;
  volatile i8 iVal;
  i8 iActualIters = 0;
  i8 iStartTime;
  i8 iEndTime;

  iStartTime = Hardware::iReadClock();

  #pragma unroll
  for (i8 i=0; i<nIterations; i++) {
    iVal = gettimeofday(&tv, nullptr);
    ++iActualIters;
  }

  iEndTime = Hardware::iReadClock();

  i8 iTimeTaken = iEndTime - iStartTime;
  d8 fTimePerCall = (d8) iTimeTaken / (d8) iActualIters;

  printf("%lld iterations of %s took %lld nanoseconds (%.1f ns / call)\n",
         nIterations, "gettimeofday()", iTimeTaken, fTimePerCall);
}

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{

  // Test code for measuring the calls to:
  //  (1) rdtscp
  //  (2) clock_gettime(CLOCK_REALTIME)
  //  (3) clock_gettime(CLOCK_MONOTONIC)
  //  (4) clock_gettime(CLOCK_MONOTONIC_RAW)
  //  (5) clock_gettime(CLOCK_MONOTONIC_RAW_APPROX)
  //  (6) gettimeofday()


  if (argc == 1 && strcmp(argv[1], "rdtscp") == 0) {
    vTest_rdtscp();

  } else if (argc == 1 && strcmp(argv[1], "clock_gettime_realtime") == 0) {
    vTest_clock_gettime_realtime();

  } else if (argc == 1 && strcmp(argv[1], "clock_gettime_monotonic") == 0) {
    vTest_clock_gettime_monotonic();

  } else if (argc == 1 && strcmp(argv[1], "clock_gettime_monotonic_raw") == 0) {
    vTest_clock_gettime_monotonic_raw();

  } else if (argc == 1 && strcmp(argv[1], "clock_gettime_monotonic_raw_approx") == 0) {
#ifdef WITH_APPLE
    vTest_clock_gettime_monotonic_raw_approx();
#endif

  } else if (argc == 1 && strcmp(argv[1], "clock_gettime_realtime_coarse") == 0) {
#ifdef WITH_LINUX
    vTest_clock_gettime_realtime_coarse();
#endif

  } else if (argc == 1 && strcmp(argv[1], "clock_gettime_monotonic_coarse") == 0) {
#ifdef WITH_LINUX
    vTest_clock_gettime_monotonic_coarse();
#endif

  } else if (argc == 1 && strcmp(argv[1], "gettimeofday") == 0) {
    vTest_gettimeofday();

  } else {
    printf("Running tests of all time APIs\n\n");

    vTest_rdtscp();
    vTest_rdtscp();
    vTest_rdtscp();
    vTest_clock_gettime_realtime();
    vTest_clock_gettime_realtime();
    vTest_clock_gettime_realtime();
    vTest_clock_gettime_monotonic();
    vTest_clock_gettime_monotonic();
    vTest_clock_gettime_monotonic();
    vTest_clock_gettime_monotonic_raw();
    vTest_clock_gettime_monotonic_raw();
    vTest_clock_gettime_monotonic_raw();

#ifdef WITH_APPLE
    vTest_clock_gettime_monotonic_raw_approx();
    vTest_clock_gettime_monotonic_raw_approx();
    vTest_clock_gettime_monotonic_raw_approx();
#endif

#ifdef WITH_LINUX
    vTest_clock_gettime_realtime_coarse();
    vTest_clock_gettime_realtime_coarse();
    vTest_clock_gettime_realtime_coarse();
    vTest_clock_gettime_monotonic_coarse();
    vTest_clock_gettime_monotonic_coarse();
    vTest_clock_gettime_monotonic_coarse();
#endif

    vTest_gettimeofday();
    vTest_gettimeofday();
    vTest_gettimeofday();
  }

  exit(0);

  // ********************************************************************************
  // NOTE: Till initial command line options are parsed and trace file is
  // initialised, we log to the trace file via piping out to stdout. We only do this
  // in developer's environment not in the installed environment (it is redirected
  // to /dev/null).
  // ********************************************************************************

  //char* s = readline ("Enter a line: ");

//  printf("sizeof(Table) %lld\n",(i8)sizeof(Table));
//  printf("sizeof(StreamDef) %lld\n",(i8)sizeof(StreamDef));
//  printf("sizeof(IndexDef) %lld\n",(i8)sizeof(IndexDef));
//  printf("sizeof(BucketMap) %lld\n",(i8)sizeof(BucketMap));
//  printf("sizeof(Object) %lld\n",(i8)sizeof(Object));
//  printf("sizeof(s32) %lld\n",(i8)sizeof(s32));
//  printf("sizeof(DataBuffer) %lld\n",(i8)sizeof(DataBuffer));
//  printf("sizeof(Lock) %lld\n",(i8)sizeof(Lock));

//  printf("sizeof(RowBuffer) %lld\n",(i8)sizeof(RowBuffer));
//  printf("C_SIZE_LARGE_OBJECT - sizeof(RowBuffer) %lld\n",C_SIZE_LARGE_OBJECT - (i8)sizeof(RowBuffer));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //signal(SIGPIPE, SIG_IGN);

  gxCmdLineArgs.vInit();

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
      // ********************************************************************************
      // NOTE: These are the default arguments used if the SE binary is executed without
      //       specifying any command line arguments. An end user is likely to do this.
      //       So let's choose options that are safe for them, not what is convenient
      //       for us developers.
      //       That's why we don't name it NODE1 - it might interfere with another
      //       cluster running on the machine. The --killPrev also ensures that there is
      //       only one instance of this running on the current machine.
      // ********************************************************************************
      u8 uRndClusterId = RndLib::uInteger(C_I8_MAX);
      sSeArgs.vSprint(
          "--start "
          "--prompt "
          "--metadata "
          "--killPrev "
          "--munged "
          "--name SINGLENODE "
          "--node SINGLENODE 127.0.0.1 5541 "
          "--heartbeat BROADCAST 8888 "
          "--cid %llu"
          ""_Stz,
          uRndClusterId);
    }
    vTrace("CmdLineArgs: %s\n"_Stz,sSeArgs.szStz());

    gi_argc = CmdLineArgs::nBurstArgs(gi_szaArgv, sSeArgs);
  }

  try {

    // parse the command line arguments for trace file options only
    if (! gxCmdLineArgs.bParseTraceFileArgs(gi_argc,gi_szaArgv))
      Process::vExitStartup();

    //Change to boolean and check like above
    Engine::vChangeDirectory(); //! what happens if this fails?

    if (! Engine::bTraceStart())
      Process::vExitStartup();
  }

  catch (Exception xException) {

    fflush(stderr); fflush(stdout);
    fprintf(stderr,"Main-level exception caught in func %s() file %s line %d\n",
            __func__,__FILE__,__LINE__);
    fflush(stderr); fflush(stdout);
    xException.vPrint();
    fflush(stderr); fflush(stdout);

    Process::vExitFailure();
  }

  // ********************************************************************************
  // NOTE: Now that we have parsed the initial command line options, we have opened
  // the trace file and vTrace() logs to the trace file. We should not be using
  // stdout or stderr for logging from this point onwards.
  // ********************************************************************************


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Initialise the random number generator
  Mersenne::init_genrand64(TickingDevice::iGetTick());

  TlsKeys::vInit();

  IntLib::vInitGlobals();

  FltLib::vInitGlobals();

  Engine::vEnvFlags();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  try {
    
    // force bamboo build
    
    Process::vEntry();
    
    // parse the rest of the command line arguments
    if (! gxCmdLineArgs.bParse(gi_argc,gi_szaArgv))
      Process::vExitStartup();
    
    // potentially run utilities
//    if (gxCmdLineArgs.m_xSysArgs.m_bUtility) {
//      Engine::vRunUtility(argc,(Stz**)argv);
//      Process::vExitSuccess();
//    }
    
    // temp for debugging hang in bamboo
//    gxCmdLineArgs.m_xSysArgs.m_bDeveloper = true;
//    gxCmdLineArgs.m_xSysArgs.m_oArgTags.vInsert(C_VB_API);
//    gxCmdLineArgs.m_xSysArgs.m_oArgTags.vInsert(C_VB_OP);
    // end temp
    
    switch (gxCmdLineArgs.m_xSysArgs.m_eUtility) {
        
      case UT_APIDEMO:
        NOT_IMPLEMENTED_YET;
        break;

      case UT_PFILE:
        PFileUtility::vRun(argc,(Stz**)argv);
        Process::vExitSuccess();
        break;

      case UT_SHM:
        ShmUtility::vRun(argc,(Stz**)argv);
        Process::vExitSuccess();
        break;
 
      case UT_RAPIDSSE:
//        vHeartInit();
//
//        if (gxCmdLineArgs.m_xSysArgs.m_iDelay > 0) {
//          printf("Delayed %lld seconds\n", gxCmdLineArgs.m_xSysArgs.m_iDelay);
//          sleep((unsigned int)gxCmdLineArgs.m_xSysArgs.m_iDelay);
//        }

        // run SE
        Engine::vMainEntry();
        break;
        
      default:
        NEVER_REACH;
    }
    
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

 ./rapidsse --node NODE2 127.0.0.1 5542 --name NODE2 --start --prompt --metadata --killprev --noMunge --developer --heartbeat BROADCAST 8888 --partitionsPerNode 4

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 # first node
 
 ./rapidsse --node NODE1 127.0.0.1 5541 --name NODE1 --start --prompt --metadata --killprev --noMunge --developer --heartbeat BROADCAST 8888 --partitionsPerNode 4

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 # second node

 ./rapidsse --node NODE2 127.0.0.1 5542 --name NODE2 --start --prompt --metadata --killprev --noMunge --developer --heartbeat BROADCAST 8888 --partitionsPerNode 4

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 # prompt
 
 ./rapidsse --name NODE1 --prompt --metadata [ --developer ]

 
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 # example of running a single seth test
 
 ./sethTest.sh ./bamboo/testlists/../../tests/tests1N4P/BackEnd/loopJoinHashcast.test
 
 
 
 
 
 
 
 
 
 
 Two Local Nodes on AWS 21st Dec 2021

 First Local Node

 ./rapidsse --node NODE1 127.0.0.1 5541 --name NODE1 --start --prompt --metadata --killprev --munged --developer --heartbeat BROADCAST 8888  --initial __loadAsAWS --threads 64 --netopt 1 --size 256GB --partitionsPerNode 32

 Second Local Node

 ./rapidsse --node NODE2 127.0.0.1 5542 --name NODE2 --start --prompt --metadata --killprev --munged --developer --heartbeat BROADCAST 8888  --initial __loadAsAWS --threads 64 --netopt 1 --size 256GB --partitionsPerNode 32
 
 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
