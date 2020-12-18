/*============================================================================*/
/* Heart.cpp - system heart                                                   */
/*============================================================================*/

#include <cstdio>
#include <cstring>
#include <Resources.h>
#include <Args.hpp>
#include "StorageEngine.h"
#include "Heart.h"
#include "Heartbeat.hpp"
#include "PsxThd.hpp"  //?


extern Args gxArgs;

extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

FILE*  gaHearbeatFile = nullptr;

Heartbeat gxHeartbeat;

/*----------------------------------------------------------------------------*/
/* heart initialise                                                           */
/*----------------------------------------------------------------------------*/
void vHeartInit(void) {

  if (gxArgs.m_nHeartbeatNodes > 0) {
    printf("SE Heart (sizeof(Heartbeat)=%lld\n", (i8) sizeof(Heartbeat));

    memset((char*)&gxHeartbeat, 0x00, sizeof(Heartbeat));
    strncpy(gxHeartbeat.m_acBuildGitHash, (char *) gsBuildGitHash, 32);

    strncpy(gxHeartbeat.m_acString0, gxArgs.m_sNodeName.azChar(), sizeof(gxHeartbeat.m_acString0)-1);
    gxHeartbeat.m_acString0[sizeof(gxHeartbeat.m_acString0)-1] = 0;

//  char azPath[256];
//  sprintf(azPath,"../tmp/rapidsse_pid%lld",(i8)0); //getpid());
//  gaHearbeatFile = fopen(azPath,"w");

    int iPort = gxArgs.m_xHeartbeatNode[0].m_iPortNo;
    char szAddr[32];

    if (iPort == 0) {
      iPort = 8888;
    }

    strcpy(szAddr, "ANY"); // For receiving Broadcast, any interface is the option
    HeartbeatRcv *pReceiver = new HeartbeatRcv(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartRcvLoop, pReceiver);

    strncpy(szAddr, gxArgs.m_xHeartbeatNode[0].m_sIPAddress.azChar(), sizeof(szAddr)-1);
    szAddr[sizeof(szAddr)-1] = 0;

    //strcpy(szAddr, "192.168.0.255"); // Send to broadcast address
    //User can put on the command line argument as --heartbeat BROADCAST 8888
    HeartbeatSnd *pSender = new HeartbeatSnd(szAddr, iPort);
    PsxThd::iSpawnPosixThread(vHeartSndLoop, pSender);

  } else {
    printf("SE Heartbeat is not set for this node\n");
  }

}

/*----------------------------------------------------------------------------*/
/* heart send loop                                                            */
/*----------------------------------------------------------------------------*/
void* vHeartSndLoop(void* pFuncObj)
{
  printf("SE Heart Send thread entry\n");
  HeartbeatSnd* pSender = (HeartbeatSnd*)pFuncObj;
  
  char azHeartbeat[C_4K];
  int iCnt = 0;

  for (;;) {
    
    gxHeartbeat.m_iSeqNo++;
    gxHeartbeat.m_iSincePosixEpoch = time(0);

    sprintf(azHeartbeat,
            
            "SE sent heartbeat: seqNo %lld sinceEpoch %lld gitHash %s\n"
            
            ,gxHeartbeat.m_iSeqNo
            ,gxHeartbeat.m_iSincePosixEpoch
            ,gxHeartbeat.m_acBuildGitHash

            );
    
    //fseeko(gaHearbeatFile,0,SEEK_SET);
//    fwrite(azHeartbeat, 1, strlen(azHeartbeat)+1,gaHearbeatFile);
//    fflush(gaHearbeatFile);
    pSender->iSend((char*)&gxHeartbeat, sizeof(Heartbeat));

    if (iCnt++ == 0) // debug only: print once
      printf(azHeartbeat);

    sleep(3);
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* heart receive loop                                                         */
/*----------------------------------------------------------------------------*/
void* vHeartRcvLoop(void* pFuncObj)
{
  printf("SE Heart Receiver thread entry\n");
  HeartbeatRcv* pReceiver = (HeartbeatRcv*)pFuncObj;

  Heartbeat xHeartbeat;
  struct tm * tmTimeInfo;

  char buffer[80];
  char azHeartbeat[C_16KB];
  int iCnt = 0;

  for (;;) {

    memset(&xHeartbeat,0x00,sizeof(Heartbeat));

    int iLen = pReceiver->iRecv((char*)&xHeartbeat, sizeof(xHeartbeat));

    if (iLen > 0) {
      tmTimeInfo = localtime((const time_t *) &xHeartbeat.m_iSincePosixEpoch);
      strftime(buffer, sizeof(buffer), "%a %Y-%m-%d %H:%M:%S %Z", tmTimeInfo);

      sprintf(azHeartbeat,

              "SE received heartbeat: seqNo %lld sinceEpoch %lld gitHash %s at %s\n", xHeartbeat.m_iSeqNo,
              xHeartbeat.m_iSincePosixEpoch, xHeartbeat.m_acBuildGitHash, buffer
      );

      if (iCnt++ == 0) // debug print once
        printf(azHeartbeat);
    }
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
