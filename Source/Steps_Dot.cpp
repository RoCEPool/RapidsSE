/*============================================================================*/
/* Steps_Dot.cpp - dot step group                                             */
/*============================================================================*/

#include "Steps.h"

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
void Step::vDot(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDot"_Stz);
  
  GenVar xVariable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xVariable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  vTrace("[%s %s] %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeName(),
         mpTask->m_pJob ? (Stz*)mpTask->szJobName() : "<no job>"_Stz,
         xVariable.szStz());

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Step::vDotPut(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDotPut"_Stz);
  
  GenVar xVariable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xVariable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  vTrace("%s"_Stz,xVariable.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .\n -- print new line                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDotNewline(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDotNewline"_Stz);
  
  vTrace("\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .verbose -- print top value on stack if arg.m_bVerbose                     */
/*----------------------------------------------------------------------------*/
void Step::vDotVerbose(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDotVerbose"_Stz);
  
  GenVar xVariable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xVariable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  if (AM::bVerbose()) {
    vTrace("[%p %s] %s\n"_Stz,
           *mpTask,
           mpTask->szJobName(),
           xVariable.szStz());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
