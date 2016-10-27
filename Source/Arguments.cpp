/*============================================================================*/
/* Arguments.cpp - command line arguments                                     */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Arguments::Arguments(void)
: Header(CN_ARGUMENTS)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  m_sRegionName = "/RapidsSE_Instance1";
  m_iRegionSize = 1_GB * 4 ;
  m_nPartitions = 1;
  
  m_sCommand    = "";
  
  m_bCommand    = false;
  m_bDocol      = false;
  m_bHelp       = false;
  m_bPrompt     = false;
  m_bStart      = false;
  m_bStatus     = false;
  m_bStop       = false;
  m_bUnlink     = false;

  m_bBgTasks    = true;
  m_bConfig     = true;
  m_sFolder     = "";
  m_bHugePages  = false;
  m_bProgress   = true;
  m_bRestart    = false;
  m_bTouch      = false;
  m_bTracing    = false;
  m_bUnitTests  = false;
  m_bVerbose    = false;
  m_bWelcome    = true;
  
  m_iPartition  = 0;
  
  _checkInitialised_(CN_ARGUMENTS);
  
  vEnvironmentVariables();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Arguments::~Arguments(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* check for environment variables                                            */
/*----------------------------------------------------------------------------*/
void Arguments::vEnvironmentVariables(void)
{
  s256 sVarName;
  s256 sRegionName;
  s256 sRegionSize;
  s256 sPartitions;
  s256 sFolder;
  
  sVarName = "SE_INSTANCE_NAME";
  if (Env::bGet(sVarName,sRegionName))
    m_sRegionName = sRegionName;

  sVarName = "SE_INSTANCE_SIZE";
  if (Env::bGet(sVarName,sRegionSize))
    m_iRegionSize = String::iToI8(sRegionSize.a());

  sVarName = "SE_PARTITIONS";
  if (Env::bGet(sVarName,sPartitions))
    m_nPartitions = String::iToI8(sPartitions.a());
  
  sVarName = "SE_WORKING_FOLDER";
  if (Env::bGet(sVarName,sFolder))
    m_sFolder = sFolder;
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments                                               */
/*----------------------------------------------------------------------------*/
bool Arguments::bParse(int argc, const char* argv[])
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  for (i8 iArg = 1; iArg < argc; iArg++) {
    
    if (String::bEqualTo("--command",argv[iArg],C_NO_CASE)) {
      m_sCommand = argv[++iArg];
      m_bCommand = true;
      m_bPrompt  = true;
      m_bWelcome = false;
      continue;
    }
    
    if (String::bEqualTo("--docol",argv[iArg],C_NO_CASE)) {
      m_bDocol = true;
      continue;
    }
    
    if (
        String::bEqualTo("--help",argv[iArg],C_NO_CASE)
        || String::bEqualTo("-help",argv[iArg],C_NO_CASE)
        || String::bEqualTo("help",argv[iArg],C_NO_CASE)
        || String::bEqualTo("--?",argv[iArg],C_NO_CASE)
        || String::bEqualTo("-?",argv[iArg],C_NO_CASE)
        || String::bEqualTo("?",argv[iArg],C_NO_CASE)
        )  {
      vPrintHelp();
      exit(EXIT_SUCCESS);
    }
    
    if (String::bEqualTo("--partitions",argv[iArg],C_NO_CASE)) {
      m_nPartitions = String::iToI8(argv[++iArg]);
      ASSERT(m_nPartitions <= C_TABLE_PARTITIONS);
      continue;
    }
    
    if (String::bEqualTo("--seprompt",argv[iArg],C_NO_CASE)) {
      m_bPrompt = true;
      continue;
    }
    if (String::bEqualTo("--prompt",argv[iArg],C_NO_CASE)) {
#ifdef _RELEASE_VERSION
      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStart) {
        vTrace("Incorrect argument: --start with %s\n",argv[iArg]);
        return false;
      }
#endif
      m_bPrompt = true;
      continue;
    }
    
    if (String::bEqualTo("--size",argv[iArg],C_NO_CASE)) {
      //m_iRegionSize = String::iToI8(argv[++iArg]);
      bool bResult = Magnitudes::bBytes(argv[++iArg], m_iRegionSize);
      if (!bResult) {
        vTrace("Invalid size argument: --size %s\n",argv[iArg]);
        return false;
      }
      continue;
    }
    
    if (String::bEqualTo("--start",argv[iArg],C_NO_CASE)) {
#ifdef _RELEASE_VERSION
      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n",argv[iArg]);
        return false;
      }
      if (m_bPrompt) {
        vTrace("Incorrect argument: --prompt with %s\n",argv[iArg]);
        return false;
      }
#endif
      m_bStart = true;
      continue;
    }

    if (String::bEqualTo("--status",argv[iArg],C_NO_CASE)) {
#ifdef _RELEASE_VERSION
      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStart) {
        vTrace("Incorrect argument: --start with %s\n",argv[iArg]);
        return false;
      }
      if (m_bPrompt) {
        vTrace("Incorrect argument: --prompt with %s\n",argv[iArg]);
        return false;
      }
#endif
      m_bStatus = true;
      continue;
    }
    
    if (String::bEqualTo("--stop",argv[iArg],C_NO_CASE)) {
#ifdef _RELEASE_VERSION
      if (m_bStart) {
        vTrace("Incorrect argument: --start with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n",argv[iArg]);
        return false;
      }
      if (m_bPrompt) {
        vTrace("Incorrect argument: --prompt with %s\n",argv[iArg]);
        return false;
      }
#endif
      m_bStop = true;
      continue;
    }

    if (String::bEqualTo("--unlink",argv[iArg],C_NO_CASE)) {
      m_bUnlink = true;
      continue;
    }
    
    // options -----------------------------------------------------------------
    
    if (String::bEqualTo("--bgtasks",argv[iArg],C_NO_CASE)) {
      m_bBgTasks = true;
      continue;
    }
    
    if (String::bEqualTo("--config",argv[iArg],C_NO_CASE)) {
      m_bConfig = true;
      continue;
    }
    
    if (String::bEqualTo("--hugepages",argv[iArg],C_NO_CASE)) {
      m_bHugePages = true;
      continue;
    }
    
    if (String::bEqualTo("--progress",argv[iArg],C_NO_CASE)) {
      m_bProgress = true;
      continue;
    }
    if (String::bEqualTo("--noprogress",argv[iArg],C_NO_CASE)) {
      m_bProgress = false;
      continue;
    }
    
    if (String::bEqualTo("--restart",argv[iArg],C_NO_CASE)) {
      m_bRestart = true;
      continue;
    }
    
    if (String::bEqualTo("--touch",argv[iArg],C_NO_CASE)) {
      m_bTouch = true;
      continue;
    }
    if (String::bEqualTo("--notouch",argv[iArg],C_NO_CASE)) {
      m_bTouch = false;
      continue;
    }
    
    if (String::bEqualTo("--tracing",argv[iArg],C_NO_CASE)) {
      m_bTracing = true;
      continue;
    }
    
    if (String::bEqualTo("--unittests",argv[iArg],C_NO_CASE)) {
      m_bUnitTests = true;
      continue;
    }
    
    if (String::bEqualTo("--verbose",argv[iArg],C_NO_CASE)) {
      m_bVerbose = true;
      continue;
    }
    
    if (String::bEqualTo("--welcome",argv[iArg],C_NO_CASE)) {
      m_bWelcome = true;
      continue;
    }
    
    if (String::bEqualTo("--nowelcome",argv[iArg],C_NO_CASE)) {
      m_bWelcome = false;
      continue;
    }
    
    // seitf options -----------------------------------------------------------
    if (String::bEqualTo("--partition",argv[iArg],C_NO_CASE)) {
      m_iPartition = String::iToI8(argv[++iArg]);
      continue;
    }
    
  /*
    if (String::iLength(m_sCommand.a()))
      String::vConcat(m_sCommand.a(),' ');
    
    String::vConcat(m_sCommand.a(),argv[iArg]);
    m_bCommand = true;
    m_bPrompt  = true;
    m_bWelcome = false;
 
    continue;
  */
    
    vTrace("Unrecognized command argument: %s\n",argv[iArg]);
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void Arguments::vPrint(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  vTrace("Args:\n");
  
  if (m_bCommand)  vTrace("  --command\n");
  if (m_bDocol)    vTrace("  --docol\n");
  if (m_bHelp)     vTrace("  --help\n");
  if (m_bPrompt)   vTrace("  --seprompt\n");
  if (m_bStart)    vTrace("  --start\n");
  if (m_bStatus)   vTrace("  --status\n");
  if (m_bStop)     vTrace("  --stop\n");

  vTrace("  --regionName %s\n",m_sRegionName.a());
  vTrace("  --regionSize %lld\n",m_iRegionSize);
  vTrace("  --partition %lld\n",m_iPartition);
  vTrace("  --partitions %lld\n",m_nPartitions);
}

/*----------------------------------------------------------------------------*/
/* print help                                                                 */
/*----------------------------------------------------------------------------*/
void Arguments::vPrintHelp(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  printf("\n");
  
  printf("Help:\n");
  printf("--command <cmd>   (run given command)\n");
  printf("--help            (print this list)\n");
  printf("--list            (print list of available systems)\n");
  printf("--start           (start system)\n");
  printf("--status          (print status of system)\n");
  printf("--stop            (stop system)\n");
  
  printf("\n");
  printf("--partitions <i>  (number of partitions)\n");
  printf("--partition <i>   (which partition)\n");
  printf("--region <name>   (name of shm region)\n");
  printf("--size <i>        (size in bytes of shm region)\n");

  printf("\n");
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Arguments::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
