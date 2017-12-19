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
void Task::vAddPrimitives_Dot(void)
{
  vAddPrimitive("."_Stz,               &vDot,              false);
  vAddPrimitive(".put"_Stz,            &vDotPut,           false);
  vAddPrimitive(".\\n"_Stz,            &vDotNewline,       false);
  vAddPrimitive(".verbose"_Stz,     &vDotVerbose,       false);
}

/*----------------------------------------------------------------------------*/
/* . -- print top value on stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDot(MP_Task mpTask)
{
  ENTRY("vDot"_Stz);
  
  s48K sWorkspace;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("."_Stz);
  
  ValLib::vToString(xValWrk.pNestedValueSelf(),sWorkspace);
  /*
  vTrace("[N%lld:%8s\\%lld %-11s] %s\n"_Stz,
         
         gpAttachMap->m_pGrid->iNode(),
         mpTask->m_sName.aChar(),
         *mpTask->m_zAcks,
         mpTask->m_sComment.aChar(),
         
         sWorkspace.aChar()
         );
  */
  vTrace("[%8s\\%lld %-11s %p] %s\n"_Stz,
         
         mpTask->m_sName.aChar(),
         *mpTask->m_zAcks,
         mpTask->m_sComment.aChar(),
         mpTask,
         sWorkspace.aChar()
         );

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Task::vDotPut(MP_Task mpTask)
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
void Task::vDotNewline(MP_Task mpTask)
{
  ENTRY("vDotNewline"_Stz);
  
  vTrace("\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .verbose -- print top value on stack if arg.m_bVerbose                     */
/*----------------------------------------------------------------------------*/
void Task::vDotVerbose(MP_Task mpTask)
{
  ENTRY("vDotVerbose"_Stz);
  
  s48K sWorkspace;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK(".verbose"_Stz);
  
  if (gpAttachMap->m_bVerbose) {
    ValLib::vToString(xValWrk.pNestedValueSelf(),sWorkspace);
    vTrace("[%p %s] %s\n"_Stz,*mpTask,mpTask->m_sName.aChar(),sWorkspace.aChar());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
