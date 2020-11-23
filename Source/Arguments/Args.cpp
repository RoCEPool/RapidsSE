/*============================================================================*/
/* Args.cpp - command line arguments                                          */
/*============================================================================*/

#include "Arguments.h"

extern Args       gxArgs;
extern AttachMap* gpAttachMap;
extern Log*       gpLog;
extern bool       gbSHM;

i8     gi_argc;
s256   gi_szaArgv[128];

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Args::Args(void)
: Object(CN_ARGUMENTS)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  // configuration
#ifdef WITH_APPLE
  m_sCommandsDir  = "../../../../Commands"_Stz;
#else
  m_sCommandsDir  = "../../Commands"_Stz;
#endif
  
  m_iConnectCode  = C_UNKNOWN;
  m_nDegOfPar     = 4;
  m_sGridName     = "SEgrid"_Stz;
  m_sNodeName     = "node"_Stz;
  m_aRegionAddr   = (void*)C_ADR_REGION;
  m_sRegionName   = ""_Stz;
  m_sRegionPrefix = "/SE"_Stz;
  m_iRegionSize   = (4 * 1_GB);
  m_bRLUDiary     = false;
  m_bRLUTimer     = false;
  
#ifdef WITH_APPLE
  m_sSenDir       = "../Sen/"_Stz;
#else
  m_sSenDir       = "../Sen/"_Stz;
