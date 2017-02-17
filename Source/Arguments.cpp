/*============================================================================*/
/* Arguments.cpp - command line arguments                                     */
/*============================================================================*/

#include "StorageEngine.h"

void* C_ADDR_POOL = (void*)0x204800000;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Arguments::Arguments(void)
: Header(CN_ARGUMENTS)
{
  _precondition_(_MAGIC_,CN_ARGUMENTS,_FOOTER_);
  
  // calc the number of pool buffers taken by jumbo buffers
  m_nJumboBuffers = C_JUMBO_COEFFICIENT * System::iCoreCount();
  m_nJumboTook    = C_JUMBO_RATIO * m_nJumboBuffers;
  m_iJumboSize    = m_nJumboTook * C_SIZE_POOL_BUFFER;
  
  m_pRegionAddr = C_ADDR_POOL;
  m_sRegionName = "";
  m_sRegionPrefix = "/SE";
  m_iRegionSize = 1_GB * 4 + m_iJumboSize;
  m_nSlots      = 4;
  m_sNodeName   = "node";
  m_nNodes      = 0;
  m_bTraceapi   = false;
  m_bTracepoint = false;
  
#ifdef __APPLE__
  m_sCommands   = "../../../../Commands";
  m_sSen        = "../Sen/";
#else
  m_sCommands   = "../../Commands";
  m_sSen        = "../Sen/";
#endif
  
  
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
    m_nSlots = String::iToI8(sPartitions.a());
  
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
  
  i8 iNode = 0;
  
  for (i8 iArg = 1; iArg < argc; iArg++) {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--command",argv[iArg],C_NO_CASE)) {
      m_sCommand = argv[++iArg];
      m_bCommand = true;
      m_bPrompt  = true;
      m_bWelcome = false;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--commands",argv[iArg],C_NO_CASE)) {
      m_sCommands = argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--sen",argv[iArg],C_NO_CASE)) {
      m_sSen = argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--docol",argv[iArg],C_NO_CASE)) {
      m_bDocol = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--name",argv[iArg],C_NO_CASE)) {
      m_sNodeName = argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--node",argv[iArg],C_NO_CASE)) {
      m_xNodes[iNode].m_sNodeName = argv[++iArg];;
      m_xNodes[iNode].m_sIPAddress = argv[++iArg];
      m_xNodes[iNode].m_iPortNo =  String::iToI8(argv[++iArg]);
      iNode++;
      
      if (m_nNodes > C_MAX_NODES) {
        vTrace("Maximum number of nodes allowed in the cluster is %lld\n",C_MAX_NODES);
        return false;
      }
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--partitionsPerNode",argv[iArg],C_NO_CASE)) {
      m_nSlots = String::iToI8(argv[++iArg]);
      if (m_nSlots > C_SLOTS_PER_NODE) {
        vTrace("Maximum number of partitions allowed per node is %lld\n",
          C_SLOTS_PER_NODE);
        return false;
      }
      
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--region",argv[iArg],C_NO_CASE)) {
      m_sRegionName = argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--regionPrefix",argv[iArg],C_NO_CASE)) {
      m_sRegionPrefix = argv[++iArg];
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--address",argv[iArg],C_NO_CASE)) {
      m_pRegionAddr = (void*)String::iToI8(argv[++iArg]);
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--seprompt",argv[iArg],C_NO_CASE)) {
      m_bPrompt = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--prompt",argv[iArg],C_NO_CASE)) {

      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n",argv[iArg]);
        return false;
      }

      m_bPrompt = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--size",argv[iArg],C_NO_CASE)) {
      //m_iRegionSize = String::iToI8(argv[++iArg]);
      bool bResult = Magnitudes::bBytes(argv[++iArg], m_iRegionSize);
      if (!bResult) {
        vTrace("Invalid size argument: --size %s\n",argv[iArg]);
        return false;
      }
      
      m_iRegionSize += m_iJumboSize;
      
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--start",argv[iArg],C_NO_CASE)) {

      if (m_bStop) {
        vTrace("Incorrect argument: --stop with %s\n",argv[iArg]);
        return false;
      }
      if (m_bStatus) {
        vTrace("Incorrect argument: --status with %s\n",argv[iArg]);
        return false;
      }

      m_bStart = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--status",argv[iArg],C_NO_CASE)) {
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

      m_bStatus = true;
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--stop",argv[iArg],C_NO_CASE)) {

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

      m_bStop = true;
      continue;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
    
    //if (String::bEqualTo("--restart",argv[iArg],C_NO_CASE)) {
    //  m_bRestart = true;
    //  continue;
    //}
    
    if (String::bEqualTo("--touch",argv[iArg],C_NO_CASE)) {
      m_bTouch = true;
      continue;
    }
    if (String::bEqualTo("--notouch",argv[iArg],C_NO_CASE)) {
      m_bTouch = false;
      continue;
    }
    
    if (String::bEqualTo("--Traceapi",argv[iArg],C_NO_CASE)) {
      m_bTraceapi = true;
      continue;
    }
    
    if (String::bEqualTo("--tracepoint",argv[iArg],C_NO_CASE)) {
      m_bTracepoint = true;
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
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (String::bEqualTo("--sleep",argv[iArg],C_NO_CASE)) {
      m_iSleep = String::iToI8(argv[++iArg]);
      continue;
    }
    
    if (String::bEqualTo("--munged",argv[iArg],C_NO_CASE)) {
      m_bMunged = true;
      continue;
    }
  
    if (String::bEqualTo("--killPrev",argv[iArg],C_NO_CASE)) {
      m_bKillPrev = true;
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
  
  // if --node was defined just set the m_nNodes
  if (iNode) {
    
    // set no. of nodes
    m_nNodes = iNode;
    
    // some basic cluster validations
    
    // there should be at aleast 2 nodes ina cluster
    if (m_nNodes < 2) {
      vTrace("Minimum two nodes should be there in a cluster\n");
      return false;
    }
    
    // the current node should have been specfied in the --node
    bool bFound = false;
    for (i8 iIndex = 0; iIndex < m_nNodes; iIndex++) {
      if (String::bEqualTo(m_xNodes[iIndex].m_sNodeName.a(), m_sNodeName.a(), C_NO_CASE)) {
        bFound = true;
        break;
      }
    }
    
    if ( ! bFound ) {
      vTrace("Add argument --name with name of this node from the --node definitions\n");
      return false;
    }
    
  } else {
    // add a single node
    m_nNodes = 1;
    m_xNodes[0].m_sNodeName = m_sNodeName;
    m_xNodes[0].m_sIPAddress = "";
    m_xNodes[0].m_iPortNo =  -1;
  }

  // if no regionName then form region name using regionPrefix and node name
  if (String::bEqualTo(m_sRegionName.a(),"")) {
    String::vConcat( m_sRegionName.a(), m_sRegionPrefix.a());
    String::vConcat( m_sRegionName.a(), "_");
    String::vConcat( m_sRegionName.a(), m_sNodeName.a());
  }
  else {
    if (m_bStart && m_nNodes > 1) {
      vTrace("--region not allowed with --start in a cluster\n");
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
  
  vTrace("Args:\n");
  
  if (m_bCommand)  vTrace("  --command\n");
  if (m_bDocol)    vTrace("  --docol\n");
  if (m_bHelp)     vTrace("  --help\n");
  if (m_bPrompt)   vTrace("  --seprompt\n");
  if (m_bStart)    vTrace("  --start\n");
  if (m_bStatus)   vTrace("  --status\n");
  if (m_bStop)     vTrace("  --stop\n");

  vTrace("  --name %s\n",m_sNodeName.a());
  vTrace("  --regionPrefix %s\n",m_sRegionPrefix.a());
  vTrace("  --address %p\n",m_pRegionAddr);
  vTrace("  --size %lld\n",m_iRegionSize);
  
  for (i8 iIndex=0; iIndex < m_nNodes; iIndex++) {
    vTrace("  --node %s %s %lld\n",
      m_xNodes[iIndex].m_sNodeName.a(),
      m_xNodes[iIndex].m_sIPAddress.a(),
      m_xNodes[iIndex].m_iPortNo);
  }
  
  vTrace("  --partitionsPerNode %lld\n",m_nSlots);
  vTrace("  --commands %s\n",m_sCommands.a());
//#ifdef _DEBUG
  vTrace("  --sen %s\n",m_sSen.a());
//#endif
  if (m_bMunged) vTrace("  --munged\n");
  

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
  //printf("--partitionsPerNode <i>  (number of partitions per node)\n");
  printf("--node <name> IP port (node information in the cluster)\n");
  printf("--regionPrefix <name>    (prefix of shm region name)\n");
  printf("--size <i>               (size in bytes of shm region)\n");

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
