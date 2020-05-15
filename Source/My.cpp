/*============================================================================*/
/* My.cpp -- My helper                                                        */
/*============================================================================*/

#include "Oblique.h"

extern Args          gxArgs;
extern PromptCatalog gxPromptCatalog;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Log*          gpLog;
extern Header*       gaSeShmRegion;
extern File*         gpTraceFile;

extern SizeHeap      gxaSizeHeap[];


/*----------------------------------------------------------------------------*/
/* get current job name                                                       */
/*----------------------------------------------------------------------------*/
Stz* My::szJobNm(void)
{
  Job* pJob = AM::pThisTLSJob();
  
  return aCN[pJob->eClass()];
}

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
//Stz* My::szToken(void)
//{
//  Task* pTask = AM::pThisTLSTask();
//  
//  return pTask->pInput()->szToken();
//}

/*----------------------------------------------------------------------------*/
/* get current word name                                                      */
/*----------------------------------------------------------------------------*/
Stz* My::szWordNm(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  Word* pWord = pTask->pCompiler()->pCurrentWord();
  
  return pWord->szWordNm();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
