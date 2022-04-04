/*============================================================================*/
/* Steps_Supervisor.cpp -- Supervisor step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"


#include "Heartbeat.hpp"

#include "CmdLineArgs.hpp"

extern CmdLineArgs gxCmdLineArgs;

extern Stz *gsBuildGitHash;
extern RingBuffer<Heartbeat> gxHeartbeatData;
 


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Supervisor(void)
{
  vAddStep("Supervisor::collectInfo"_Stz, &Step::vSupervisorCollectInfo);
  vAddStep("Supervisor::halt@"_Stz,       &Step::vSupervisorHaltFetch);
  vAddStep("Supervisor::halt!"_Stz,       &Step::vSupervisorHaltStore);
}

/*----------------------------------------------------------------------------*/
/* Supervisor::collectInfo -- collect heartbeat related info                  */
/*----------------------------------------------------------------------------*/
/**
 * @brief Supervisor Collects Heartbeat Info
 * @details Supervisor Collects Heartbeat Info for this node to send to other nodes
 * <p>
 * @param[in] void * (unused)
 *
 * @return none
 *
 */
void Step::vSupervisorCollectInfo(void)
{
  ENTRY();
  
  //vTrace("  ** Step::vSupervisorCollectInfo ** \n"_Stz);

  static i8 iSeq = 0;
   
  Heartbeat xHeartbeat;

  MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);

  xHeartbeat.m_u2Version = AM::iVersionNumber();
  xHeartbeat.m_iClusterID = gxCmdLineArgs.m_xSysArgs.m_iClusterID;
  xHeartbeat.m_iSeqNo = ++iSeq;
  xHeartbeat.m_iSincePosixEpoch = time(0);
  strncpy(xHeartbeat.m_acBuildGitHash, (char *) gsBuildGitHash, sizeof(xHeartbeat.m_acBuildGitHash));
  xHeartbeat.m_iPid = Process::iGetPid();

  // Add node name to the packet
  MemLib::vZero(xHeartbeat.m_acNodeName,sizeof(xHeartbeat.m_acNodeName),CN_NONE);
  /*
  s32 szNodeName;
  if (gxCmdLineArgs.m_xSysArgs.m_nBootNodes > 1)
    szNodeName = gxCmdLineArgs.m_xSysArgs.m_sNodeName;
  else
    szNodeName.vSprint("%s:%lld"_Stz, gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[0].m_sIPAddress.azChar(),
                     gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[0].m_iPortNo);
  strncpy(xHeartbeat.m_acNodeName, szNodeName.azChar(), sizeof(xHeartbeat.m_acNodeName) - 1);
   */
  strncpy(xHeartbeat.m_acNodeName, gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(), sizeof(xHeartbeat.m_acNodeName) - 1);
  strncpy(xHeartbeat.m_acRegionName, AM::szRegionName()->azChar(), sizeof(xHeartbeat.m_acRegionName) - 1);

  xHeartbeat.xBaseHeartbeat.m_iSystemUsec = AM::iSystemUsec();
  xHeartbeat.xBaseHeartbeat.m_iUserUsec = AM::iUserUsec();
  xHeartbeat.xBaseHeartbeat.m_iResidentSize = AM::iResidentSize();

  // Debug
  /*
  s32 sResident_magn;
  MagLib::vByteMag(xHeartbeat.xBaseHeartbeat.m_iResidentSize,3,sResident_magn);

  vTrace("Node:%s  ** Step::vSupervisorCollectInfo ** SystemUsec:%lld   UserUsec::%lld Resident:%s\n"_Stz,
         gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
         xHeartbeat.xBaseHeartbeat.m_iSystemUsec,
         xHeartbeat.xBaseHeartbeat.m_iUserUsec,
         sResident_magn.azChar());
  */

  xHeartbeat.m_u2DataLen = sizeof(xHeartbeat.m_iHeader)
      + sizeof(xHeartbeat.m_u2Version)
      + sizeof(xHeartbeat.m_u2DataLen)
      + sizeof(xHeartbeat.m_iClusterID)
      + sizeof(xHeartbeat.m_acNodeName)
      + sizeof(xHeartbeat.m_iSeqNo)
      + sizeof(xHeartbeat.m_iSincePosixEpoch)
      + sizeof(xHeartbeat.m_acRegionName)
      + sizeof(xHeartbeat.m_acBuildGitHash)
      + sizeof(xHeartbeat.m_iPid)
      + sizeof(xHeartbeat.m_iNodeNo)
      + sizeof(xHeartbeat.m_acAddr)
      + sizeof(xHeartbeat.m_iPort)
      + sizeof(xHeartbeat.xStoreHeartbeat)
      + sizeof(xHeartbeat.xBaseHeartbeat)
      + sizeof(xHeartbeat.m_iChecksum)
      + sizeof(xHeartbeat.m_iFooter)
      + sizeof(xHeartbeat.m_acSpare);


  //printf("NODE:%s, This node is %lld   ", gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(), iValue );
  Node *pNode = AM::pGrid()->pThisNode();
  if (pNode ) {
    if (pNode->iGetPrevUserUsec() == 0) {
      pNode->vSetPrevUserUsec(xHeartbeat.xBaseHeartbeat.m_iUserUsec);
    }

    // Adding Address and Port
    //
    xHeartbeat.m_iNodeNo = AM::pGrid()->iThisNodeId();
    strncpy(xHeartbeat.m_acAddr, pNode->szIPAddr()->azChar(), sizeof(xHeartbeat.m_acAddr) - 1);
    xHeartbeat.m_acAddr[sizeof(xHeartbeat.m_acAddr) - 1] = 0;
    xHeartbeat.m_iPort = pNode->iPortNo();
    //printf("Hello, My addr:%s and port:%lld\n", pNode->szIPAddr()->azChar(), pNode->iPortNo());
  }

  CheckSum ChecksumValue((unsigned char *) &xHeartbeat, sizeof(Heartbeat));

  xHeartbeat.m_iChecksum = ChecksumValue.uGetFletcher16();

  //vTrace("  ** Step::vSupervisorCollectInfo ** fletcher16 %ld\n"_Stz, xHeartbeat.m_iChecksum);

  // Write to Send-Ringbuffer
  gxHeartbeatData.bTryWrite(xHeartbeat);


  // May as well update this node's heartbeat here
  // So that if we receive heartbeat from the same node (loopback, broadcast), we can ignore

  // NOTE: But for the time being, commented out for testing/debug
  //AM::vPutHeartbeat(AM::iThisNodeNo(), xHeartbeat);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Supervisor::halt@ -- fetch supervisor halt flag                            */
/*----------------------------------------------------------------------------*/
void Step::vSupervisorHaltFetch(void)
{
  ENTRY();
  
  bool bHalt = AM::bSupervisorHalt();
  
  m_pDataStack->vPushInt(bHalt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Supervisor::halt! -- store supervisor halt flag                            */
/*----------------------------------------------------------------------------*/
void Step::vSupervisorHaltStore(void)
{
  ENTRY();
  
  bool bHalt = (bool)m_pDataStack->iPopInt(__func__);
  
  AM::vSupervisorHalt(bHalt);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
