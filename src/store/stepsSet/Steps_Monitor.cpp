/*============================================================================*/
/* Steps_Monitor.cpp -- Monitor step group                              */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
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
#include "Socket.hpp"

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

  // Get number of nodes in the grid/cluster
  //i8 nNodes = AM::nNodes();

  Heartbeat xHeartbeat;

  for (i8 iNode = 0; iNode < C_MAX_NODES; iNode++) {

    Node *pNode = AM::pGrid()->pFindNodeById(iNode);

    // when we monitoring, we need to skip the null node, it might be removed as the
    // node had failed.
    if (!pNode) continue;

    //if (pNode->iGetNodeStatus() == 99 ) // skip node has been down
    //  continue;
/* debug
    xHeartbeat = pNode->xGetHeartbeat();

    pNode->vSetNodeStatus(1); // need to remove this line
    i8 iLastUpdate = time(0) - xHeartbeat.m_iSincePosixEpoch;
    vTrace("Node:%s  ** Step::vMonitorUpdateNodeStatus ** Node %s last update:%lld\n"_Stz,
           gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
           xHeartbeat.m_acNodeName,
           iLastUpdate); */
    continue;
    if (xHeartbeat.m_iSincePosixEpoch > 0) {
      i8 iLastUpdate = time(0) - xHeartbeat.m_iSincePosixEpoch;

      // This 10 seconds max should be defined somewhere
      if (iLastUpdate > 10) { // older than 10 seconds
        pNode->vSetNodeStatus(1); // late

        vTrace("Node:%s  ** Step::vMonitorUpdateNodeStatus ** Node %s last update:%lld\n"_Stz,
               gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
               xHeartbeat.m_acNodeName,
               iLastUpdate);

        // One way for sure is try to socket-connect to this node, if failed then node-is-down message
        TestConnection TestSocket(pNode->szIPAddr(), pNode->iPortNo());
        if (!TestSocket.bTryConnect()) {
          vTrace("Node:%s reports Node %s is DOWN, failed connection test.\n"_Stz, gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
                 xHeartbeat.m_acNodeName);
          pNode->vSetNodeStatus(99); // DOWN


          // This node is down, better to remove its sockets

          SysCall sysCall;

          Socket *pSock = pNode->pRecvSocket();
          if (pSock) {
            pNode->vRecvSocket((Socket *) nullptr);
            pSock->bClose(sysCall);
            //delete pSock;
          }

          pSock = pNode->pSendSocket();
          if (pSock) {
            pNode->vSendSocket((Socket *) nullptr);
            pSock->bClose(sysCall);
            //delete pSock;
          }

          //AM::pGrid()->vDecConnectedNodes();



          // a longer period, this node has not reported and has been marked as "down"
          // try to restart or remove it from the cluster
          if (iLastUpdate > 40 && pNode->iGetNodeStatus() == 99) {


            // SAVE THIS AUTO RESTART CODE FOR NOW
            // Try to restart, if this node is NODE0
            /*
            if (gxCmdLineArgs.m_xSysArgs.m_sNodeName.bSame((Stz *) "NODE0")) {
              // only if we have the SE start command
              Stz *pCmd = pNode->pszGetStartCmd();
              if (pCmd) {
                vTrace("NODE: %s TRYING TO RESTART NODE: %s\n %s\n"_Stz,
                       gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(), xHeartbeat.m_acNodeName,
                       pCmd->azChar());

                // Restart the node
                i8 iResult = system(pCmd->azChar());
                if (-1 == iResult || 127 == iResult) {
                  vTrace("FAILED TO RESTART NODE %s\n"_Stz, xHeartbeat.m_acNodeName);
                } else {
                  AM::pGrid()->vIncConnectedNodes();
                }
              }
            }*/

            AM::vRemoveNode((Stz*)xHeartbeat.m_acNodeName);
            vTrace("NODE: %s is REMOVING NODE: %s from the cluster\n"_Stz,
                   gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(), xHeartbeat.m_acNodeName
                   );
          }
        }

      } else {
        pNode->vSetNodeStatus(0); // OK
      }

      //i8 iDelta = pNode->iGetPrevUserUsec() + pNode->xGetHeartbeat().xBaseHeartbeat.m_iUserUsec;
      /*
      vTrace("Node:%s  ** Step::vMonitorUpdateNodeStatus ** Node %s saved:%lld Now:%lld Delta:%lld\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar(),
             pNode->xGetHeartbeat().m_acNodeName,
             pNode->iGetPrevUserUsec(),
             pNode->xGetHeartbeat().xBaseHeartbeat.m_iUserUsec,
             pNode->iGetPrevUserUsec() + pNode->xGetHeartbeat().xBaseHeartbeat.m_iUserUsec);
             */

    }

  } // for

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
