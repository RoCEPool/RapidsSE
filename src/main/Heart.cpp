/*============================================================================*/
/* Heart.cpp - system heart                                                   */
/*============================================================================*/



#include <cstdio>
#include <cstring>
#include "base/BASE.h"

#include "Heart.h"
#include "Heartbeat.hpp"

#include "CmdLineArgs.hpp"


extern CmdLineArgs gxCmdLineArgs;

extern Stz *gsBuildGitHash;
extern Stz *gsBuildDate;
extern Stz *gsBuildHostName;
extern Stz *gsBuildUserName;
extern Stz *gsBuildBranch;


// global
RingBuffer<Heartbeat> gxHeartbeatBuffer(1000); // receiving from network
RingBuffer<Heartbeat> gxHeartbeatData(100); // collect heartbeat data from the system to be sent out


/*----------------------------------------------------------------------------*/
/* heart initialise                                                           */
/*----------------------------------------------------------------------------*/
void vHeartInit(void) {

  if (gxCmdLineArgs.m_xSysArgs.m_nDefinedHeartbeats > 0) {
    //vTrace("SE Heart (sizeof(Heartbeat)=%lld\n"_Stz, (i8) sizeof(Heartbeat));

    int iPort = gxCmdLineArgs.m_xSysArgs.m_xHeartbeatDefn[0].m_iPortNo;
    char szAddr[32];

    if (iPort == 0) {
      iPort = 8888;
    }

    strncpy(szAddr, gxCmdLineArgs.m_xSysArgs.m_xHeartbeatDefn[0].m_sIPAddress.azChar(), sizeof(szAddr) - 1);
    szAddr[sizeof(szAddr) - 1] = 0;

    //User can put on the command line argument as --heartbeat NODE0 BROADCAST 8888
    //strcpy(szAddr, "192.168.0.255"); // for DEBUG
    HeartbeatSnd *pSender = new HeartbeatSnd(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartSndLoop, pSender);

    strcpy(szAddr, "ANY"); // For receiving Broadcast, any interface is the option
    HeartbeatRcv *pReceiver = new HeartbeatRcv(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartRcvLoop, pReceiver);

    // Create Reader thread to maintain the Grid
    PsxThd::iSpawnPosixThread(vHeartReadLoop, nullptr);


  } else {
    vTrace("SE Heartbeat is not set for this node\n"_Stz);
  }

  // Process::vExitFailure();
}




/*----------------------------------------------------------------------------*/
/* heart send loop: pick up a packet from ringbuffer and send                 */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartSndLoop(void *pFuncObj)
{

  HeartbeatSnd *pSender = (HeartbeatSnd *) pFuncObj;

  Heartbeat xHeartbeat;
  char azHeartbeat[C_1KB];
  int iCnt = 0;

  MemLib::vZero(&azHeartbeat,sizeof(azHeartbeat),CN_NONE);

  for (;;) {
    sleep(3); // send every 3 seconds

    if (gxHeartbeatData.bTryRead(xHeartbeat)) {

      pSender->iSend((char *) &xHeartbeat, sizeof(Heartbeat));

      if (iCnt++ == 0) { // debug only: a few prints
        //vTrace("Node: %s sent HB\n"_Stz, xHeartbeat.m_acNodeName);
        // faulty injected message
        //u2 uVal = AM::iVersionNumber() ;
        //memcpy(&azHeartbeat[8], (char *) &uVal, sizeof(AM::iVersionNumber()));
        //pSender->iSend((char *) azHeartbeat, 10);
      }
    }
  }

}

/*----------------------------------------------------------------------------*/
/* heart receive loop: gets a packet from network then writes to ringbuffer   */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartRcvLoop(void *pFuncObj)
{

  HeartbeatRcv *pReceiver = (HeartbeatRcv *) pFuncObj;

  Heartbeat xHeartbeat;

  char azHeartbeat[C_1KB];
  int iCnt = 0;
  MemLib::vZero(&azHeartbeat,sizeof(azHeartbeat),CN_NONE);

  if (! AM::SYSTEM::bStarted()) {
    // delay receiving 5 seconds when SE not started
    // Although, no harms to receive HB, we will ignore them as SE is sot started.
    sleep(5);
  }

  for (;;) {

    //MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);

    int iLen = pReceiver->iRecv((char *) &xHeartbeat, sizeof(xHeartbeat));

    if (iLen >= 10) { // if < 10 bytes, ignore
      i8 iSaveChk = xHeartbeat.m_iChecksum;

      xHeartbeat.m_iChecksum = 0;

      // recalculate checksum and compare
      CheckSum ChecksumValue((unsigned char *) &xHeartbeat, sizeof(Heartbeat));

      if (ChecksumValue.uGetFletcher16() == iSaveChk) {
        xHeartbeat.m_iChecksum = iSaveChk;
        if (gxHeartbeatBuffer.bTryWrite(xHeartbeat)) {

          /*if (iCnt++ <= 1) { // debug print twice

            //vTrace("Received HB from Node %s\n"_Stz, xHeartbeat.m_acNodeName);
            //vTrace("iLen=%d m_u2DataLen=%d checksum=%lld \n"_Stz, iLen, xHeartbeat.m_u2DataLen,
                   iSaveChk);
          }*/
        }
      } else {
        vTrace("Invalid Heartbeat message, checksum is not equal, skip this message.\n"_Stz);
      }
    }
  }
}


/**
 * @brief heart read loop
 * @details reads heartbeat data from the inbound ringbuffer and updates the node heartbeat
 * <p>
 * @param[in] void * (unused)
 *
 * @return none
 *
 * @todo If the heartbeat comes from a new node, that is not exists in the grid
 *       we should add this node
 *       Will add the correct global version number
 */
[[noreturn]] void *vHeartReadLoop(void *pVoid)
{
  UNUSED(pVoid);
  Heartbeat xHeartbeat;

  for (;;) {

    MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);
    // Read from the inbound buffer
    if (gxHeartbeatBuffer.bTryRead(xHeartbeat)) {
      // update heartbeat info for the node

      if (xHeartbeat.m_iClusterID == gxCmdLineArgs.m_xSysArgs.m_iClusterID) {
        //vTrace("Try to update heartbeat from Node %s  %lld\n"_Stz, xHeartbeat.m_acNodeName, xHeartbeat.m_iClusterID);
        if (!AM::bPutHeartbeat((Stz *) xHeartbeat.m_acNodeName, xHeartbeat)) {
          // We failed to update the node with heartbeat,
          // meaning we don't have this node in the cluster/grid yet.
          // Then we need to add this node into the cluster/grid.
          if (xHeartbeat.m_u2Version >= AM::iVersionNumber()) {// will add the correct global version number
            AM::GRID::vNodeDetected(xHeartbeat.m_iNodeNo,
                                    (Stz *) xHeartbeat.m_acNodeName,
                                    (Stz *) xHeartbeat.m_acAddr,
                                    xHeartbeat.m_iPort,
                                    (Stz *) xHeartbeat.m_acRegionName);
            
            vTrace("Node %s has been added\n"_Stz, xHeartbeat.m_acNodeName);
          } else {
            vTrace("Invalid Heartbeat message version (%ld) is rejected. \n"_Stz, xHeartbeat.m_u2Version);
          }
        }
      }
    }

    // Nothing in the queue to read, sleep a little
    usleep(300*1000);
  }

  //return nullptr;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
