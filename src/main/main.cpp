/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "BASE.h"

#include "Version.h"

#include "CmdLineArgs.hpp"

#include "Heart.h"
#include "Engine.hpp"


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

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  //char* s = readline ("Enter a line: ");

//  printf("sizeof(Table) %lld\n",(i8)sizeof(Table));
//  printf("sizeof(StreamDef) %lld\n",(i8)sizeof(StreamDef));
//  printf("sizeof(IndexDef) %lld\n",(i8)sizeof(IndexDef));
//  printf("sizeof(BucketMap) %lld\n",(i8)sizeof(BucketMap));
//  printf("sizeof(Object) %lld\n",(i8)sizeof(Object));
//  printf("sizeof(s32) %lld\n",(i8)sizeof(s32));
//  printf("sizeof(DataBuffer) %lld\n",(i8)sizeof(DataBuffer));
//  printf("sizeof(Lock) %lld\n",(i8)sizeof(Lock));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TlsKeys::vInit();

  IntLib::vInitGlobals();

  FltLib::vInitGlobals();

  Engine::vEnvFlags();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
    vTrace("CmdLineArgs: %s\n"_Stz,sSeArgs.szStz());
    
    gi_argc = CmdLineArgs::nBurstArgs(gi_szaArgv, sSeArgs);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  try {
    
    // force bamboo build
    
    Process::vEntry();
    
    // parse the command line arguments
    if (! gxCmdLineArgs.bParse(gi_argc,gi_szaArgv))
      Process::vExitFailure();
    
    // potentially run utilities
    if (gxCmdLineArgs.m_xSysArgs.m_bUtility) {
      Engine::vRunUtility(argc,(Stz**)argv);
      Process::vExitSuccess();
    }
    
    // temp for debugging hang in bamboo
//    gxCmdLineArgs.m_xSysArgs.m_bDeveloper = true;
//    gxCmdLineArgs.m_xSysArgs.m_oArgTags.vInsert(C_VB_API);
//    gxCmdLineArgs.m_xSysArgs.m_oArgTags.vInsert(C_VB_OP);
    // end temp
    
    vHeartInit();

    if (gxCmdLineArgs.m_xSysArgs.m_iDelay > 0) {
      printf("Delayed %lld seconds\n", gxCmdLineArgs.m_xSysArgs.m_iDelay);
      sleep(gxCmdLineArgs.m_xSysArgs.m_iDelay);
    }

    // run SE
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

 
 x
 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
