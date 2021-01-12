/*============================================================================*/
/* Steps_Supervisor.cpp -- Supervisor step group                              */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
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

extern CmdLineArgs gxCmdLineArgs;

extern Stz *gsBuildGitHash;
extern RingBuffer<Heartbeat> gxHeartbeatData;
 


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Supervisor(void)
{
  vAddStep("Supervisor::collectInfo"_Stz,  &Step::vSupervisorCollectInfo);
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
  xHeartbeat.m_iSeqNo = ++iSeq;
  xHeartbeat.m_iSincePosixEpoch = time(0);
  strncpy(xHeartbeat.m_acBuildGitHash, (char *) gsBuildGitHash, 32);

  // Add node name to the packet
  strncpy(xHeartbeat.m_acString0, gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(), sizeof(xHeartbeat.m_acString0) - 1);
  xHeartbeat.m_acString0[sizeof(xHeartbeat.m_acString0) - 1] = 0;

  // Add node number to the packet
  xHeartbeat.m_iValue0 = AM::iThisNodeNo();

  // Write to Send Ringbuffer
  gxHeartbeatData.bTryWrite(xHeartbeat);


  // May as well update this node's heartbeat here
  // So that if we receive heartbeat from the same node (loopback, broadcast), we can ignore

  // NOTE: But for the time being, commented out for testing/debug
  //AM::vPutHeartbeat((Stz*)xHeartbeat.m_acString0, xHeartbeat);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
