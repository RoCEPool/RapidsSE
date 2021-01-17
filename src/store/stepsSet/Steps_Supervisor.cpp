/*============================================================================*/
/* Steps_Supervisor.cpp -- Supervisor step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
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
void Step::vSupervisorCollectInfo(void)
{
  ENTRY();
  
  //vTrace("  ** Step::vSupervisorCollectInfo ** \n"_Stz);

  static i8 iSeq = 0;
   
  Heartbeat xHeartbeat;

  memset((char *) &xHeartbeat, 0x00, sizeof(Heartbeat));
  xHeartbeat.m_u2Version = 1; // hardcode here but it should be defined somewhere
  xHeartbeat.m_iSeqNo = ++iSeq;
  xHeartbeat.m_iSincePosixEpoch = time(0);
  strncpy(xHeartbeat.m_acBuildGitHash, (char *) gsBuildGitHash, 32);

  // Add node name to the packet
  strncpy(xHeartbeat.m_acNodeName, gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(), sizeof(xHeartbeat.m_acNodeName) - 1);
  xHeartbeat.m_acNodeName[sizeof(xHeartbeat.m_acNodeName) - 1] = 0;

  xHeartbeat.m_u2DataLen = sizeof(xHeartbeat.m_iHeader)
      + sizeof(xHeartbeat.m_u2Version)
      + sizeof(xHeartbeat.m_u2DataLen)
      + sizeof(xHeartbeat.m_acNodeName)
      + sizeof(xHeartbeat.m_iSeqNo)
      + sizeof(xHeartbeat.m_iSincePosixEpoch)
      + sizeof(xHeartbeat.m_acBuildGitHash)
      + sizeof(xHeartbeat.xStoreHeartbeat)
      + sizeof(xHeartbeat.xBaseHeartbeat)
      + sizeof(xHeartbeat.m_iChecksum)
      + sizeof(xHeartbeat.m_iFooter)
      + sizeof(xHeartbeat.m_acSpare);



  xHeartbeat.m_iChecksum = fletcher16((unsigned char *) &xHeartbeat, sizeof(Heartbeat));


  //vTrace("  ** Step::vSupervisorCollectInfo ** fletcher16 %ld\n"_Stz, xHeartbeat.m_iChecksum);

  // Add node number to the packet
  //xHeartbeat.m_iValue0 = AM::iThisNodeNo();

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
