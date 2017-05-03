/*============================================================================*/
/* Arguments.cpp - command line arguments                                     */
/*============================================================================*/

#include "StorageEngine.h"

void* C_ADDR_POOL = (void*)0x204800000000;
i8 C_MEMORY_PERCENT = 80;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Arguments::Arguments(void)
: Object(CN_ARGUMENTS)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  // calc the number of pool buffers taken by jumbo buffers
  m_nJumboBuffers = C_JUMBO_COEFFICIENT * Hardware::iCoreCount();
  m_nJumboTook    = C_JUMBO_RATIO * m_nJumboBuffers;
  m_iJumboSize    = m_nJumboTook * C_SIZE_POOL_BUFFER;
  
  //m_pRegionAddr = C_ADDR_POOL;
  m_sRegionName = ""_Str;
  m_sRegionPrefix = "/SE"_Str;
  m_iRegionSize = 1_GB * 4 + m_iJumboSize;
  m_nSlots      = 4;
  m_sNodeName   = "node"_Str;
  m_nNodes      = 0;
  m_bTracepoint = false;
  
#ifdef __APPLE__
#ifdef SEMICOLON
  m_sCommands   = "../../../../CommandsSemicolon"_Str;
#else
  m_sCommands   = "../../../../Commands"_Str;
#endif
  m_sSen        = "../Sen/"_Str;
#else
  m_sCommands   = "../../Commands"_Str;
  m_sSen        = "../Sen/"_Str;
