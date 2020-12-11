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

/*----------------------------------------------------------------------------*/
/* heart initialise                                                           */
/*----------------------------------------------------------------------------*/
void vHeartInit(void)
{
  printf("SE Heart (sizeof(Heartbeat)=%lld\n",(i8)sizeof(Heartbeat));
}

/*----------------------------------------------------------------------------*/
/* heart initialise                                                           */
/*----------------------------------------------------------------------------*/
void vHeartThreadEntry(void)
{
  printf("SE Heart thread entry\n");
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
