/*============================================================================*/
/* Heart.cpp - system heart                                                   */
/*============================================================================*/



#include <cstdio>
#include <cstring>
#include <base/BASE.h>

#include "STORE.h"

#include "Heart.h"
#include "Heartbeat.hpp"


extern CmdLineArgs gxCmdLineArgs;

extern Stz *gsBuildGitHash;
extern Stz *gsBuildDate;
extern Stz *gsBuildHostName;
extern Stz *gsBuildUserName;
extern Stz *gsBuildBranch;

FILE *gaHearbeatFile = nullptr;

// global
RingBuffer<Heartbeat> gxHeartbeatBuffer(1000); // receiving from network
RingBuffer<Heartbeat> gxHeartbeatData(100); // collect heartbeat data from the system to be sent out


/*----------------------------------------------------------------------------*/
/* heart initialise                                                           */
/*----------------------------------------------------------------------------*/
void vHeartInit(void) {

  if (gxCmdLineArgs.m_xSysArgs.m_nHeartbeatNodes > 0) {
    printf("SE Heart (sizeof(Heartbeat)=%lld\n", (i8) sizeof(Heartbeat));

//  char azPath[256];
//  sprintf(azPath,"../tmp/rapidsse_pid%lld",(i8)0); //getpid());
//  gaHearbeatFile = fopen(azPath,"w");

    int iPort = gxCmdLineArgs.m_xSysArgs.m_xHeartbeatNode[0].m_iPortNo;
    char szAddr[32];

    if (iPort == 0) {
      iPort = 8888;
    }

    strcpy(szAddr, "ANY"); // For receiving Broadcast, any interface is the option
    HeartbeatRcv *pReceiver = new HeartbeatRcv(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartRcvLoop, pReceiver);


    strncpy(szAddr, gxCmdLineArgs.m_xSysArgs.m_xHeartbeatNode[0].m_sIPAddress.azChar(), sizeof(szAddr) - 1);
    szAddr[sizeof(szAddr) - 1] = 0;

    //User can put on the command line argument as --heartbeat NODE0 BROADCAST 8888

    //strcpy(szAddr, "192.168.0.255"); // for DEBUG
    HeartbeatSnd *pSender = new HeartbeatSnd(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartSndLoop, pSender);

    // Create Reader thread to test out the ringbuffer
    PsxThd::iSpawnPosixThread(vHeartReadLoop, nullptr);


  } else {
    printf("SE Heartbeat is not set for this node\n");
  }

}




/*----------------------------------------------------------------------------*/
/* heart send loop: pick up a packet from ringbuffer and send                 */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartSndLoop(void *pFuncObj) {
  printf("SE Heart Send thread entry\n");
  HeartbeatSnd *pSender = (HeartbeatSnd *) pFuncObj;

  Heartbeat xHeartbeat;
  char azHeartbeat[C_4K];
  int iCnt = 0;

  for (;;) {

    memset((char *) &xHeartbeat, 0x00, sizeof(Heartbeat));

    if (gxHeartbeatData.bTryRead(xHeartbeat)) {

      sprintf(azHeartbeat,

              "SE sent heartbeat from Node: %s seqNo %lld sinceEpoch %lld gitHash %s\n",
              xHeartbeat.m_acNodeName, // node number
              xHeartbeat.m_iSeqNo,
              xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash

      );

      pSender->iSend((char *) &xHeartbeat, sizeof(Heartbeat));

      if (iCnt++ <= 1) // debug only: print twice
        printf("%s", azHeartbeat);
    }

    sleep(3);
  }

  //return nullptr;
}