#endif
  
  
  m_sCommand    = ""_Str;
  
  m_bCommand    = false;
  m_bDocol      = false;
  m_bHelp       = false;
  m_bPrompt     = false;
  m_bStart      = false;
  m_bStatus     = false;
  m_bStop       = false;
  m_bUnlink     = false;
  m_bLock       = false;
  m_uLevels     = 6;
  m_uFrequency  = 2;

  m_bBgTasks    = true;
  m_bConfig     = true;
  m_sFolder     = ""_Str;
  m_bHugePages  = false;
  m_bProgress   = false;
  m_bRestart    = true;
  m_bTouch      = false;
  m_bTracing    = false;
  m_bUnitTests  = false;
  m_bVerbose    = false;
  m_bWelcome    = true;
  m_iSleep      = 1;
  m_bMunged     = false;
  m_bKillPrev   = false;
  m_bDontFree   = false;
  m_bFreeRB     = false;
  
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
  
  sVarName = "SE_INSTANCE_NAME"_Str;
  if (Env::bGet(sVarName,sRegionName))
    m_sRegionName = sRegionName;

  sVarName = "SE_INSTANCE_SIZE"_Str;
  if (Env::bGet(sVarName,sRegionSize))
    m_iRegionSize = sRegionSize.iToI8();

  sVarName = "SE_PARTITIONS"_Str;
  if (Env::bGet(sVarName,sPartitions))
    m_nSlots = sPartitions.iToI8();
  
  sVarName = "SE_WORKING_FOLDER"_Str;
  if (Env::bGet(sVarName,sFolder))
    m_sFolder = sFolder;
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments                                               */
/*----------------------------------------------------------------------------*/
bool Arguments::bParse(int argc, Str* argv[])
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  i8 iNode = 0;
  
  for (i8 iArg = 1; iArg < argc; iArg++) {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--command"_Str)) {
      m_sCommand = (Str*)argv[++iArg];
      m_bCommand = true;
      m_bPrompt  = true;
      m_bWelcome = false;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--commands"_Str)) {
      m_sCommands = (Str*)argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--sen"_Str)) {
      m_sSen = (Str*)argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--docol"_Str)) {
      m_bDocol = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (
        argv[iArg]->bSame("--help"_Str)
        || argv[iArg]->bSame("-help"_Str)
        || argv[iArg]->bSame("help"_Str)
        || argv[iArg]->bSame("--?"_Str)
        || argv[iArg]->bSame("-?"_Str)
        || argv[iArg]->bSame("?"_Str)
        )  {
      vPrintHelp();
      exit(EXIT_SUCCESS);
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--name"_Str)) {
      m_sNodeName = (Str*)argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--node"_Str)) {
      m_xNodes[iNode].m_sNodeName = (Str*)argv[++iArg];;
      m_xNodes[iNode].m_sIPAddress = (Str*)argv[++iArg];
      m_xNodes[iNode].m_iPortNo = argv[++iArg]->iToI8();
      iNode++;
      
      if (m_nNodes > C_MAX_NODES) {
        vTrace("Maximum number of nodes allowed in the cluster is %lld\n"_Str,C_MAX_NODES);
        return false;
      }
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--partitionsPerNode"_Str)) {
      m_nSlots = argv[++iArg]->iToI8();
      if (m_nSlots > C_SLOTS_PER_NODE) {
        vTrace("Maximum number of partitions allowed per node is %lld\n"_Str,
          C_SLOTS_PER_NODE);
        return false;
      }
      
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--region"_Str)) {
      m_sRegionName = (Str*)argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--regionPrefix"_Str)) {
      m_sRegionPrefix = (Str*)argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //if (argv[iArg]->bSame("--address"_Str)) {
    //  m_pRegionAddr = (void*)argv[++iArg]->iToI8();
    //  continue;
    //}
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--seprompt"_Str)) {
      m_bPrompt = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--prompt"_Str)) {

      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n"_Str,argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n"_Str,argv[iArg]);
        return false;
      }

      m_bPrompt = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--size"_Str)) {
      bool bResult = Magnitudes::bBytes((Str*)argv[++iArg], m_iRegionSize);
      if (!bResult) {
        vTrace("Invalid size argument: --size %s\n"_Str,argv[iArg]);
        return false;
      }
      
      i8 iTotalMemory = MemLib::iGetTotalMemory();
      if (m_iRegionSize > DIV(C_MEMORY_PERCENT * iTotalMemory, 100)) {
        vTrace("Invalid size argument: --size %s\n"_Str,argv[iArg]);
        s32 sMag;
        Magnitudes::vMagnitude(iTotalMemory,sMag);
        vTrace("Cannot set size > %lld%% of memory on this node (node has only %s of memory)\n"_Str,
               C_MEMORY_PERCENT,
               sMag.s());
        return false;
      }

      m_iRegionSize += m_iJumboSize;
      
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--start"_Str)) {

      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n"_Str,argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n"_Str,argv[iArg]);
        return false;
      }

      m_bStart = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--status"_Str)) {
      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n"_Str,argv[iArg]);
        return false;
      }
      if (m_bStart) {
        vTrace("Incorrect argument: --start with %s\n"_Str,argv[iArg]);
        return false;
      }
      if (m_bPrompt) {
        vTrace("Incorrect argument: --prompt with %s\n"_Str,argv[iArg]);
        return false;
      }

      m_bStatus = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--stop"_Str)) {

      if (m_bStart) {
        vTrace("Incorrect argument: --start with %s\n"_Str,argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n"_Str,argv[iArg]);
        return false;
      }
      if (m_bPrompt) {
        vTrace("Incorrect argument: --prompt with %s\n"_Str,argv[iArg]);
        return false;
      }

      m_bStop = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--unlink"_Str)) {
      m_bUnlink = true;
      continue;
    }
    
    // options -----------------------------------------------------------------
    
    if (argv[iArg]->bSame("--bgtasks"_Str)) {
      m_bBgTasks = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--config"_Str)) {
      m_bConfig = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--hugepages"_Str)) {
      m_bHugePages = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--levels"_Str)) {
      m_uLevels = (u8)argv[++iArg]->iToI8();
      continue;
    }
    
    if (argv[iArg]->bSame("--frequency"_Str)) {
      m_uFrequency = (u8)argv[++iArg]->iToI8();
      continue;
    }
    
    if (argv[iArg]->bSame("--lock"_Str)) {
      m_bLock = true;
      continue;
    }

    if (argv[iArg]->bSame("--progress"_Str)) {
      m_bProgress = true;
      continue;
    }
    if (argv[iArg]->bSame("--noprogress"_Str)) {
      m_bProgress = false;
      continue;
    }
    
    //if (argv[iArg]->bSame("--restart"_Str)) {
    //  m_bRestart = true;
    //  continue;
    //}
    
    if (argv[iArg]->bSame("--touch"_Str)) {
      m_bTouch = true;
      continue;
    }
    if (argv[iArg]->bSame("--notouch"_Str)) {
      m_bTouch = false;
      continue;
    }
    
    if (argv[iArg]->bSame("--tracepoint"_Str)) {
      m_bTracepoint = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--tracing"_Str)) {
      m_bTracing = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--unittests"_Str)) {
      m_bUnitTests = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--verbose"_Str)) {
      m_bVerbose = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--welcome"_Str)) {
      m_bWelcome = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--nowelcome"_Str)) {
      m_bWelcome = false;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[iArg]->bSame("--sleep"_Str)) {
      m_iSleep = argv[++iArg]->iToI8();
      continue;
    }
    
    if (argv[iArg]->bSame("--munged"_Str)) {
      m_bMunged = true;
      continue;
    }
  
    if (argv[iArg]->bSame("--killPrev"_Str)) {
      m_bKillPrev = true;
      continue;
    }

    if (argv[iArg]->bSame("--dontFree"_Str)) {
      m_bDontFree = true;
      continue;
    }
    
    if (argv[iArg]->bSame("--freeRB"_Str)) {
      m_bFreeRB = true;
      continue;
    }
    
    vTrace("Unrecognized command argument: %s\n"_Str,argv[iArg]);
    return false;
  }
  
  // if --node was defined just set the m_nNodes
  if (iNode) {
    
    // set no. of nodes
    m_nNodes = iNode;
    
    // some basic cluster validations
    
    // there should be at aleast 2 nodes ina cluster
    if (m_nNodes < 2) {
      vTrace("Minimum two nodes should be there in a cluster\n"_Str);
      return false;
    }
    
    // the current node should have been specfied in the --node
    bool bFound = false;
    for (i8 iIndex = 0; iIndex < m_nNodes; iIndex++) {
      if (m_xNodes[iIndex].m_sNodeName.bSame(m_sNodeName)) {
        bFound = true;
        break;
      }
    }
    
    if ( ! bFound ) {
      vTrace("Add argument --name with name of this node from the --node definitions\n"_Str);
      return false;
    }
    
  } else {
    // add a single node
    m_nNodes = 1;
    m_xNodes[0].m_sNodeName = m_sNodeName;
    m_xNodes[0].m_sIPAddress = ""_Str;
    m_xNodes[0].m_iPortNo =  -1;
  }

  // if no regionName then form region name using regionPrefix and node name
  if (m_sRegionName.bSame(""_Str)) {
    m_sRegionName.vCat(m_sRegionPrefix,"_"_Str,m_sNodeName.s(),nullptr);
  }
  else {
    if (m_bStart && m_nNodes > 1) {
      vTrace("--region not allowed with --start in a cluster\n"_Str);
      return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void Arguments::vPrint(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  vTrace("Args:\n"_Str);
  
  if (m_bCommand)  vTrace("  --command\n"_Str);
  if (m_bDocol)    vTrace("  --docol\n"_Str);
  if (m_bHelp)     vTrace("  --help\n"_Str);
  if (m_bPrompt)   vTrace("  --seprompt\n"_Str);
  if (m_bStart)    vTrace("  --start\n"_Str);
  if (m_bStatus)   vTrace("  --status\n"_Str);
  if (m_bStop)     vTrace("  --stop\n"_Str);

  vTrace("  --name %s\n"_Str,m_sNodeName.s());
  vTrace("  --regionPrefix %s\n"_Str,m_sRegionPrefix.s());
  //vTrace("  --address %p\n"_Str,m_pRegionAddr);
  vTrace("  --size %lld\n"_Str,m_iRegionSize);
  
  for (i8 iIndex=0; iIndex < m_nNodes; iIndex++) {
    vTrace("  --node %s %s %lld\n"_Str,
      m_xNodes[iIndex].m_sNodeName.s(),
      m_xNodes[iIndex].m_sIPAddress.s(),
      m_xNodes[iIndex].m_iPortNo);
  }
  
  vTrace("  --partitionsPerNode %lld\n"_Str,m_nSlots);
  vTrace("  --commands %s\n"_Str,m_sCommands.s());
//#ifdef _DEBUG
  vTrace("  --sen %s\n"_Str,m_sSen.s());
//#endif
  if (m_bMunged) vTrace("  --munged\n"_Str);
  

}

/*----------------------------------------------------------------------------*/
/* print help                                                                 */
/*----------------------------------------------------------------------------*/
void Arguments::vPrintHelp(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  vInfo("\n"_Str);
  
  vInfo("Help:\n"_Str);
  vInfo("--command <cmd>   (run given command)\n"_Str);
  vInfo("--help            (print this list)\n"_Str);
  vInfo("--list            (print list of available systems)\n"_Str);
  vInfo("--start           (start system)\n"_Str);
  vInfo("--status          (print status of system)\n"_Str);
  vInfo("--stop            (stop system)\n"_Str);
  
  vInfo("\n"_Str);
  //vInfo("--partitionsPerNode <i>  (number of partitions per node)\n"_Str);
  vInfo("--node <name> IP port (node information in the cluster)\n"_Str);
  vInfo("--regionPrefix <name>    (prefix of shm region name)\n"_Str);
  vInfo("--size <i>               (size in bytes of shm region)\n"_Str);

  vInfo("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* scribble over arguments                                                    */
/*----------------------------------------------------------------------------*/
void Arguments::vScribble(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  byte* aDst = pAdd<byte*>(this,sizeof(Object));
  
  i8 nBytes = sizeof(Arguments) - sizeof(Object) - sizeof(Footer);
  
  MemLib::vSet(aDst,0xFF,nBytes);
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
