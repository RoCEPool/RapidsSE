/*============================================================================*/
/* Heart.cpp - system heart                                                   */
/*============================================================================*/

#if 1 // ray 7 jan 2020; compilation problem in bamboo re atomics

#include <cstdio>
#include <cstring>
#include <base/BASE.h>

#include "STORE.h"

#include "Heart.h"
#include "Heartbeat.hpp"


extern CmdLineArgs gxArgs;

extern Stz *gsBuildGitHash;
extern Stz *gsBuildDate;
extern Stz *gsBuildHostName;
extern Stz *gsBuildUserName;
extern Stz *gsBuildBranch;

FILE *gaHearbeatFile = nullptr;

// global
RingBuffer<Heartbeat> gxHeartbeatBuffer(100); // receiving from network
RingBuffer<Heartbeat> gxHeartbeatData(100); // collect heartbeat data from the system to be sent out


/*----------------------------------------------------------------------------*/
/* heart initialise                                                           */
/*----------------------------------------------------------------------------*/
void vHeartInit(void) {

  if (gxArgs.m_xSysArgs.m_nHeartbeatNodes > 0) {
    printf("SE Heart (sizeof(Heartbeat)=%lld\n", (i8) sizeof(Heartbeat));

//  char azPath[256];
//  sprintf(azPath,"../tmp/rapidsse_pid%lld",(i8)0); //getpid());
//  gaHearbeatFile = fopen(azPath,"w");

    int iPort = gxArgs.m_xSysArgs.m_xHeartbeatNode[0].m_iPortNo;
    char szAddr[32];

    if (iPort == 0) {
      iPort = 8888;
    }

    strcpy(szAddr, "ANY"); // For receiving Broadcast, any interface is the option
    HeartbeatRcv *pReceiver = new HeartbeatRcv(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartRcvLoop, pReceiver);


    strncpy(szAddr, gxArgs.m_xSysArgs.m_xHeartbeatNode[0].m_sIPAddress.azChar(), sizeof(szAddr) - 1);
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


/*--------------------------------------------------------------------------------*/
/* This function to collect heartbeat data and write to ringbuffer to be sent out */
/*--------------------------------------------------------------------------------*/
void vCollectData()
{
  Heartbeat xHeartbeat;
  static i8 iSeq = 0;

  memset((char *) &xHeartbeat, 0x00, sizeof(Heartbeat));
  xHeartbeat.m_iSeqNo = ++iSeq;
  xHeartbeat.m_iSincePosixEpoch = time(0);
  strncpy(xHeartbeat.m_acBuildGitHash, (char *) gsBuildGitHash, 32);

  strncpy(xHeartbeat.m_acString0, gxArgs.m_xSysArgs.m_sNodeName.azChar(), sizeof(xHeartbeat.m_acString0) - 1);
  xHeartbeat.m_acString0[sizeof(xHeartbeat.m_acString0) - 1] = 0;

  gxHeartbeatData.bTryWrite(xHeartbeat);
}

/*----------------------------------------------------------------------------*/
/* heart send loop                                                            */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartSndLoop(void *pFuncObj) {
  printf("SE Heart Send thread entry\n");
  HeartbeatSnd *pSender = (HeartbeatSnd *) pFuncObj;

  Heartbeat xHeartbeat;
  char azHeartbeat[C_4K];
  int iCnt = 0;

  for (;;) {

    vCollectData();

    memset((char *) &xHeartbeat, 0x00, sizeof(Heartbeat));

    gxHeartbeatData.bTryRead(xHeartbeat);

    sprintf(azHeartbeat,

            "SE sent heartbeat: seqNo %lld sinceEpoch %lld gitHash %s\n", xHeartbeat.m_iSeqNo,
            xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash

    );

      //fseeko(gaHearbeatFile,0,SEEK_SET);
      //fwrite(azHeartbeat, 1, strlen(azHeartbeat)+1,gaHearbeatFile);
      //fflush(gaHearbeatFile);


    pSender->iSend((char *) &xHeartbeat, sizeof(Heartbeat));

    if (iCnt++ <= 1) // debug only: print twice
      printf("%s",azHeartbeat);

    sleep(3);
  }

  //return nullptr;
}

/*----------------------------------------------------------------------------*/
/* heart receive loop                                                         */
/*----------------------------------------------------------------------------*/
[[noreturn]] void *vHeartRcvLoop(void *pFuncObj) {
  printf("SE Heart Receiver thread entry\n");
  HeartbeatRcv *pReceiver = (HeartbeatRcv *) pFuncObj;

  Heartbeat hbHeartbeat;
  struct tm *tmTimeInfo;

  char azTimeBuffer[80];
  char azHeartbeat[C_16KB];
  int iCnt = 0;

  for (;;) {

    memset(&hbHeartbeat, 0x00, sizeof(Heartbeat));

    int iLen = pReceiver->iRecv((char *) &hbHeartbeat, sizeof(hbHeartbeat));

    if (iLen > 0) {
      if (gxHeartbeatBuffer.bTryWrite(hbHeartbeat)) {

            tmTimeInfo = localtime((const time_t *) &hbHeartbeat.m_iSincePosixEpoch);
            strftime(azTimeBuffer, sizeof(azTimeBuffer), "%a %Y-%m-%d %H:%M:%S %Z", tmTimeInfo);

            sprintf(azHeartbeat,

            "%ld: SE received heartbeat: seqNo %lld sinceEpoch %lld gitHash %s at %s\n",
            gxHeartbeatBuffer.iWriteIdx(), hbHeartbeat.m_iSeqNo,
            hbHeartbeat.m_iSincePosixEpoch, hbHeartbeat.m_acBuildGitHash, azTimeBuffer
            );

            if (iCnt++ <= 1) // debug print twice
            printf("%s", azHeartbeat);
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

  Heartbeat hbHeartbeat;
  struct tm *tmTimeInfo;

  char azTimeBuffer[80];
  char azHeartbeat[C_16KB];
  int iCnt = 0;

  for (;;) {

    memset(&hbHeartbeat, 0x00, sizeof(Heartbeat));

    usleep(500*1000);

    if (gxHeartbeatBuffer.bTryRead(hbHeartbeat))  {

      tmTimeInfo = localtime((const time_t *) &hbHeartbeat.m_iSincePosixEpoch);
      strftime(azTimeBuffer, sizeof(azTimeBuffer), "%a %Y-%m-%d %H:%M:%S %Z", tmTimeInfo);

      sprintf(azHeartbeat,

              "%ld SE read from received heartbeat: seqNo %lld sinceEpoch %lld gitHash %s at %s\n",
              gxHeartbeatBuffer.iReadIdx(),
              hbHeartbeat.m_iSeqNo,
              hbHeartbeat.m_iSincePosixEpoch, hbHeartbeat.m_acBuildGitHash, azTimeBuffer
      );

      if (iCnt++ <= 1) // debug print twice
        printf("%s",azHeartbeat);
    }
  }

  //return nullptr;
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
