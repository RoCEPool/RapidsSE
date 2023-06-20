/*============================================================================*/
/* CmdLineArgs.cpp - command line arguments                                   */
/*============================================================================*/

#include "BASE.h"

#include "CmdLineArgs.hpp"
#include "TagConstants.h"

extern CmdLineArgs       gxCmdLineArgs;
extern AttachMap*  gpAttachMap;

i8     gi_argc;
s256   gi_szaArgv[128];

extern void* C_ADR_REGION;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CmdLineArgs::CmdLineArgs(void)
: Object(CN_CMDLINEARGS)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CmdLineArgs::~CmdLineArgs(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* burst env string into command line arguments                               */
/*----------------------------------------------------------------------------*/
i8 CmdLineArgs::nBurstArgs(s256 saArgv[], Stz* szArgStr)
{
  _static_

  i8 i_Arg = 1;
  for (char* acSrc = szArgStr->azChar(); *acSrc; ) {
  
    if (' ' == *acSrc) {
      acSrc++;
      continue;
    }
    
    char* acDst = saArgv[i_Arg].azChar();
    
    while (*acSrc && ' ' != *acSrc)
      *acDst++ = *acSrc++;
    *acDst = '\0';
    
    if (*acSrc)
      acSrc++;
    
    i_Arg++;
  }
  
  return i_Arg;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vInit(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  // configuration
#ifdef WITH_APPLE
  m_xSysArgs.m_sCommandsDir  = "../commands"_Stz;
#else
  m_xSysArgs.m_sCommandsDir  = "../commands"_Stz;
#endif
  
  m_xSysArgs.m_iConnectCode  = C_UNKNOWN;
  for(i8 i=0; i<C_MAX_PTNSPERNODE; i++)
    m_xSysArgs.m_sDiskRootDir[i] = ""_Stz;
  m_xSysArgs.m_nDiskPaths    = 0;
  m_xSysArgs.m_sGridName     = "SEgrid"_Stz;
  m_xSysArgs.m_sLogFile      = ""_Stz;
  m_xSysArgs.m_sNodeName     = "node"_Stz;
  m_xSysArgs.m_sPageFilePath = "../../tmp/pfile.dat"_Stz;
  m_xSysArgs.m_nPtnsPerNode  = 4;
  m_xSysArgs.m_aRegionAddr   = (void*)C_ADR_REGION;
  m_xSysArgs.m_sRegionName   = ""_Stz;
  m_xSysArgs.m_sRegionPrefix = "/SE"_Stz;
  m_xSysArgs.m_iRegionSize   = (4 * 1_GB);
  m_xSysArgs.m_sReloadDir    = "";
  m_xSysArgs.m_bRLUDiary     = false;
  m_xSysArgs.m_bRLUTimer     = false;
  
  m_xSysArgs.m_nMetricsSleep = 1000;

#ifdef WITH_APPLE
  m_xSysArgs.m_sSenDir       = "../sen/"_Stz;
#else
  m_xSysArgs.m_sSenDir       = "../sen/"_Stz;
#endif
  
  // node configuration
  m_xSysArgs.m_nDefinedNodes   = 0;
  m_xSysArgs.m_nDefinedHeartbeats = 0;

  m_xSysArgs.m_sCommand = ""_Stz;
  m_xSysArgs.m_sInitial = ""_Stz;

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // commands
  m_xSysArgs.m_bAddNode     = false;
  m_xSysArgs.m_bAppendTrace = false;
  m_xSysArgs.m_bCommand     = false;
  m_xSysArgs.m_bHelp        = false;
  m_xSysArgs.m_bInitial     = false;
  m_xSysArgs.m_bPrompt      = false;
  m_xSysArgs.m_bSethScript  = false;
  m_xSysArgs.m_bStart       = false;
  m_xSysArgs.m_bStatus      = false;
  m_xSysArgs.m_bStop        = false;
  m_xSysArgs.m_bUnlink      = false;

  // options
  m_xSysArgs.m_iClusterID       = 0;
  m_xSysArgs.m_bCombined        = false;
  m_xSysArgs.m_bConfig          = true;
  m_xSysArgs.m_iCondCode        = 0;
  m_xSysArgs.m_bDebugIndex      = false;
  m_xSysArgs.m_bDebugMode       = false;
  m_xSysArgs.m_iDelay           = 0;
  m_xSysArgs.m_nDemonSleep      = 60;
  m_xSysArgs.m_bDerefMetrics    = false;
  m_xSysArgs.m_bDeveloper       = false;
  m_xSysArgs.m_bDisablePromptCmds = false;
  m_xSysArgs.m_bDistJob         = false;
  m_xSysArgs.m_bDontFree        = false;
  m_xSysArgs.m_nDummyThreads    = 0;
  m_xSysArgs.m_nDummySleep      = 10;
  m_xSysArgs.m_nDummyLoop       = 0;
  m_xSysArgs.m_nEntries         = 0;
  m_xSysArgs.m_bEvents          = false;
  m_xSysArgs.m_bFreeSpecific    = false;
  m_xSysArgs.m_bGridTest        = false;
  m_xSysArgs.m_bHugePages       = false;
  m_xSysArgs.m_nIndexEntries    = 0;
  m_xSysArgs.m_bJobMetrics      = false;
  m_xSysArgs.m_bJot             = false;
  m_xSysArgs.m_bKillPrev        = false;
  m_xSysArgs.m_bLockTracking    = false;
  m_xSysArgs.m_nLvlLevels       = C_DEF_LVL_LEVELS;
  m_xSysArgs.m_iLvlRatio        = C_DEF_LVL_RATIO;
  m_xSysArgs.m_bNoMunge         = false;
  m_xSysArgs.m_nMaxLoadErrors   = 0;
  m_xSysArgs.m_nMaxReadyLP      = 32;
  m_xSysArgs.m_bMetadata        = false;
  m_xSysArgs.m_bMunged          = false;
  m_xSysArgs.m_nOperands        = 0;
  m_xSysArgs.m_nOperandYield    = 10;
  m_xSysArgs.m_iOptimiserLvl    = 0;
  m_xSysArgs.m_nPeriodicSleep   = 10;
  m_xSysArgs.m_bProgress        = false;
  m_xSysArgs.m_iPromptDebug     = 0;
  m_xSysArgs.m_bReload          = false;
  m_xSysArgs.m_bRestart         = true;
  m_xSysArgs.m_bRLUDiary        = false;
  m_xSysArgs.m_bRLUTimer        = false;
  m_xSysArgs.m_bShmLock         = false;
  m_xSysArgs.m_iSocketLowMem    = C_UNKNOWN;
  m_xSysArgs.m_bSysMetrics      = false;
  m_xSysArgs.m_iTest            = C_UNKNOWN;
  m_xSysArgs.m_i_ThisNode       = C_UNKNOWN;
  m_xSysArgs.m_nThreads         = 12;
  m_xSysArgs.m_bTouch           = false;
  m_xSysArgs.m_bTracepoint      = false;
  m_xSysArgs.m_bTracing         = false;
  m_xSysArgs.m_eUtility         = UT_RAPIDSSE;
  m_xSysArgs.m_bVBlong          = false;
  m_xSysArgs.m_bVerbose         = false;
  m_xSysArgs.m_bWelcome         = true;

  _checkInitialised_(CN_CMDLINEARGS);
  
  m_xSysArgs.m_oArgTags.vInsert(C_SHOW_EXCEPTION,1);
  //m_xSysArgs.m_oArgTags.vInsert(C_NO_FREE,1);
  //m_xSysArgs.m_oArgTags.vInsert("<bamboo>"_Stz,1);
  
  vEnvironmentVariables();
}

/*----------------------------------------------------------------------------*/
/* check for environment variables                                            */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vEnvironmentVariables(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  s32  sVarName;
  s256 sRegionName;
  s256 sRegionSize;
  s256 sPartitions;
  s256 sFolder;
  
  sVarName = "SE_INSTANCE_NAME"_Stz;
  if (Env::bGet(sVarName,sRegionName))
    m_xSysArgs.m_sRegionName = sRegionName;

  sVarName = "SE_INSTANCE_SIZE"_Stz;
  if (Env::bGet(sVarName,sRegionSize))
    m_xSysArgs.m_iRegionSize = sRegionSize.iToI8();

  sVarName = "SE_PARTITIONS"_Stz;
  if (Env::bGet(sVarName,sPartitions))
    m_xSysArgs.m_nPtnsPerNode = sPartitions.iToI8();
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments                                               */
/*----------------------------------------------------------------------------*/
bool CmdLineArgs::bParse(i8 argc, s256 argv[])
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);

  if (argv[1].bSame("apiDemo"_Stz)) {
    m_xSysArgs.m_eUtility = UT_APIDEMO;
    return true;
  }

  if (argv[1].bSame("pfile"_Stz)) {
    m_xSysArgs.m_eUtility = UT_PFILE;
    return true;
  }

  if (argv[1].bSame("shm"_Stz)) {
    m_xSysArgs.m_eUtility = UT_SHM;
    return true;
  }

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  
  m_xSysArgs.m_oArgTags.vInsert(C_SHOW_RECURRING);

  i8 i_BootNode = 0;
  i8 i_HeartbeatNode = 0;
  i8 i_DiskPath = 0;

  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {
    
    Stz* szArg = argv[i_Arg];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--addNode"_Stz)) {
      m_xSysArgs.m_bAddNode = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--address"_Stz)) {
      m_xSysArgs.m_aRegionAddr = (void*)argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--appendTrace"_Stz)) {
      m_xSysArgs.m_bAppendTrace = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Cluster ID, this is to group nodes into a cluster.
    // Should only accept heartbeat from the same cluster.
    if (szArg->bSame("--cid"_Stz)) {
      if ( ! StrLib::bIsInteger(argv[++i_Arg].azChar()) ) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid cid argument value: %s\n"_Stz,(Stz*)argv[i_Arg]);
        return false;
      }
      m_xSysArgs.m_iClusterID = argv[i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--command"_Stz)) {
      m_xSysArgs.m_sCommand = (Stz*)argv[++i_Arg];
      vTrace("command %s\n"_Stz, m_xSysArgs.m_sCommand.azChar());
      m_xSysArgs.m_bCommand = true;
      m_xSysArgs.m_bPrompt  = true;
      m_xSysArgs.m_bWelcome = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--commands"_Stz)) {
      if(argv[++i_Arg].bContains("--"_Stz))
      {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid commands argument value: %s\n"_Stz, (Stz*)argv[i_Arg]);
        vTrace("Usage: --commands <path> (path to commands directory)\n"_Stz);
        return false;
      }
      m_xSysArgs.m_sCommandsDir = (Stz*)argv[i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--condCode"_Stz)) {
      m_xSysArgs.m_iCondCode = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--config"_Stz)) {
      m_xSysArgs.m_bConfig = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--connectCode"_Stz)) {
      m_xSysArgs.m_iConnectCode = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--debugIndex"_Stz)) {
      m_xSysArgs.m_bDebugIndex = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--debugMode"_Stz)) {
      m_xSysArgs.m_bDebugMode = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--delay"_Stz)) {
      m_xSysArgs.m_iDelay = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--demonSleep"_Stz)) {
      m_xSysArgs.m_nDemonSleep = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--derefMetrics"_Stz)) {
      m_xSysArgs.m_bDerefMetrics = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--developer"_Stz)) {
      m_xSysArgs.m_bDeveloper = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--disk"_Stz)) {
      /* Note: This is the path to a disk root directory. Under this directory,
       * we search for or add the dir/saveFile that is used during unload/reload.
       */

      m_xSysArgs.m_sDiskRootDir[i_DiskPath] = (Stz*)argv[++i_Arg];

      if( ! Dir::bExists(m_xSysArgs.m_sDiskRootDir[i_DiskPath]))
      {
        vTrace("Disk directory %s does not exist!!\n"_Stz,
               m_xSysArgs.m_sDiskRootDir[i_DiskPath].szStz());
        return false;
      }
      if( ! StrLib::bEnds(m_xSysArgs.m_sDiskRootDir[i_DiskPath].azChar(), '/'))
      {
        m_xSysArgs.m_sDiskRootDir[i_DiskPath].vAppend('/');
      }
      i_DiskPath++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--distJob"_Stz)) {
      m_xSysArgs.m_bDistJob = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dontFree"_Stz)) {
      m_xSysArgs.m_bDontFree = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dummyLoop"_Stz)) {
      m_xSysArgs.m_nDummyLoop = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dummySleep"_Stz)) {
      m_xSysArgs.m_nDummySleep = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dummyThreads"_Stz)) {
      m_xSysArgs.m_nDummyThreads = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (szArg->bSame("--enableCommands"_Stz)) {
      m_xSysArgs.m_bDisablePromptCmds = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (szArg->bSame("--entries"_Stz)) {
      m_xSysArgs.m_nEntries = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (szArg->bSame("--events"_Stz)) {
      m_xSysArgs.m_bEvents = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--freeSpecific"_Stz)) {
      m_xSysArgs.m_bFreeSpecific = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--gridName"_Stz)) {
      m_xSysArgs.m_sGridName = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--gridTest"_Stz)) {
      m_xSysArgs.m_bGridTest = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Heartbeat command is here, using the same syntax as --node command, we can finalize
    // the command syntax later, but for now to have the code in place
    if (szArg->bSame("--heartbeat"_Stz)) {
      // TODO Arjun: The below needs to check for a valid hostname as well. Will be done at a later date.
//      if(0 == StrLib::bIsIPAddr(argv[++i_Arg].azChar()) && 0 == argv[i_Arg].bSame("BROADCAST"_Stz))
//      {
//        vPrintErrorMarker(argv, i_Arg);
//        vTrace("Invalid heartbeat argument value: %s\n"_Stz, (Stz*)argv[i_Arg]);
//        vTrace("Usage: --heartbeat BROADCAST|<ip-addr> <port>\n"_Stz);
//        return false;
//      }
      m_xSysArgs.m_xHeartbeatDefn[i_HeartbeatNode].m_sIPAddress = (Stz*)argv[++i_Arg];

      if ( ! StrLib::bIsInteger(argv[++i_Arg].azChar()) ) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid heartbeat argument value: %s\n"_Stz, (Stz*)argv[i_Arg]);
        vTrace("Usage: --heartbeat BROADCAST|<ip-addr> <port>\n"_Stz);
        return false;
      }
      m_xSysArgs.m_xHeartbeatDefn[i_HeartbeatNode].m_iPortNo    = argv[i_Arg].iToI8();
      i_HeartbeatNode++;

      if (i_HeartbeatNode > C_MAX_NODES) {
        vTrace("Invalid heartbeat parameter\n"_Stz);
        vTrace("Maximum number of nodes allowed in the cluster is %lld\n"_Stz,C_MAX_NODES);
        return false;
      }
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (
        argv[i_Arg].bSame("--help"_Stz)
        || argv[i_Arg].bSame("-help"_Stz)
        || argv[i_Arg].bSame("help"_Stz)
        || argv[i_Arg].bSame("--?"_Stz)
        || argv[i_Arg].bSame("-?"_Stz)
        || argv[i_Arg].bSame("?"_Stz)
        )  {
      vPrintHelp();
      Process::vExitSuccess();
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--hugepages"_Stz)) {
      m_xSysArgs.m_bHugePages = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--initial"_Stz)) {
      m_xSysArgs.m_bInitial = true;
      m_xSysArgs.m_sInitial = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--jobMetrics"_Stz)) {
      m_xSysArgs.m_bJobMetrics = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--jot"_Stz)) {
      vJotDebug(3);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--killPrev"_Stz)) {
      m_xSysArgs.m_bKillPrev = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--lockTracking"_Stz)) {
      m_xSysArgs.m_bLockTracking = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--logFile"_Stz)) {
      if (m_xSysArgs.m_bStart) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_sLogFile = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--lvlLevels"_Stz)) {
      m_xSysArgs.m_nLvlLevels = argv[++i_Arg].iToI8();
      if (m_xSysArgs.m_nLvlLevels < C_MIN_LVL_LEVELS) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid lvlLevels argument value: %lld\n"_Stz, m_xSysArgs.m_nLvlLevels);
        vTrace("Minimum number of levels allowed is %lld\n"_Stz,C_MIN_LVL_LEVELS);
        return false;
      }
      if (m_xSysArgs.m_nLvlLevels > C_MAX_LVL_LEVELS) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid lvlLevels argument value: %lld\n"_Stz, m_xSysArgs.m_nLvlLevels);
        vTrace("Maximum number of levels allowed is %lld\n"_Stz,C_MAX_LVL_LEVELS);
        return false;
      }
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--lvlRatio"_Stz)) {
      m_xSysArgs.m_iLvlRatio = argv[++i_Arg].iToI8();
      if (m_xSysArgs.m_iLvlRatio < C_MIN_LVL_RATIO) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid lvlRatio argument value: %lld\n"_Stz, m_xSysArgs.m_iLvlRatio);
        vTrace("Minimum ratio of levels allowed is %lld\n"_Stz,C_MIN_LVL_RATIO);
        return false;
      }
      if (m_xSysArgs.m_iLvlRatio > C_MAX_LVL_RATIO) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid lvlRatio argument value: %lld\n"_Stz, m_xSysArgs.m_iLvlRatio);
        vTrace("Maximum ratio of levels allowed is %lld\n"_Stz,C_MAX_LVL_RATIO);
        return false;
      }
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--maxLoadErrors"_Stz)) {
      m_xSysArgs.m_nMaxLoadErrors = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--maxReadyLP"_Stz)) {
      m_xSysArgs.m_nMaxReadyLP = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--metadata"_Stz)) {
      m_xSysArgs.m_bMetadata = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--metricsSleep"_Stz)) {
      m_xSysArgs.m_nMetricsSleep = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--munged"_Stz)) {
      m_xSysArgs.m_bMunged = true;
#ifdef WITH_MUNGED_SEN
      continue;
#else
      vTrace("--munged specified but SE not compiled WITH_MUNGED_SEN\n"_Stz);
      return false;
#endif
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--name"_Stz) || argv[i_Arg].bSame("--nodeName"_Stz)) {
      m_xSysArgs.m_sNodeName = (Stz*)argv[++i_Arg];
      continue;
    }

   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--node"_Stz)) {
      m_xSysArgs.m_xNodeDefn[i_BootNode].m_sNodeName  = (Stz*)argv[++i_Arg];
        //TODO Arjun: Below needs to check for a valid hostname as well. Will be done at a later date.
//      if(0 == StrLib::bIsIPAddr(argv[++i_Arg].azChar()))
//      {
//        vPrintErrorMarker(argv, i_Arg);
//        vTrace("Invalid node argument IP Address: %s\n"_Stz, (Stz*)argv[i_Arg]);
//        vTrace("Usage: --node <name> <ip-addr> <port>\n"_Stz);
//        return false;
//      }
      m_xSysArgs.m_xNodeDefn[i_BootNode].m_sIPAddress = (Stz*)argv[++i_Arg];

      if ( ! StrLib::bIsInteger(argv[++i_Arg].azChar()) ) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid node argument Port number: %s\n"_Stz,(Stz*)argv[i_Arg]);
        vTrace("Usage: --node <name> <ip-addr> <port>\n"_Stz);
        return false;
      }
      m_xSysArgs.m_xNodeDefn[i_BootNode].m_iPortNo    = argv[i_Arg].iToI8();

      i_BootNode++;
      //TODO: Arjun check if we are supposed to use i_BootNode here.
      if (m_xSysArgs.m_nDefinedNodes > C_MAX_NODES) {
        vTrace("Invalid node argument value : %lld\n"_Stz, m_xSysArgs.m_nDefinedNodes);
        vTrace("Maximum number of nodes definable nodes is %lld\n"_Stz,
              C_MAX_NODES);
        return false;
      }
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--NoMunge"_Stz)) {
      m_xSysArgs.m_bNoMunge = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--noprogress"_Stz)) {
      m_xSysArgs.m_bProgress = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--notouch"_Stz)) {
      m_xSysArgs.m_bTouch = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--nowelcome"_Stz)) {
      m_xSysArgs.m_bWelcome = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--operands"_Stz)) {
      m_xSysArgs.m_nOperands = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--operandYield"_Stz)) {
      m_xSysArgs.m_nOperandYield = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--optimiserLvl"_Stz)) {
      m_xSysArgs.m_iOptimiserLvl = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--partitionsPerNode"_Stz)) {

      if ( ! StrLib::bIsInteger(argv[++i_Arg].azChar()) ) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid partitionsPerNode argument value: %s\n"_Stz,(Stz*)argv[i_Arg]);
        vTrace("Usage: --partitionsPerNode <no-of-ptns>\n"_Stz);
        return false;
      }

      m_xSysArgs.m_nPtnsPerNode = argv[i_Arg].iToI8();
      if (m_xSysArgs.m_nPtnsPerNode <= 0 ||
          m_xSysArgs.m_nPtnsPerNode > C_MAX_PTNSPERNODE ) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("partitionsPerNode %lld is outside the valid range (1 to %lld)\n"_Stz,
              m_xSysArgs.m_nPtnsPerNode, C_MAX_PTNSPERNODE);
        return false;
      }
      if (m_xSysArgs.m_nPtnsPerNode > C_MAX_PTNSPERNODE) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Maximum number of partitions allowed per node is %lld\n"_Stz,
          C_MAX_PTNSPERNODE);
        return false;
      }

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--periodicSleep"_Stz)) {
      m_xSysArgs.m_nPeriodicSleep = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--pFile"_Stz)) {
      m_xSysArgs.m_sPageFilePath = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--progress"_Stz)) {
      m_xSysArgs.m_bProgress = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--prompt"_Stz)) {

      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bPrompt = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--promptDebug"_Stz)) {
      m_xSysArgs.m_iPromptDebug = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--region"_Stz)) {
      m_xSysArgs.m_sRegionName = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--regionPrefix"_Stz)) {
      m_xSysArgs.m_sRegionPrefix = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--reload"_Stz)) {
      /* Note:  We take the name of the reload directory path, relative to the
       * disk paths given in --disk argument. This assumes that each disk
       * path given has this directory, which contains the files for reload.
       */
      m_xSysArgs.m_bReload = true;
      m_xSysArgs.m_sReloadDir = (Stz*)argv[++i_Arg];

      if( ! StrLib::bEnds(m_xSysArgs.m_sReloadDir.azChar(), '/'))
      {
        m_xSysArgs.m_sReloadDir.vAppend('/');
      }
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //if (argv[iArg]->bSame("--restart"_Stz)) {
    //  m_xSysArgs.m_bRestart = true;
    //  continue;
    //}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--RLUdiary"_Stz)) {
      m_xSysArgs.m_bRLUDiary = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--RLUTimer"_Stz)) {
      m_xSysArgs.m_bRLUTimer = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--script"_Stz)) {
      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      m_xSysArgs.m_bSethScript = true;
      m_xSysArgs.m_bWelcome = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--seprompt"_Stz)) {
      m_xSysArgs.m_bPrompt = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--sen"_Stz)) {
      m_xSysArgs.m_sSenDir = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lock shared memory pages in memory
    if (szArg->bSame("--shmlock"_Stz)) {
      m_xSysArgs.m_bShmLock = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--size"_Stz)) {

      bool bResult = MagLib::bToBytes((Stz*)argv[++i_Arg], m_xSysArgs.m_iRegionSize);
      if (!bResult) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid size argument value: %s\n"_Stz,argv[i_Arg].szStz());
        vTrace("Usage: --size <no>kb|mb|gb|tb|pb"_Stz);
        return false;
      }

      i8 iTotalMemory = MemLib::iGetTotalMemory();
      if (! m_xSysArgs.m_bDeveloper) {

        bool bAbove95Pct = m_xSysArgs.m_iRegionSize > DIV(C_MAX_SHM_PERCENT * iTotalMemory, 100);
        bool bLt10GBLeft = iTotalMemory - m_xSysArgs.m_iRegionSize < C_MAX_LEFTOVER_MEM;

        if (bAbove95Pct && bLt10GBLeft) {
          vPrintErrorMarker(argv, i_Arg);
          vTrace("Invalid size argument value: %s\n"_Stz, argv[i_Arg].szStz());
          s32 sMag;
          MagLib::vByteMag(iTotalMemory, sMag);
          vTrace("Cannot set size > %lld%% of memory on this node (node has only %s of memory)\n"_Stz,
                 C_MAX_SHM_PERCENT,
                 sMag.azChar());
          return false;
        }
      }

      s32 sMinSize;
      MagLib::vByteMag(C_MIN_SHM_REGION, sMinSize);

      s32 sSize;
      MagLib::vByteMag(m_xSysArgs.m_iRegionSize, sSize);

      if (m_xSysArgs.m_iRegionSize < C_MIN_SHM_REGION) {
        vTrace("Insufficient region size: %s. At least %s is required\n"_Stz,
              sSize.szStz(),
              sMinSize.szStz());
        return false;
      }

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--socketLowMem"_Stz)) {
      m_xSysArgs.m_iSocketLowMem = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--start"_Stz)) {

      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bStart = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--status"_Stz)) {
      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStart) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bPrompt) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bStatus = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--stop"_Stz)) {

      if (m_xSysArgs.m_bStart) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bPrompt) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bStop = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--sysMetrics"_Stz)) {
      m_xSysArgs.m_bSysMetrics = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--tag"_Stz)) {

      // --tag name = "value"
      // --tag name

      i8 i_SysArg = i_Arg + 1;

      if (i_SysArg >= argc) {
        vTrace("Incorrect argument: --tag with nothing after\n"_Stz);
        vTrace("Usage: --tag name or --tag name = value\n"_Stz);
        return false;
      }

      s32 sTagName;

      sTagName.vSprint("<%s>"_Stz, (Stz*)argv[++i_Arg]);

      i_SysArg++;

      if (i_SysArg >= argc) {
        m_xSysArgs.m_oSysTags.vInsert(sTagName,1);
        continue;
      }

      Stz* szTagEqual = argv[i_SysArg];
      if ('=' != * szTagEqual->azChar()) {
        m_xSysArgs.m_oSysTags.vInsert(sTagName,1);
        continue;
      }

      // consume name
      i_Arg++;

      // consume =
      i_Arg++;

      Stz* szTagValue = argv[i_Arg];

      if (szTagValue->bIsInteger())
        m_xSysArgs.m_oSysTags.vInsert(sTagName,szTagValue->iToI8());
      else
        m_xSysArgs.m_oSysTags.vInsert(sTagName,szTagValue);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--test"_Stz)) {
      m_xSysArgs.m_iTest = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--threads"_Stz)) {
      if ( ! StrLib::bIsInteger(argv[++i_Arg].azChar()) ) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid threads argument value: %s\n"_Stz, (Stz*)argv[i_Arg]);
        return false;
      }
      m_xSysArgs.m_nThreads = argv[i_Arg].iToI8();
      if ( m_xSysArgs.m_nThreads <= 0 ||
           m_xSysArgs.m_nThreads > C_MAX_THREADS) {
        vTrace("Number of thread is %lld is outside the valid range (1 to %lld)\n"_Stz,
               m_xSysArgs.m_nThreads,
               C_MAX_THREADS);
        return false;
      }

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--touch"_Stz)) {
      m_xSysArgs.m_bTouch = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--tracepoint"_Stz)) {
      m_xSysArgs.m_bTracepoint = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--tracing"_Stz)) {
      m_xSysArgs.m_bTracing = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--unlink"_Stz)) {
      m_xSysArgs.m_bUnlink = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--VBlong"_Stz)) {
      m_xSysArgs.m_bVBlong = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--verbose"_Stz)) {
      m_xSysArgs.m_bVerbose = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--welcome"_Stz)) {
      m_xSysArgs.m_bWelcome = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--combined"_Stz)) {
      m_xSysArgs.m_bCombined = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if ('<' == * szArg->azChar()) {

      i8 i_TagArg = i_Arg + 1;

      if (i_TagArg >= argc) {
        m_xSysArgs.m_oArgTags.vInsert(szArg);
        continue;
      }

      Stz* szTagEqual = argv[i_TagArg];
      if ('=' != * szTagEqual->azChar()) {
        m_xSysArgs.m_oArgTags.vInsert(szArg);
        continue;
      }

      // consume <tagNm>
      i_Arg++;

      // consume =
      i_Arg++;

      Stz* szTagValue = argv[i_Arg];

      if (szTagValue->bIsInteger())
        m_xSysArgs.m_oArgTags.vInsert(szArg,szTagValue->iToI8());
      else
        m_xSysArgs.m_oArgTags.vInsert(szArg,szTagValue);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    vPrintErrorMarker(argv, i_Arg);
    vTrace("Unrecognized command line argument: %s\n"_Stz,argv[i_Arg].szStz());
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Save the number of disk paths given (used for unload/reload).
  m_xSysArgs.m_nDiskPaths = i_DiskPath;
  if(m_xSysArgs.m_bReload)
  {
    if(m_xSysArgs.m_nDiskPaths == 0)
    {
      vTrace("Incorrect argument: --reload was given without the --disk option\n"_Stz);
      vTrace("Please provide disk root paths\n"_Stz);
      return false;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if not in prompt mode, log file path should not be given
  if ( ! m_xSysArgs.m_sLogFile.bIsEmpty() ) {

    if( ! m_xSysArgs.m_bPrompt && ! m_xSysArgs.m_bSethScript)
    {
      vTrace("Incorrect argument: --logFile, can only specify log file path with --prompt or --script\n"_Stz);
      return false;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if --heartbeat was defined just set the m_xSysArgs.m_nDefinedHeartbeats
  // for Broadcast mode, this is 1
  if (i_HeartbeatNode) {

    // set no. of HB nodes
    m_xSysArgs.m_nDefinedHeartbeats = i_HeartbeatNode;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if --node was defined just set the m_xSysArgs.m_nBootNodes
  if (i_BootNode) {

    // set no. of nodes
    m_xSysArgs.m_nDefinedNodes = i_BootNode;

    // some basic cluster validations

    // there should be at aleast 2 nodes ina cluster
    // if (m_xSysArgs.m_nBootNodes < 2) {
    //   vTrace("Minimum two nodes should be there in a cluster\n"_Stz);
    //   return false;
    // }

    // the current node should have been specfied in the --node
    bool bFound = false;
    for (i8 i_Test = 0; i_Test < m_xSysArgs.m_nDefinedNodes; i_Test++) {
      if (m_xSysArgs.m_xNodeDefn[i_Test].m_sNodeName.bSame(m_xSysArgs.m_sNodeName)) {
        bFound = true;
        m_xSysArgs.m_i_ThisNode = i_Test;
        break;
      }
    }

// RJH: moved this into Engine::vCheckNodes() 17 Mar 2020
//    if ( ! bFound ) {
//      vTrace("Add argument --name with name of this node from the --node definitions\n"_Stz);
//      return false;
//    }

  } else {
    // add a single node
    // m_xSysArgs.m_nBootNodes = 1;
    // m_xSysArgs.m_xNodeDefn[0].m_sNodeName = m_xSysArgs.m_sNodeName;
    // m_xSysArgs.m_xNodeDefn[0].m_sIPAddress = ""_Stz;
    // m_xSysArgs.m_xNodeDefn[0].m_iPortNo =  -1;
//    if ( m_xSysArgs.m_bStart ) {
//      vTrace("Add argument '--node name ip-address port' for this node %s definition\n"_Stz, m_xSysArgs.m_sNodeName.azChar());
//      return false;
//    }
  }

  // if no regionName then form region name using regionPrefix and node name
  if (m_xSysArgs.m_sRegionName.bSame(""_Stz)) {
    m_xSysArgs.m_sRegionName.vCat(m_xSysArgs.m_sRegionPrefix,"_"_Stz,m_xSysArgs.m_sNodeName.azChar(),nullptr);
  }
  else {
    if (m_xSysArgs.m_bStart && m_xSysArgs.m_nDefinedNodes > 1) {
      vTrace("--region not allowed with --start in a cluster\n"_Stz);
      return false;
    }
  }

  if (m_xSysArgs.m_oArgTags.bHasName(C_NOSHOW_RECURRING))
    m_xSysArgs.m_oArgTags.vDelete(C_SHOW_RECURRING);

  //m_xSysArgs.m_oArgTags.vDumpSet(true);

  if (m_xSysArgs.m_oArgTags.bHasName("<bamboo>"_Stz)) {

    m_xSysArgs.m_oArgTags.vInsert("<show_command>"_Stz,1);
    //m_xSysArgs.m_oArgTags.vInsert("<show_op_report>"_Stz,1);

//    if (m_xSysArgs.m_iRegionSize < C_8GB)
//      m_xSysArgs.m_iRegionSize = C_8GB;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments to figure out trace file settings             */
/*----------------------------------------------------------------------------*/
bool CmdLineArgs::bParseTraceFileArgs(i8 argc, s256 argv[])
{
  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {

    Stz *szArg = argv[i_Arg];

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--addNode"_Stz)) {
      m_xSysArgs.m_bAddNode = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--command"_Stz)) {
      m_xSysArgs.m_sCommand = (Stz*)argv[++i_Arg];
      vTrace("command %s\n"_Stz, m_xSysArgs.m_sCommand.azChar());
      m_xSysArgs.m_bCommand = true;
      m_xSysArgs.m_bPrompt  = true;
      m_xSysArgs.m_bWelcome = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--commands"_Stz)) {
      if(argv[++i_Arg].bContains("--"_Stz))
      {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Invalid commands argument value: %s\n"_Stz, (Stz*)argv[i_Arg]);
        vTrace("Usage: --commands <path> (path to commands directory)\n"_Stz);
        return false;
      }
      m_xSysArgs.m_sCommandsDir = (Stz*)argv[i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--developer"_Stz)) {
      m_xSysArgs.m_bDeveloper = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--logFile"_Stz)) {
      if (m_xSysArgs.m_bStart) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_sLogFile = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--name"_Stz) || argv[i_Arg].bSame("--nodeName"_Stz)) {
      m_xSysArgs.m_sNodeName = (Stz*)argv[++i_Arg];
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--prompt"_Stz)) {

      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bPrompt = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--start"_Stz)) {

      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bStart = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--seprompt"_Stz)) {
      m_xSysArgs.m_bPrompt = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--status"_Stz)) {
      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStart) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bPrompt) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bStatus = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--stop"_Stz)) {

      if (m_xSysArgs.m_bStart) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bPrompt) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }

      m_xSysArgs.m_bStop = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--script"_Stz)) {
      if (m_xSysArgs.m_bStop) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_xSysArgs.m_bStatus) {
        vPrintErrorMarker(argv, i_Arg);
        vTrace("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      m_xSysArgs.m_bSethScript = true;
      m_xSysArgs.m_bWelcome = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--unlink"_Stz)) {
      m_xSysArgs.m_bUnlink = true;
      continue;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if not in prompt mode, log file path should not be given
  if ( ! m_xSysArgs.m_sLogFile.bIsEmpty() ) {

    if( ! m_xSysArgs.m_bPrompt && ! m_xSysArgs.m_bSethScript)
    {
      vTrace("Incorrect argument: --logFile, can only specify log file path with --prompt or --script\n"_Stz);
      return false;
    }
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vPrint(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  vTrace("\nArgs:"_Stz);
  
  for (i8 i_Arg = 1; i_Arg < gi_argc; i_Arg++) {
    if ('-' == *gi_szaArgv[i_Arg].azChar())
      vTrace("\n  %s"_Stz,gi_szaArgv[i_Arg].szStz());
    else
      vTrace(" %s"_Stz,gi_szaArgv[i_Arg].szStz());
  }
  vTrace("\n\n"_Stz);
  
  vTrace("  ./SE "_Stz);
  for (i8 i_Arg = 1; i_Arg < gi_argc; i_Arg++) {
    vTrace("%s "_Stz,gi_szaArgv[i_Arg].szStz());
  }
  
  //vTrace("--region %s"_Stz,m_xSysArgs.m_sRegionName.szStz());
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vPrintErrorMarker(s256* argv, i8 i_Arg)
{
  s256 sErrorMarker;
  i8 iCount = 0;
  for(i8 i = 0; i < i_Arg; i++)
  {
    iCount += argv[i].nLength() + 1;
  }
  for(i8 i = 0; i < iCount; i++)
  {
    sErrorMarker.vAppend(' ');
  }
  sErrorMarker.vAppend('^');
  for(i8 i = 0; i < argv[i_Arg].nLength() - 1; i++)
  {
    sErrorMarker.vAppend('~');
  }
  vTrace("%s\n"_Stz, sErrorMarker.szStz());
}
/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vPrintStartupInfo(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  // print startup information
  //if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
  //  return;
  
  
  if (gxCmdLineArgs.m_xSysArgs.m_bVerbose) {
    vTrace("\n"_Stz,CN_ENUMS);
    vTrace("  No of Classes %lld\n"_Stz,CN_ENUMS);
#ifdef WITH_MUNGED_SEN
    vTrace("  Compiled with MUNGE\n\n"_Stz);
#else
    vTrace("  Not compiled with MUNGE\n"_Stz);
#endif
  }

  if (! gxCmdLineArgs.bUserPrompt()) {
    vTrace("\n"_Stz);
    vTrace("Args:\n"_Stz);

#ifdef WITH_DEBUG_VERSION
    if (m_xSysArgs.m_bAppendTrace) vTrace("  --appendTrace\n"_Stz);
    if (m_xSysArgs.m_bCommand)     vTrace("  --command\n"_Stz);
    if (m_xSysArgs.m_bHelp)        vTrace("  --help\n"_Stz);
    if (m_xSysArgs.m_bProgress)    vTrace("  --progress\n"_Stz);
    if (m_xSysArgs.m_bPrompt)      vTrace("  --seprompt\n"_Stz);
    if (m_xSysArgs.m_bSethScript)  vTrace("  --script\n"_Stz);
    if (m_xSysArgs.m_bStart)       vTrace("  --start\n"_Stz);
    if (m_xSysArgs.m_bStatus)      vTrace("  --status\n"_Stz);
    if (m_xSysArgs.m_bStop)        vTrace("  --stop\n"_Stz);
    if (m_xSysArgs.m_bVerbose)     vTrace("  --verbose\n"_Stz);
#endif

    vTrace("  --name %s\n"_Stz, m_xSysArgs.m_sNodeName.azChar());
    vTrace("  --region %s\n"_Stz, m_xSysArgs.m_sRegionName.azChar());

#ifdef WITH_DEBUG_VERSION
    vTrace("  --address %p\n"_Stz,m_xSysArgs.m_aRegionAddr);
#endif

    s32 sSizeMag;
    MagLib::vByteMag(m_xSysArgs.m_iRegionSize, sSizeMag);
    vTrace("  --size %s\n"_Stz, sSizeMag.azChar());

    for (i8 i_Index = 0; i_Index < m_xSysArgs.m_nDefinedNodes; i_Index++) {
      vTrace("  --node %s %s %lld\n"_Stz,
             m_xSysArgs.m_xNodeDefn[i_Index].m_sNodeName.azChar(),
             m_xSysArgs.m_xNodeDefn[i_Index].m_sIPAddress.azChar(),
             m_xSysArgs.m_xNodeDefn[i_Index].m_iPortNo);
    }

    for (i8 i_Index = 0; i_Index < m_xSysArgs.m_nDefinedHeartbeats; i_Index++) {
      vTrace("  --heartbeat %s %lld\n"_Stz,
             m_xSysArgs.m_xHeartbeatDefn[i_Index].m_sIPAddress.azChar(),
             m_xSysArgs.m_xHeartbeatDefn[i_Index].m_iPortNo);
    }

    vTrace("  --cid %lld\n"_Stz, m_xSysArgs.m_iClusterID);
    vTrace("  --partitionsPerNode %lld\n"_Stz, m_xSysArgs.m_nPtnsPerNode);
    vTrace("  --threads %lld\n"_Stz, m_xSysArgs.m_nThreads);

    if (m_xSysArgs.m_nDummyThreads) {
      vTrace("  --dummyThreads %lld\n"_Stz, m_xSysArgs.m_nDummyThreads);
      vTrace("  --dummySleep %lld\n"_Stz, m_xSysArgs.m_nDummySleep);
      vTrace("  --dummyLoop %lld\n"_Stz, m_xSysArgs.m_nDummyLoop);
      vTrace("  --periodicSleep %lld\n"_Stz, m_xSysArgs.m_nPeriodicSleep);
    }

    if (m_xSysArgs.m_bMetadata) vTrace("  --metadata\n"_Stz);
    if (m_xSysArgs.m_bKillPrev) vTrace("  --killPrev\n"_Stz);
    if (m_xSysArgs.m_bMunged) vTrace("  --munged\n"_Stz);
    if (m_xSysArgs.m_bMetadata) vTrace("  --metadata\n"_Stz);

#ifdef WITH_DEBUG_VERSION
    vTrace("  --commands %s\n"_Stz,m_xSysArgs.m_sCommandsDir.azChar());
    vTrace("  --sen %s\n"_Stz,m_xSysArgs.m_sSenDir.azChar());
    if (m_xSysArgs.m_bMunged) vTrace("  --munged\n"_Stz);
#endif

    if (m_xSysArgs.m_bDeveloper) vTrace("  --developer\n"_Stz);

    if (m_xSysArgs.m_bInitial) vTrace("  --initial %s\n"_Stz,m_xSysArgs.m_sInitial.szStz());

    BASE::vTraceWith();
  }
}

/*----------------------------------------------------------------------------*/
/* print help                                                                 */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vPrintHelp(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  vTrace("\n"_Stz);
  
  vTrace("Help:\n"_Stz);
  vTrace("--command <cmd>   (run given command)\n"_Stz);
  vTrace("--commands <path> (path to commands directory)\n"_Stz);
  vTrace("--heartbeat BROADCAST|<ip-addr> <port>\n"_Stz);
  vTrace("--help            (print this list)\n"_Stz);
  vTrace("--list            (print list of available systems)\n"_Stz);
  vTrace("--start           (start system)\n"_Stz);
  vTrace("--status          (print status of system)\n"_Stz);
  vTrace("--stop            (stop system)\n"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("--partitionsPerNode <no-of-ptns> (number of partitions per node)\n"_Stz);
  vTrace("--node <name> <ip-addr> <port> (node information in the cluster)\n"_Stz);
  vTrace("--regionPrefix <name>    (prefix of shm region name)\n"_Stz);
  vTrace("--size <i>               (size in bytes of shm region)\n"_Stz);
  //cid
  //metadata always on
  //prompt off


  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* check arguments                                                            */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vSanityCheck(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  if (m_xSysArgs.m_nDefinedNodes > C_MAX_NODES) {
    vTrace("Too many nodes: %lld. Max of %lld\n"_Stz,m_xSysArgs.m_nDefinedNodes,C_MAX_NODES);
    Process::vExitFailure();
  }
  
//  i8 iRequiredThreads =
//    1                 // 1 thread for socket answer
//    + m_xSysArgs.m_nBootNodes        // n threads for socket receiver
//    + 1               // a thread for table loader
//    + 1               // 1 for the pot
//  ;
  
//RJH  if (m_xSysArgs.m_nThreads < iRequiredThreads) {
//RJH    vTrace("Insufficient threads: %lld. At least %lld threads required\n"_Stz,
//RJH           m_xSysArgs.m_nThreads,
//RJH           iRequiredThreads);
//RJH    Process::vExitFailure();
//RJH  }
  
}

/*----------------------------------------------------------------------------*/
/* scribble over arguments                                                    */
/*----------------------------------------------------------------------------*/
void CmdLineArgs::vScribble(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);
  
  byte* aDst = aAdd<byte*>(this,sizeof(Object));
  
  i8 nBytes = sizeof(CmdLineArgs) - sizeof(Object) - sizeof(Footer);
  
  MemLib::vSet(aDst,0xFF,nBytes,CN_CMDLINEARGS);
}

/*----------------------------------------------------------------------------*/
/* is this a simple user prompt                                               */
/*----------------------------------------------------------------------------*/
bool CmdLineArgs::bUserPrompt(void)
{
  _precondition_(_MAGIC_,CN_CMDLINEARGS,_FOOTER_);

  if (m_xSysArgs.m_bDeveloper)
    return false;
  
  if (m_xSysArgs.m_bStart)
    return false;
  
  if (m_xSysArgs.m_bStatus)
    return false;
  
  if (m_xSysArgs.m_bStop)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CmdLineArgs::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
