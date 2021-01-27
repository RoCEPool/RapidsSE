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

#include "CmdLineArgs.hpp"

extern CmdLineArgs gxCmdLineArgs;
extern Stz *gsBuildGitHash;
extern RingBuffer<Heartbeat> gxHeartbeatData;



/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Monitor(void) {
  vAddStep("Monitor::updateNodeStatus"_Stz, &Step::vMonitorUpdateNodeStatus);
}

/*----------------------------------------------------------------------------*/
/* Monitor::updateNodeStatus -- checks node heartbeat and updates its status  */
/*----------------------------------------------------------------------------*/
void Step::vMonitorUpdateNodeStatus(void) {
  ENTRY();

  //vTrace("  ** Step::vMonitorUpdateNodeStatus ** \n"_Stz);

  i8 nNodes = AM::nNodes();

  for (i8 iNode = 0; iNode < nNodes; iNode++) {

    Node *pNode = AM::pGrid()->pGetNode(iNode);

    if (pNode && pNode->xGetHeartbeat().m_iSincePosixEpoch > 0) {
      i8 iLastUpdate = time(0) - pNode->xGetHeartbeat().m_iSincePosixEpoch;

      // This 20 seconds max should be defined somewhere
      if (iLastUpdate > 20) { // older than 20 seconds
        pNode->vSetNodeStatus(1); // late

        vTrace("Node:%s  ** Step::vMonitorUpdateNodeStatus ** Node %s last update:%lld\n"_Stz,
               gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
               pNode->xGetHeartbeat().m_acNodeName,
               iLastUpdate);

        // a longer period, this node has not reportedor has been mark as down from somewhere else
        if (iLastUpdate > 60 || pNode->iGetNodeStatus() == 99) {
          // One way for sure is try to socket-connect to this node, if failed then node-is-down message
          TestConnection TestSocket(pNode->szIPAddr(), pNode->iPortNo());
          if (!TestSocket.bTryConnect()) {
            fprintf(stderr, "Node %s is DOWN\n", pNode->xGetHeartbeat().m_acNodeName);
            pNode->vSetNodeStatus(99); // DOWN
          }
        }

      } else {
        pNode->vSetNodeStatus(0); // OK
      }

      i8 iDelta = pNode->iGetPrevUserUsec() + pNode->xGetHeartbeat().xBaseHeartbeat.m_iUserUsec;
      vTrace("Node:%s  ** Step::vMonitorUpdateNodeStatus ** Node %s saved:%lld Now:%lld Delta:%lld\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
             pNode->xGetHeartbeat().m_acNodeName,
             pNode->iGetPrevUserUsec(),
             pNode->xGetHeartbeat().xBaseHeartbeat.m_iUserUsec,
             pNode->iGetPrevUserUsec() + pNode->xGetHeartbeat().xBaseHeartbeat.m_iUserUsec);

    }

  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
