/*============================================================================*/
/* Steps_Dot.cpp - dot step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// dot: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dot(void)
{
  vAddStep("."_Stz,            &Step::vDot);
  vAddStep(".put"_Stz,         &Step::vDotPut);
  vAddStep(".\\n"_Stz,         &Step::vDotNewline);
  vAddStep(".verbose"_Stz,     &Step::vDotVerbose);
}

/*----------------------------------------------------------------------------*/
/* . -- print top value on stack                                              */
/*----------------------------------------------------------------------------*/
void Step::vDot(MP_Task mpTask)
{
  ENTRY("vDot"_Stz);
  
  s48K sWorkspace;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("."_Stz);
  
  ValLib::vToString(xValWrk.pNestedValueSelf(),sWorkspace);

  vTrace("[%s %s] %s\n"_Stz,
         gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
         mpTask->m_pJob ? (Stz*)mpTask->aJobName() : "<no job>"_Stz,
         sWorkspace.aChar()
         );

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Step::vDotPut(MP_Task mpTask)
{
  ENTRY("vDotPut"_Stz);
  
  s48K sWorkspace;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK(".put"_Stz);
  
  ValLib::vToString(xValWrk.pNestedValueSelf(),sWorkspace);
  vTrace("%s"_Stz,sWorkspace.aChar());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .\n -- print new line                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDotNewline(MP_Task mpTask)
{
  ENTRY("vDotNewline"_Stz);
  
  vTrace("\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .verbose -- print top value on stack if arg.m_bVerbose                     */
/*----------------------------------------------------------------------------*/
void Step::vDotVerbose(MP_Task mpTask)
{
  ENTRY("vDotVerbose"_Stz);
  
  s48K sWorkspace;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK(".verbose"_Stz);
  
  if (gpAttachMap->m_bVerbose) {
    ValLib::vToString(xValWrk.pNestedValueSelf(),sWorkspace);
    vTrace("[%p %s] %s\n"_Stz,
           *mpTask,
           mpTask->aJobName(),
           sWorkspace.aChar());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