/*----------------------------------------------------------------------------*/
/* heart receive loop: gets a packet from network then writes to ringbuffer   */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartRcvLoop(void *pFuncObj) {
  printf("SE Heart Receiver thread entry\n");
  HeartbeatRcv *pReceiver = (HeartbeatRcv *) pFuncObj;

  Heartbeat xHeartbeat;
  struct tm *tmTimeInfo;

  char azTimeBuffer[80];
  char azHeartbeat[C_16KB];
  int iCnt = 0;

  for (;;) {

    memset(&xHeartbeat, 0x00, sizeof(Heartbeat));

    int iLen = pReceiver->iRecv((char *) &xHeartbeat, sizeof(xHeartbeat));

    if (iLen > 0) {
      i8 iSaveChk = xHeartbeat.m_iChecksum;

      xHeartbeat.m_iChecksum = 0;
      //printf("fletcher16 mychecksum1=%d\n",fletcher16((unsigned char *) &xHeartbeat, sizeof(Heartbeat)));

      if (fletcher16((unsigned char *) &xHeartbeat, sizeof(Heartbeat)) == iSaveChk) {
        xHeartbeat.m_iChecksum = iSaveChk;
        if (gxHeartbeatBuffer.bTryWrite(xHeartbeat)) {

          tmTimeInfo = localtime((const time_t *) &xHeartbeat.m_iSincePosixEpoch);
          strftime(azTimeBuffer, sizeof(azTimeBuffer), "%a %Y-%m-%d %H:%M:%S %Z", tmTimeInfo);


          sprintf(azHeartbeat,
                  "\n%lld: SE received heartbeat from Node Name:%s seqNo %lld sinceEpoch %lld gitHash %s at %s\n",
                  gxHeartbeatBuffer.iWriteIdx(),
                  xHeartbeat.m_acNodeName, // name
                  xHeartbeat.m_iSeqNo,
                  xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash, azTimeBuffer
          );

          if (iCnt++ <= 1) { // debug print twice
            printf("%s", azHeartbeat);
            printf("iLen=%ld m_u2DataLen=%d checksum=%d \n", iLen, xHeartbeat.m_u2DataLen,
                   xHeartbeat.m_iChecksum);
          }
        }
      } else {
        vTrace("Checksum is not equal, skip this heartbeat packet\n"_Stz);
      }
    }
  }

  //return nullptr;
}




/*---------------------------------------------------------------------------------*/
/* heart read loop: to simulate the reading, but doesn't do anything with the data */
/*---------------------------------------------------------------------------------*/
[[noreturn]] void *vHeartReadLoop(void *pFuncObj) {

  printf("SE Heart Read from received ring-buffer thread entry\n");

  Heartbeat xHeartbeat;
  struct tm *tmTimeInfo;

  char azTimeBuffer[80];
  char azHeartbeat[C_16KB];
  int iCnt = 0;

  for (;;) {

    memset(&xHeartbeat, 0x00, sizeof(Heartbeat));

    if (gxHeartbeatBuffer.bTryRead(xHeartbeat))  {

      // update heartbeat info for the node
      // m_acString0 is the node name
      AM::vPutHeartbeat((Stz*)xHeartbeat.m_acNodeName,xHeartbeat);

      tmTimeInfo = localtime((const time_t *) &xHeartbeat.m_iSincePosixEpoch);
      strftime(azTimeBuffer, sizeof(azTimeBuffer), "%a %Y-%m-%d %H:%M:%S %Z", tmTimeInfo);

      sprintf(azHeartbeat,

              "\n%lld SE read from received heartbeat from Node Name:%s seqNo %lld sinceEpoch %lld gitHash %s at %s\n",
              gxHeartbeatBuffer.iReadIdx(),
              xHeartbeat.m_acNodeName, // name
              xHeartbeat.m_iSeqNo,
              xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash, azTimeBuffer
      );

      if (iCnt++ <= 1) // debug print twice
        printf("%s",azHeartbeat);



      // DEBUG get back the heartbeat
      Heartbeat xxHeartbeat = AM::xGetHeartbeat((Stz*)xHeartbeat.m_acNodeName);

      sprintf(azHeartbeat,

              "SE after update node info for Node Name:%s seqNo %lld sinceEpoch %lld gitHash %s at %s\n",

              xxHeartbeat.m_acNodeName, // name
              xxHeartbeat.m_iSeqNo,
              xxHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash, azTimeBuffer
      );

      if (iCnt++ < 5) // debug print twice
        printf("%s",azHeartbeat);

      usleep(100*1000); // short sleep
    } else {
      // Nothing in the queue to read, sleep a little
      usleep(500*1000);
    }
  }

  //return nullptr;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
