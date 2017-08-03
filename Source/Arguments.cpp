/*============================================================================*/
/* Arguments.cpp - command line arguments                                     */
/*============================================================================*/

#include "StorageEngine.h"

void* C_ADDR_POOL = (void*)0x204800000000;
i8 C_MEMORY_PERCENT = 95;

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
  m_sRegionName = ""_Stz;
  m_sRegionPrefix = "/SE"_Stz;
  m_iRegionSize = 1_GB * 4 + m_iJumboSize;
  m_nSlots      = 4;
  m_sNodeName   = "node"_Stz;
  m_nNodes      = 0;
  m_bTracepoint = false;
  
#ifdef __APPLE__
  m_sCommands   = "../../../../Commands"_Stz;
  m_sSen        = "../Sen/"_Stz;
#else
  m_sCommands   = "../../Commands"_Stz;
  m_sSen        = "../Sen/"_Stz;
#endif
  
  
  m_sCommand    = ""_Stz;
  
  m_bCommand    = false;
  m_bDocol      = false;
  m_bHelp       = false;
  m_bPrompt     = false;
  m_bScript     = false;
  m_bStart      = false;
  m_bStatus     = false;
  m_bStop       = false;
  m_bUnlink     = false;
  m_bLock       = false;
  m_iLevels     = 6;
  m_iFrequency  = 2;

  m_bBgTasks       = true;
  m_bConfig        = true;
  m_sFolder        = ""_Stz;
  m_bHugePages     = false;
  m_bProgress      = false;
  m_bRestart       = true;
  m_bTouch         = false;
  m_bTracing       = false;
  m_bUnitTests     = false;
  m_bVerbose       = false;
  m_bWelcome       = true;
  m_iSleep         = 1;
  m_bMetadata      = false;
  m_bMunged        = false;
  m_bKillPrev      = false;
  m_bDontFree      = false;
  m_bFreeRB        = false;
  
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
  
  sVarName = "SE_INSTANCE_NAME"_Stz;
  if (Env::bGet(sVarName,sRegionName))
    m_sRegionName = sRegionName;

  sVarName = "SE_INSTANCE_SIZE"_Stz;
  if (Env::bGet(sVarName,sRegionSize))
    m_iRegionSize = sRegionSize.iToI8();

  sVarName = "SE_PARTITIONS"_Stz;
  if (Env::bGet(sVarName,sPartitions))
    m_nSlots = sPartitions.iToI8();
  
  sVarName = "SE_WORKING_FOLDER"_Stz;
  if (Env::bGet(sVarName,sFolder))
    m_sFolder = sFolder;
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments                                               */
/*----------------------------------------------------------------------------*/
bool Arguments::bParse(int argc, Stz* argv[])
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  i8 iNode = 0;
  
  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--command"_Stz)) {
      m_sCommand = (Stz*)argv[++i_Arg];
      m_bCommand = true;
      m_bPrompt  = true;
      m_bWelcome = false;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--commands"_Stz)) {
      m_sCommands = (Stz*)argv[++i_Arg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--sen"_Stz)) {
      m_sSen = (Stz*)argv[++i_Arg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--docol"_Stz)) {
      m_bDocol = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (
        argv[i_Arg]->bSame("--help"_Stz)
        || argv[i_Arg]->bSame("-help"_Stz)
        || argv[i_Arg]->bSame("help"_Stz)
        || argv[i_Arg]->bSame("--?"_Stz)
        || argv[i_Arg]->bSame("-?"_Stz)
        || argv[i_Arg]->bSame("?"_Stz)
        )  {
      vPrintHelp();
      exit(EXIT_SUCCESS);
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--name"_Stz)) {
      m_sNodeName = (Stz*)argv[++i_Arg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--node"_Stz)) {
      m_xNodes[iNode].m_sNodeName = (Stz*)argv[++i_Arg];;
      m_xNodes[iNode].m_sIPAddress = (Stz*)argv[++i_Arg];
      m_xNodes[iNode].m_iPortNo = argv[++i_Arg]->iToI8();
      iNode++;
      
      if (m_nNodes > C_MAX_NODES) {
        vError("Maximum number of nodes allowed in the cluster is %lld\n"_Stz,C_MAX_NODES);
        return false;
      }
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--partitionsPerNode"_Stz)) {
      m_nSlots = argv[++i_Arg]->iToI8();
      if (m_nSlots > C_SLOTS_PER_NODE) {
        vError("Maximum number of partitions allowed per node is %lld\n"_Stz,
          C_SLOTS_PER_NODE);
        return false;
      }
      
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--region"_Stz)) {
      m_sRegionName = (Stz*)argv[++i_Arg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--regionPrefix"_Stz)) {
      m_sRegionPrefix = (Stz*)argv[++i_Arg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //if (argv[iArg]->bSame("--address"_Stz)) {
    //  m_pRegionAddr = (void*)argv[++iArg]->iToI8();
    //  continue;
    //}
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--seprompt"_Stz)) {
      m_bPrompt = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--prompt"_Stz)) {

      if (m_bStop) {
        vError("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bStatus) {
        vError("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bScript) {
        vError("Incorrect argument: --script with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      m_bPrompt = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--size"_Stz)) {
      bool bResult = Magnitudes::bBytes((Stz*)argv[++i_Arg], m_iRegionSize);
      if (!bResult) {
        vError("Invalid size argument: --size %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      
      i8 iTotalMemory = MemLib::iGetTotalMemory();
      if (m_iRegionSize > DIV(C_MEMORY_PERCENT * iTotalMemory, 100)) {
        vError("Invalid size argument: --size %s\n"_Stz,argv[i_Arg]);
        s32 sMag;
        Magnitudes::vMagnitude(iTotalMemory,sMag);
        vError("Cannot set size > %lld%% of memory on this node (node has only %s of memory)\n"_Stz,
               C_MEMORY_PERCENT,
               sMag.s());
        return false;
      }

      m_iRegionSize += m_iJumboSize;
      
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--start"_Stz)) {

      if (m_bStop) {
        vError("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bStatus) {
        vError("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg]);
        return false;
      }

      m_bStart = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--status"_Stz)) {
      if (m_bStop) {
        vError("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bStart) {
        vError("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bPrompt) {
        vError("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg]);
        return false;
      }

      m_bStatus = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--stop"_Stz)) {

      if (m_bStart) {
        vError("Incorrect argument: --start with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bStatus) {
        vError("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bPrompt) {
        vError("Incorrect argument: --prompt with %s\n"_Stz,argv[i_Arg]);
        return false;
      }

      m_bStop = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--unlink"_Stz)) {
      m_bUnlink = true;
      continue;
    }
    
    // options -----------------------------------------------------------------
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--script"_Stz)) {
      if (m_bStop) {
        vError("Incorrect argument: --stop with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bStatus) {
        vError("Incorrect argument: --status with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      if (m_bPrompt) {
        vError("Incorrect argument: --script with %s\n"_Stz,argv[i_Arg]);
        return false;
      }
      m_bScript = true;
      m_bWelcome = false;
      continue;
    }

    if (argv[i_Arg]->bSame("--bgtasks"_Stz)) {
      m_bBgTasks = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--config"_Stz)) {
      m_bConfig = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--hugepages"_Stz)) {
      m_bHugePages = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--levels"_Stz)) {
      m_iLevels = argv[++i_Arg]->iToI8();
      continue;
    }
    
    if (argv[i_Arg]->bSame("--frequency"_Stz)) {
      m_iFrequency = argv[++i_Arg]->iToI8();
      continue;
    }
    
    if (argv[i_Arg]->bSame("--lock"_Stz)) {
      m_bLock = true;
      continue;
    }

    if (argv[i_Arg]->bSame("--progress"_Stz)) {
      m_bProgress = true;
      continue;
    }
    if (argv[i_Arg]->bSame("--noprogress"_Stz)) {
      m_bProgress = false;
      continue;
    }
    
    //if (argv[iArg]->bSame("--restart"_Stz)) {
    //  m_bRestart = true;
    //  continue;
    //}
    
    if (argv[i_Arg]->bSame("--touch"_Stz)) {
      m_bTouch = true;
      continue;
    }
    if (argv[i_Arg]->bSame("--notouch"_Stz)) {
      m_bTouch = false;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--tracepoint"_Stz)) {
      m_bTracepoint = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--tracing"_Stz)) {
      m_bTracing = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--unittests"_Stz)) {
      m_bUnitTests = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--verbose"_Stz)) {
      m_bVerbose = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--welcome"_Stz)) {
      m_bWelcome = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--nowelcome"_Stz)) {
      m_bWelcome = false;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (argv[i_Arg]->bSame("--sleep"_Stz)) {
      m_iSleep = argv[++i_Arg]->iToI8();
      continue;
    }
    
    if (argv[i_Arg]->bSame("--metadata"_Stz)) {
      m_bMetadata = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--munged"_Stz)) {
      m_bMunged = true;
      continue;
    }
  
    if (argv[i_Arg]->bSame("--killPrev"_Stz)) {
      m_bKillPrev = true;
      continue;
    }

    if (argv[i_Arg]->bSame("--dontFree"_Stz)) {
      m_bDontFree = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--freeRB"_Stz)) {
      m_bFreeRB = true;
      continue;
    }
    
    vError("Unrecognized command argument: %s\n"_Stz,argv[i_Arg]);
    return false;
  }
  
  // if --node was defined just set the m_nNodes
  if (iNode) {
    
    // set no. of nodes
    m_nNodes = iNode;
    
    // some basic cluster validations
    
    // there should be at aleast 2 nodes ina cluster
    if (m_nNodes < 2) {
      vError("Minimum two nodes should be there in a cluster\n"_Stz);
      return false;
    }
    
    // the current node should have been specfied in the --node
    bool bFound = false;
    for (i8 i_Index = 0; i_Index < m_nNodes; i_Index++) {
      if (m_xNodes[i_Index].m_sNodeName.bSame(m_sNodeName)) {
        bFound = true;
        break;
      }
    }
    
    if ( ! bFound ) {
      vError("Add argument --name with name of this node from the --node definitions\n"_Stz);
      return false;
    }
    
  } else {
    // add a single node
    m_nNodes = 1;
    m_xNodes[0].m_sNodeName = m_sNodeName;
    m_xNodes[0].m_sIPAddress = ""_Stz;
    m_xNodes[0].m_iPortNo =  -1;
  }

  // if no regionName then form region name using regionPrefix and node name
  if (m_sRegionName.bSame(""_Stz)) {
    m_sRegionName.vCat(m_sRegionPrefix,"_"_Stz,m_sNodeName.s(),nullptr);
  }
  else {
    if (m_bStart && m_nNodes > 1) {
      vError("--region not allowed with --start in a cluster\n"_Stz);
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
  
  vTrace("Args:\n"_Stz);
  
  if (m_bCommand)  vTrace("  --command\n"_Stz);
  if (m_bDocol)    vTrace("  --docol\n"_Stz);
  if (m_bHelp)     vTrace("  --help\n"_Stz);
  if (m_bPrompt)   vTrace("  --seprompt\n"_Stz);
  if (m_bScript)   vTrace("  --script\n"_Stz);
  if (m_bStart)    vTrace("  --start\n"_Stz);
  if (m_bStatus)   vTrace("  --status\n"_Stz);
  if (m_bStop)     vTrace("  --stop\n"_Stz);

  vTrace("  --name %s\n"_Stz,m_sNodeName.s());
  vTrace("  --regionPrefix %s\n"_Stz,m_sRegionPrefix.s());
  //vTrace("  --address %p\n"_Stz,m_pRegionAddr);
  vTrace("  --size %lld\n"_Stz,m_iRegionSize);
  
  for (i8 i_Index = 0; i_Index < m_nNodes; i_Index++) {
    vTrace("  --node %s %s %lld\n"_Stz,
      m_xNodes[i_Index].m_sNodeName.s(),
      m_xNodes[i_Index].m_sIPAddress.s(),
      m_xNodes[i_Index].m_iPortNo);
  }
  
  vTrace("  --partitionsPerNode %lld\n"_Stz,m_nSlots);
  vTrace("  --commands %s\n"_Stz,m_sCommands.s());
//#ifdef _DEBUG
  vTrace("  --sen %s\n"_Stz,m_sSen.s());
//#endif
  if (m_bMunged) vTrace("  --munged\n"_Stz);
  if (m_bMetadata) vTrace("  --metadata\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print help                                                                 */
/*----------------------------------------------------------------------------*/
void Arguments::vPrintHelp(void)
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
/* scribble over arguments                                                    */
/*----------------------------------------------------------------------------*/
void Arguments::vScribble(void)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  byte* aDst = pAdd<byte*>(this,sizeof(Object));
  
  i8 nBytes = sizeof(Arguments) - sizeof(Object) - sizeof(Footer);
  
  MemLib::vSet(aDst,0xFF,nBytes,CN_ARGUMENTS);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Arguments::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
