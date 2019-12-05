/*============================================================================*/
/* My.cpp -- My helper                                                        */
/*============================================================================*/

#include "Oblique.h"

extern Arguments     gxArguments;
extern OpCatalog     gxOpCatalog;
extern PromptCatalog gxPromptCatalog;

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Log*          gpLog;
extern Header*       gaSeShmRegion;
extern File*         gpTraceFile;

extern eHT           geWhichHeap[];
extern i8            giClassSize[];
extern pFINALIZE*    gpFinalize[];


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
Stz* My::szToken(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  return pTask->pCompiler()->m_pTokeniser->szToken();
}

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
