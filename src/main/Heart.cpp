/*============================================================================*/
/* Heart.cpp - system heart                                                   */
/*============================================================================*/



#include <cstdio>
#include <cstring>
#include <base/BASE.h>

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

  if (gxCmdLineArgs.m_xSysArgs.m_nHeartbeatNodes > 0) {
    vTrace("SE Heart (sizeof(Heartbeat)=%lld\n"_Stz, (i8) sizeof(Heartbeat));

    int iPort = gxCmdLineArgs.m_xSysArgs.m_xHeartbeatNode[0].m_iPortNo;
    char szAddr[32];

    if (iPort == 0) {
      iPort = 8888;
    }

    strncpy(szAddr, gxCmdLineArgs.m_xSysArgs.m_xHeartbeatNode[0].m_sIPAddress.azChar(), sizeof(szAddr) - 1);
    szAddr[sizeof(szAddr) - 1] = 0;

    //User can put on the command line argument as --heartbeat NODE0 BROADCAST 8888
    //strcpy(szAddr, "192.168.0.255"); // for DEBUG
    HeartbeatSnd *pSender = new HeartbeatSnd(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartSndLoop, pSender);

    strcpy(szAddr, "ANY"); // For receiving Broadcast, any interface is the option
    HeartbeatRcv *pReceiver = new HeartbeatRcv(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartRcvLoop, pReceiver);

    // Create Reader thread to test out the ringbuffer
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
    //MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);

    if (gxHeartbeatData.bTryRead(xHeartbeat)) {

      /*
      sprintf(azHeartbeat,

              "SE sent heartbeat from Node: %s seqNo %lld sinceEpoch %lld gitHash %s\n",
              xHeartbeat.m_acNodeName, // node number
              xHeartbeat.m_iSeqNo,
              xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash

      );*/

      pSender->iSend((char *) &xHeartbeat, sizeof(Heartbeat));

      if (iCnt++ <= 1) // debug only: print twice
        vTrace("Node: %s sent HB\n"_Stz, xHeartbeat.m_acNodeName);
    }


    if (iCnt >8) {
      //if (strcmp(xHeartbeat.m_acNodeName, "NODE1") == 0) {
      //  vTrace("Process NODE1 got killed!!!!!!!\n"_Stz);
        //Process::vExitSuccess();
        //printf("Supervisor got killed, no more sending, but monitor continues, works fine.\n");
        //AM::vSupervisorHalt(true);
      //}
    }

  }

  //return nullptr;
}

/*----------------------------------------------------------------------------*/
/* heart receive loop: gets a packet from network then writes to ringbuffer   */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartRcvLoop(void *pFuncObj)
{

  HeartbeatRcv *pReceiver = (HeartbeatRcv *) pFuncObj;

  Heartbeat xHeartbeat;
  struct tm *tmTimeInfo;

  char azTimeBuffer[80];
  char azHeartbeat[C_1KB];
  int iCnt = 0;
  MemLib::vZero(&azHeartbeat,sizeof(azHeartbeat),CN_NONE);
  sleep(5); // delay receiving 5 seconds when started

  for (;;) {

    //MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);

    int iLen = pReceiver->iRecv((char *) &xHeartbeat, sizeof(xHeartbeat));

    if (iLen > 0) {
      i8 iSaveChk = xHeartbeat.m_iChecksum;

      xHeartbeat.m_iChecksum = 0;
      //printf("fletcher16 mychecksum1=%d\n",fletcher16((unsigned char *) &xHeartbeat, sizeof(Heartbeat)));

      // recalculate checksum and compare
      CheckSum ChecksumValue((unsigned char *) &xHeartbeat, sizeof(Heartbeat));

      //if (ChecksumValue.uGetFletcher16() == iSaveChk) {
        xHeartbeat.m_iChecksum = iSaveChk;
        if (gxHeartbeatBuffer.bTryWrite(xHeartbeat)) {

          if (iCnt++ <= 1) { // debug print twice
            /*tmTimeInfo = localtime((const time_t *) &xHeartbeat.m_iSincePosixEpoch);
            strftime(azTimeBuffer, sizeof(azTimeBuffer), "%a %Y-%m-%d %H:%M:%S %Z", tmTimeInfo);
            sprintf(azHeartbeat,
                    "\nSE received heartbeat from Node Name:%s seqNo %lld sinceEpoch %lld gitHash %s at %s\n",

                    xHeartbeat.m_acNodeName, // name
                    xHeartbeat.m_iSeqNo,
                    xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash, azTimeBuffer
            );*/

            vTrace("Received HB from Node %s\n"_Stz, xHeartbeat.m_acNodeName);
            vTrace("iLen=%d m_u2DataLen=%d checksum=%lld \n"_Stz, iLen, xHeartbeat.m_u2DataLen,
                   iSaveChk);
          }
        }
      //} else {
      //  vTrace("Checksum is not equal, skip this heartbeat packet\n"_Stz);
      //}
    }
  }

  //return nullptr;
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

  Heartbeat xHeartbeat;

  int iCnt = 0;

  for (;;) {

    MemLib::vZero(&xHeartbeat,sizeof(xHeartbeat),CN_NODE);
    if (gxHeartbeatBuffer.bTryRead(xHeartbeat)) {
      // update heartbeat info for the node
      //vTrace("Try to update heartbeat from Node %s\n"_Stz, xHeartbeat.m_acNodeName);
      if (!AM::bPutHeartbeat((Stz *) xHeartbeat.m_acNodeName, xHeartbeat)) {
        // We failed to update the node with heartbeat,
        // meaning we don't have this node in the cluster/grid yet.
        // Then we need to add this node into the cluster/grid.
        if (xHeartbeat.m_u2Version >= AM::iVersionNumber()) {// will add the correct global version number
          AM::vAddNewNode(xHeartbeat.m_iNodeNo, (Stz *) xHeartbeat.m_acNodeName, (Stz *) xHeartbeat.m_acAddr,
                          xHeartbeat.m_iPort);
          vTrace("Node %s has been added\n"_Stz, xHeartbeat.m_acNodeName);
        } else {
          vTrace("Heartbeat message came from an older version of SE %ld"_Stz, xHeartbeat.m_u2Version);
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
