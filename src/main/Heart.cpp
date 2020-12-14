/*============================================================================*/
/* Heart.cpp - system heart                                                   */
/*============================================================================*/
  
#include "StorageEngine.h"
#include "Heart.h"
#include "Heartbeat.hpp"

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
void vHeartInit(void)
{
  printf("SE Heart (sizeof(Heartbeat)=%lld\n",(i8)sizeof(Heartbeat));
  
  memset(&gxHeartbeat,0x00,sizeof(Heartbeat));
  strncpy(gxHeartbeat.m_acBuildGitHash,(char*)gsBuildGitHash,32);
  
//  char azPath[256];
//  sprintf(azPath,"../tmp/rapidsse_pid%lld",(i8)0); //getpid());
//  gaHearbeatFile = fopen(azPath,"w");
  
  PsxThd::iSpawnPosixThread(vHeartLoop,nullptr);
}

/*----------------------------------------------------------------------------*/
/* heart loop                                                                 */
/*----------------------------------------------------------------------------*/
void* vHeartLoop(void*)
{
  printf("SE Heart thread entry\n");
  
  char azHeartbeat[C_16KB];

  for (;;) {
    
    gxHeartbeat.m_iSeqNo++;
    gxHeartbeat.m_iSincePosixEpoch = time(0);

    sprintf(azHeartbeat,
            
            "SE heartbeat: seqNo %lld sinceEpoch %lld gitHash %s\n"
            
            ,gxHeartbeat.m_iSeqNo
            ,gxHeartbeat.m_iSincePosixEpoch
            ,gxHeartbeat.m_acBuildGitHash

            );
    
    //fseeko(gaHearbeatFile,0,SEEK_SET);
//    fwrite(azHeartbeat, 1, strlen(azHeartbeat)+1,gaHearbeatFile);
//    fflush(gaHearbeatFile);

    sleep(3);
  }
  
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