#endif
  m_nPtnsPerTable    = 4;

  m_iLiftNo = 0;
  
  // node configuration
  m_nBootNodes = 0;
  
  m_sCommand = ""_Stz;
  m_sInitial = ""_Stz;

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // commands
  m_bAddNode     = false;
  m_bAppendTrace = false;
  m_bCommand     = false;
  m_bHelp        = false;
  m_bInitial     = false;
  m_bPrompt      = false;
  m_bSethScript  = false;
  m_bStart       = false;
  m_bStatus      = false;
  m_bStop        = false;
  m_bUnlink      = false;

  // options
  m_bConfig          = true;
  m_iCondCode        = 0;
  m_bDebugIndex      = false;
  m_bDebugMode       = false;
  m_bDeveloper       = false;
  m_bDontFree        = false;
  m_nDummyThreads    = 0;
  m_nDummySleep      = 10;
  m_nDummyLoop       = 0;
  m_nEntries         = 0;
  m_bFreeCB          = false;
  m_bGridTest        = false;
  m_bHugePages       = false;
  m_bJot             = false;
  m_bKillPrev        = false;
  m_bLock            = false;
  m_bLog             = false;
  m_nLvlLevels       = C_DEF_LVL_LEVELS;
  m_iLvlRatio        = C_DEF_LVL_RATIO;
  m_bNoMunge         = false;
  m_nMaxLoadErrors   = 0;
  m_nMaxReadyLP      = 32;
  m_bMetadata        = false;
  m_bMunged          = false;
  m_nOperands        = 0;
  m_nPeriodicSleep   = 10;
  m_bProgress        = false;
  m_iPromptDebug     = 0;
  m_bReload          = false;
  m_bRestart         = true;
  m_bRLUDiary        = false;
  m_bRLUTimer        = false;
  m_nSubIngesters    = 4;
  m_nSubLoaders      = 4;
  m_nSupervisorSleep = 10;
  m_iThisNode        = C_UNKNOWN;
  m_nThreads         = 12;
  m_bTouch           = false;
  m_bTracepoint      = false;
  m_bTracing         = false;
  m_bVerbose         = false;
  m_bWelcome         = true;

  _checkInitialised_(CN_ARGUMENTS);
  
  vEnvironmentVariables();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Args::~Args(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* burst env string into command line arguments                               */
/*----------------------------------------------------------------------------*/
i8 Args::nBurstArgs(s256 saArgv[], Stz* szArgStr)
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
/* log the command line arguments                                             */
/*----------------------------------------------------------------------------*/
void Args::vLog(void)
{
  _static_
  
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_ARGS,
              "Command line args:"_Stz); //!format-LogSys
  
  s256 sArg;
  
  for (i8 i_Arg = 0; i_Arg < gi_argc; ) {
    
    sArg = (Stz*)(gi_szaArgv[i_Arg ++].szStz());
    
    while (i_Arg < gi_argc && '-' != *gi_szaArgv[i_Arg].azChar()) {
      StrLib::vConcat(sArg.azChar(),' ');
      StrLib::vConcat(sArg.azChar(),gi_szaArgv[i_Arg ++].azChar());
    }
    
    AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_ARGS,
                "  %s"_Stz //!format-LogSys
                ,sArg.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* check for environment variables                                            */
/*----------------------------------------------------------------------------*/
void Args::vEnvironmentVariables(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  s32  sVarName;
  s256 sRegionName;
  s256 sRegionSize;
  s256 sPartitions;
  s256 sFolder;
  
  sVarName = "SE_INSTANCE_NAME"_Stz;
  if (Env::bGet(sVarName,sRegionName))
    m_sRegionName = sRegionName;

  sVarName = "SE_INSTANCE_SIZE"_Stz;
  if (Env::bGet(sVarName,sRegionSize))
    m_iRegionSize = sRegionSize.iToI8();

  sVarName = "SE_PARTITIONS"_Stz;
  if (Env::bGet(sVarName,sPartitions))
    m_nPtnsPerTable = sPartitions.iToI8();
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments                                               */
/*----------------------------------------------------------------------------*/
bool Args::bParse(i8 argc, s256 argv[])
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
    
  if (argv[1].bSame("shm"_Stz)) {
    gbSHM = true;
    return true;
  }
  
  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
  
  i8 i_BootNode = 0;

  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {
    
    Stz* szArg = argv[i_Arg];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--addNode"_Stz)) {
      m_bAddNode = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--address"_Stz)) {
      m_aRegionAddr = (void*)argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--appendTrace"_Stz)) {
      m_bAppendTrace = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--command"_Stz)) {
      m_sCommand = (Stz*)argv[++i_Arg];
      vTrace("command %s\n"_Stz, m_sCommand.azChar());
      m_bCommand = true;
      m_bPrompt  = true;
      m_bWelcome = false;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--commands"_Stz)) {
      m_sCommandsDir = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--condCode"_Stz)) {
      m_iCondCode = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--config"_Stz)) {
      m_bConfig = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--connectCode"_Stz)) {
      m_iConnectCode = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--debugIndex"_Stz)) {
      m_bDebugIndex = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--debugMode"_Stz)) {
      m_bDebugMode = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--developer"_Stz)) {
      m_bDeveloper = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dontFree"_Stz)) {
      m_bDontFree = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dummyLoop"_Stz)) {
      m_nDummyLoop = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dummySleep"_Stz)) {
      m_nDummySleep = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--dummyThreads"_Stz)) {
      m_nDummyThreads = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (szArg->bSame("--entries"_Stz)) {
      m_nEntries = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--freeCB"_Stz)) {
      m_bFreeCB = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--gridName"_Stz)) {
      m_sGridName = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--gridTest"_Stz)) {
      m_bGridTest = true;
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
      m_bHugePages = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--initial"_Stz)) {
      m_bInitial = true;
      m_sInitial = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--jot"_Stz)) {
      vJotDebug(3);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--killPrev"_Stz)) {
      m_bKillPrev = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lock shared memory pages in memory
    if (szArg->bSame("--lock"_Stz)) {
      m_bLock = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef WITH_SHARED_LOG
    if (szArg->bSame("--log"_Stz)) {
      m_bLog = true;
      continue;
    }
#else
    if (szArg->bSame("--log"_Stz)) {
      vWarn("arg --log ignored; not compiled WITH_SHARED_LOG\n"_Stz);
      continue;
    }
#endif
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--lvlLevels"_Stz)) {
      m_nLvlLevels = argv[++i_Arg].iToI8();
      if (m_nLvlLevels < C_MIN_LVL_LEVELS) {
        vWarn("Minimum number of levels allowed is %lld\n"_Stz,C_MIN_LVL_LEVELS);
        return false;
      }
      if (m_nLvlLevels > C_MAX_LVL_LEVELS) {
        vWarn("Maximum number of levels allowed is %lld\n"_Stz,C_MAX_LVL_LEVELS);
        return false;
      }
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--lvlRatio"_Stz)) {
      m_iLvlRatio = argv[++i_Arg].iToI8();
      if (m_iLvlRatio < C_MIN_LVL_RATIO) {
        vWarn("Minimum ratio of levels allowed is %lld\n"_Stz,C_MIN_LVL_RATIO);
        return false;
      }
      if (m_iLvlRatio > C_MAX_LVL_RATIO) {
        vWarn("Maximum ratio of levels allowed is %lld\n"_Stz,C_MAX_LVL_RATIO);
        return false;
      }
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--maxLoadErrors"_Stz)) {
      m_nMaxLoadErrors = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--maxReadyLP"_Stz)) {
      m_nMaxReadyLP = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--metadata"_Stz)) {
      m_bMetadata = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--munged"_Stz)) {
      m_bMunged = true;
#ifdef WITH_MUNGED_SEN
      continue;
#else
      vWarn("--munged specified but SE not compiled WITH_MUNGED_SEN\n"_Stz);
      return false;
#endif
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--name"_Stz) || argv[i_Arg].bSame("--nodeName"_Stz)) {
      m_sNodeName = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--node"_Stz)) {
      m_xBootNode[i_BootNode].m_sNodeName  = (Stz*)argv[++i_Arg];;
      m_xBootNode[i_BootNode].m_sIPAddress = (Stz*)argv[++i_Arg];
      m_xBootNode[i_BootNode].m_iPortNo    = argv[++i_Arg].iToI8();
      i_BootNode++;
      
      if (m_nBootNodes > C_MAX_NODES) {
        vWarn("Maximum number of nodes allowed in the cluster is %lld\n"_Stz,C_MAX_NODES);
        return false;
      }
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--noLog"_Stz)) {
      m_bLog = false;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--NoMunge"_Stz)) {
      m_bNoMunge = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--noprogress"_Stz)) {
      m_bProgress = false;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--notouch"_Stz)) {
      m_bTouch = false;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--nowelcome"_Stz)) {
      m_bWelcome = false;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--operands"_Stz)) {
      m_nOperands = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--partitionsPerNode"_Stz)) {
      m_nPtnsPerTable = argv[++i_Arg].iToI8();
      if (m_nPtnsPerTable > C_MAX_TABLEPTNS) {
        vWarn("Maximum number of partitions allowed per node is %lld\n"_Stz,
          C_MAX_TABLEPTNS);
        return false;
      }
      m_nDegOfPar = m_nPtnsPerTable;
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--periodicSleep"_Stz)) {
      m_nPeriodicSleep = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--progress"_Stz)) {
      m_bProgress = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--prompt"_Stz)) {
      
      if (m_bStop) {
        vWarn("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      if (m_bStatus) {
        vWarn("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      if (m_bSethScript) {
        vWarn("Incorrect argument: --script with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      m_bPrompt = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--promptDebug"_Stz)) {
      m_iPromptDebug = argv[++i_Arg].iToI8();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--region"_Stz)) {
      m_sRegionName = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--regionPrefix"_Stz)) {
      m_sRegionPrefix = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--reload"_Stz)) {
      m_bReload = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //if (argv[iArg]->bSame("--restart"_Stz)) {
    //  m_bRestart = true;
    //  continue;
    //}
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--RLUdiary"_Stz)) {
      m_bRLUDiary = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--RLUTimer"_Stz)) {
      m_bRLUTimer = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--script"_Stz)) {
      if (m_bStop) {
        vWarn("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bStatus) {
        vWarn("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bPrompt) {
        vWarn("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      m_bSethScript = true;
      m_bWelcome = false;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--seprompt"_Stz)) {
      m_bPrompt = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--sen"_Stz)) {
      m_sSenDir = (Stz*)argv[++i_Arg];
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--size"_Stz)) {
      
      bool bResult = MagLib::bToBytes((Stz*)argv[++i_Arg], m_iRegionSize);
      if (!bResult) {
        vWarn("Invalid size argument: --size %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      i8 iTotalMemory = MemLib::iGetTotalMemory();
      if (m_iRegionSize > DIV(C_MAX_SHM_PERCENT * iTotalMemory, 100)) {
        vWarn("Invalid size argument: --size %s\n"_Stz,argv[i_Arg].szStz());
        s32 sMag;
        MagLib::vByteMag(iTotalMemory,sMag);
        vWarn("Cannot set size > %lld%% of memory on this node (node has only %s of memory)\n"_Stz,
              C_MAX_SHM_PERCENT,
              sMag.azChar());
        return false;
      }
      
      s32 sMinSize;
      MagLib::vByteMag(C_MIN_SHM_REGION, sMinSize);
      
      s32 sSize;
      MagLib::vByteMag(m_iRegionSize, sSize);
      
      if (m_iRegionSize < C_MIN_SHM_REGION) {
        vWarn("Insufficient region size: %s. At least %s is required\n"_Stz,
              sSize.szStz(),
              sMinSize.szStz());
        return false;
      }
            
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--start"_Stz)) {
      
      if (m_bStop) {
        vWarn("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bStatus) {
        vWarn("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      m_bStart = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--status"_Stz)) {
      if (m_bStop) {
        vWarn("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bStart) {
        vWarn("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bPrompt) {
        vWarn("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      m_bStatus = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--stop"_Stz)) {
      
      if (m_bStart) {
        vWarn("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bStatus) {
        vWarn("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      if (m_bPrompt) {
        vWarn("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg].szStz());
        return false;
      }
      
      m_bStop = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--subIngesters"_Stz)) {
      m_nSubIngesters = argv[++i_Arg].iToI8();
      if (m_nThreads > C_MAX_THREADS) {
        vWarn("Maximum number of subIngesters allowed is %lld\n"_Stz,
              C_MAX_THREADS);
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--subloaders"_Stz)) {
      m_nSubLoaders = argv[++i_Arg].iToI8();
      if (m_nThreads > C_MAX_THREADS) {
        vWarn("Maximum number of subloaders allowed is %lld\n"_Stz,
              C_MAX_THREADS);
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--supervisorSleep"_Stz)) {
      m_nSupervisorSleep = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--threads"_Stz)) {
      m_nThreads = argv[++i_Arg].iToI8();
      if (m_nThreads > C_MAX_THREADS) {
        vWarn("Maximum number of thread allowed is %lld\n"_Stz,
               C_MAX_THREADS);
        return false;
      }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--touch"_Stz)) {
      m_bTouch = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--tracepoint"_Stz)) {
      m_bTracepoint = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--tracing"_Stz)) {
      m_bTracing = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--unlink"_Stz)) {
      m_bUnlink = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--verbose"_Stz)) {
      m_bVerbose = true;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (szArg->bSame("--welcome"_Stz)) {
      m_bWelcome = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (szArg->bSame("--liftNo"_Stz)) {
      m_iLiftNo = argv[++i_Arg].iToI8();
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if ('<' == * szArg->azChar()) {
      
      i8 i_TagArg = i_Arg + 1;
      
      if (i_TagArg >= argc) {
        m_oArgTagSet.vInsert(szArg);
        continue;
      }
      
      Stz* szTagEqual = argv[i_TagArg];
      if ('=' != * szTagEqual->azChar()) {
        m_oArgTagSet.vInsert(szArg);
        continue;
      }
      
      // consume <tagNm>
      i_Arg++;

      // consume =
      i_Arg++;

      Stz* szTagValue = argv[i_Arg];

      if (szTagValue->bIsNumeric())
        m_oArgTagSet.vInsert(szArg,szTagValue->iToI8());
      else
        m_oArgTagSet.vInsert(szArg,szTagValue);

      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    vWarn("Unrecognized command argument: %s\n"_Stz,argv[i_Arg].szStz());
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if --node was defined just set the m_nBootNodes
  if (i_BootNode) {
    
    // set no. of nodes
    m_nBootNodes = i_BootNode;
    
    // some basic cluster validations
    
    // there should be at aleast 2 nodes ina cluster
    // if (m_nBootNodes < 2) {
    //   vWarn("Minimum two nodes should be there in a cluster\n"_Stz);
    //   return false;
    // }
    
    // the current node should have been specfied in the --node
    bool bFound = false;
    for (i8 i_Test = 0; i_Test < m_nBootNodes; i_Test++) {
      if (m_xBootNode[i_Test].m_sNodeName.bSame(m_sNodeName)) {
        bFound = true;
        m_iThisNode = i_Test;
        break;
      }
    }

// RJH: moved this into Engine::vCheckNodes() 17 Mar 2020
//    if ( ! bFound ) {
//      vWarn("Add argument --name with name of this node from the --node definitions\n"_Stz);
//      return false;
//    }
    
  } else {
    // add a single node
    // m_nBootNodes = 1;
    // m_xBootNode[0].m_sNodeName = m_sNodeName;
    // m_xBootNode[0].m_sIPAddress = ""_Stz;
    // m_xBootNode[0].m_iPortNo =  -1;
//    if ( m_bStart ) {
//      vWarn("Add argument '--node name ip-address port' for this node %s definition\n"_Stz, m_sNodeName.azChar());
//      return false;
//    }
  }

  // if no regionName then form region name using regionPrefix and node name
  if (m_sRegionName.bSame(""_Stz)) {
    m_sRegionName.vCat(m_sRegionPrefix,"_"_Stz,m_sNodeName.azChar(),nullptr);
  }
  else {
    if (m_bStart && m_nBootNodes > 1) {
      vWarn("--region not allowed with --start in a cluster\n"_Stz);
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void Args::vPrint(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
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
  
  //vTrace("--region %s"_Stz,m_sRegionName.szStz());
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void Args::vPrintStartupInfo(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  // print startup information
  if (! AM::bVerboseProgress())
    return;
  
  
  if (gxArgs.m_bVerbose) {
    vTrace("\n"_Stz,CN_ENUMS);
    vTrace("  No of Classes %lld\n"_Stz,CN_ENUMS);
#ifdef WITH_MUNGED_SEN
    vTrace("  Compiled with MUNGE\n\n"_Stz);
#else
    vTrace("  Not compiled with MUNGE\n"_Stz);
#endif
  }
  

  vTrace("\nArgs:\n"_Stz);
  
#ifdef WITH_DEBUG_VERSION
  if (m_bAppendTrace) vTrace("  --appendTrace\n"_Stz);
  if (m_bCommand)     vTrace("  --command\n"_Stz);
  if (m_bHelp)        vTrace("  --help\n"_Stz);
  if (m_bProgress)    vTrace("  --progress\n"_Stz);
  if (m_bPrompt)      vTrace("  --seprompt\n"_Stz);
  if (m_bSethScript)  vTrace("  --script\n"_Stz);
  if (m_bStart)       vTrace("  --start\n"_Stz);
  if (m_bStatus)      vTrace("  --status\n"_Stz);
  if (m_bStop)        vTrace("  --stop\n"_Stz);
  if (m_bVerbose)     vTrace("  --verbose\n"_Stz);
#endif
  
  vTrace("  --name %s\n"_Stz,m_sNodeName.azChar());
  vTrace("  --region %s\n"_Stz,m_sRegionName.azChar());
  
#ifdef WITH_DEBUG_VERSION
  vTrace("  --address %p\n"_Stz,m_aRegionAddr);
#endif
  
  s32 sSizeMag;
  MagLib::vByteMag(m_iRegionSize,sSizeMag);
  vTrace("  --size %s\n"_Stz,sSizeMag.azChar());
  
  for (i8 i_Index = 0; i_Index < m_nBootNodes; i_Index++) {
    vTrace("  --node %s %s %lld\n"_Stz,
      m_xBootNode[i_Index].m_sNodeName.azChar(),
      m_xBootNode[i_Index].m_sIPAddress.azChar(),
      m_xBootNode[i_Index].m_iPortNo);
  }
  
  vTrace("  --partitionsPerNode %lld\n"_Stz,m_nPtnsPerTable);
  vTrace("  --threads %lld\n"_Stz,m_nThreads);

  if (m_nDummyThreads) {
    vTrace("  --dummyThreads %lld\n"_Stz,m_nDummyThreads);
    vTrace("  --dummySleep %lld\n"_Stz,m_nDummySleep);
    vTrace("  --dummyLoop %lld\n"_Stz,m_nDummyLoop);
    vTrace("  --periodicSleep %lld\n"_Stz,m_nPeriodicSleep);
  }
  
  if (m_bMetadata) vTrace("  --metadata\n"_Stz);

#ifdef WITH_DEBUG_VERSION
  vTrace("  --commands %s\n"_Stz,m_sCommandsDir.azChar());
  vTrace("  --sen %s\n"_Stz,m_sSenDir.azChar());
  if (m_bMunged) vTrace("  --munged\n"_Stz);
#endif
  
  if (m_bDeveloper) vTrace("  --developer\n"_Stz);
  
  for (i8 i_Node = 0; i_Node < m_nBootNodes; i_Node++)
    vTrace("  --node %s ip %s port %lld\n"_Stz,
           m_xBootNode[i_Node].m_sNodeName.szStz(),
           m_xBootNode[i_Node].m_sIPAddress.szStz(),
           m_xBootNode[i_Node].m_iPortNo);
    

  
  Correctness::vTraceWith();
}

/*----------------------------------------------------------------------------*/
/* print help                                                                 */
/*----------------------------------------------------------------------------*/
void Args::vPrintHelp(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  vInfo("\n"_Stz);
  
  vInfo("Help:\n"_Stz);
  vInfo("--command <cmd>   (run given command)\n"_Stz);
  vInfo("--help            (print this list)\n"_Stz);
  vInfo("--list            (print list of available systems)\n"_Stz);
  vInfo("--start           (start system)\n"_Stz);
  vInfo("--status          (print status of system)\n"_Stz);
  vInfo("--stop            (stop system)\n"_Stz);
  
  vInfo("\n"_Stz);
  //vInfo("--partitionsPerNode <i>  (number of partitions per node)\n"_Stz);
  vInfo("--node <name> IP port (node information in the cluster)\n"_Stz);
  vInfo("--regionPrefix <name>    (prefix of shm region name)\n"_Stz);
  vInfo("--size <i>               (size in bytes of shm region)\n"_Stz);

  vInfo("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* check arguments                                                            */
/*----------------------------------------------------------------------------*/
void Args::vSanityCheck(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  if (m_nBootNodes > C_MAX_NODES) {
    vTrace("Too many nodes: %lld. Max of %lld\n"_Stz,m_nBootNodes,C_MAX_NODES);
    Process::vExitFailure();
  }
  
  i8 iRequiredThreads =
    1                 // 1 thread for socket answer
    + m_nBootNodes        // n threads for socket receiver
    + m_nSubLoaders   // a threaad per subloader
    + 1               // a thread for table loader
    + 1               // 1 for the pot
  ;
  
//RJH  if (m_nThreads < iRequiredThreads) {
//RJH    vTrace("Insufficient threads: %lld. At least %lld threads required\n"_Stz,
//RJH           m_nThreads,
//RJH           iRequiredThreads);
//RJH    Process::vExitFailure();
//RJH  }
  
}

/*----------------------------------------------------------------------------*/
/* scribble over arguments                                                    */
/*----------------------------------------------------------------------------*/
void Args::vScribble(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  byte* aDst = aAdd<byte*>(this,sizeof(Object));
  
  i8 nBytes = sizeof(Args) - sizeof(Object) - sizeof(Footer);
  
  MemLib::vSet(aDst,0xFF,nBytes,CN_ARGUMENTS);
}

/*----------------------------------------------------------------------------*/
/* is this a simple user prompt                                               */
/*----------------------------------------------------------------------------*/
bool Args::bUserPrompt(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);

  if (m_bDeveloper)
    return false;
  
  if (m_bStart)
    return false;
  
  if (m_bStatus)
    return false;
  
  if (m_bStop)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Args::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
