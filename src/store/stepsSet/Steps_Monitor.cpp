/*============================================================================*/
/* Steps_Monitor.cpp -- Monitor step group                              */
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
#include "Node.hpp"
#include "Grid.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"


#include "Heartbeat.hpp"

extern CmdLineArgs gxCmdLineArgs;

extern Stz *gsBuildGitHash;
extern RingBuffer<Heartbeat> gxHeartbeatData;
 


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Monitor(void)
{
  vAddStep("Monitor::updateNodeStatus"_Stz,  &Step::vMonitorUpdateNodeStatus);
}

/*----------------------------------------------------------------------------*/
/* Monitor::updateNodeStatus -- checks node heartbeat and updates its status  */
/*----------------------------------------------------------------------------*/
void Step::vMonitorUpdateNodeStatus(void)
{
  ENTRY();
  
  vTrace("  ** Step::vMonitorUpdateNodeStatus ** \n"_Stz);

  //Grid* pGrid = AM::pGrid();

  i8  nNodes = AM::nNodes(); //pGrid->nNodes();

  for (i8 iNode = 0; iNode < nNodes; iNode++) {
    //Node* pNode = pGrid->pGetNode(iNode);

    Node *pNode = AM::pGrid()->pGetNode(iNode);

    if (pNode)  {
      i8 iLastUpdate = time(0) - pNode->xGetHeartbeat().m_iSincePosixEpoch;

      vTrace("  ** Step::vMonitorCollectInfo ** Node %s last update:%lld\n"_Stz, iLastUpdate);

      if (iLastUpdate > 10) { // older than 10 seconds
        pNode->vSetNodeStatus(1); // late
      } else {
        pNode->vSetNodeStatus(0); // OK
      }
    }



  }



  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
