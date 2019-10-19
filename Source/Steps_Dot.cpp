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
  if ( ! pForth->m_pDataStack->bPop(xVariable) )
    THROW_DSTACK();
  
  vTrace("[%s %s] %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeName(),
         pForth->m_pTask->m_pJob ? (Stz*)pForth->m_pTask->szJobName() : "<no job>"_Stz,
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
  if ( ! pForth->m_pDataStack->bPop(xVariable) )
    THROW_DSTACK();
  
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
  if ( ! pForth->m_pDataStack->bPop(xVariable) )
    THROW_DSTACK();
  
  if (AM::bVerbose()) {
    vTrace("[%p %s] %s\n"_Stz,
           *mpTask,
           pForth->m_pTask->szJobName(),
           xVariable.szStz());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
