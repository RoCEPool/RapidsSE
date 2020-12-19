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
void Step::vDot(void)
{
  ENTRY();
  
  CellSingle xVariable;
  m_pDataStack->vPopVal(xVariable,__func__);
  
  if (xVariable.bIsIntVal()) {
//    vTrace("[%s %s] %lld\n"_Stz,
//           AM::pGrid()->pThisNode()->szNodeNm(),
//           m_pTask->m_pJob ? (Stz*)m_pTask->szJobName() : "<no job>"_Stz,
//           xVariable.iInt());
    
    m_pTask->m_pJob->vPrint("%lld"_Stz,xVariable.iGetColVal());
    
  }
  else {
//    vTrace("[%s %s] %s\n"_Stz,
//           AM::pGrid()->pThisNode()->szNodeNm(),
//           m_pTask->m_pJob ? (Stz*)m_pTask->szJobName() : "<no job>"_Stz,
//           xVariable.szStz());
    m_pTask->m_pJob->vPrint("%s"_Stz,xVariable.szStz());
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
//void Step::vDot(void)
//{
//  ENTRY();
//
//  CellSingle xVariable;
//  m_pDataStack->vPopVal(xVariable,__func__);
//
//  s32 sSide;
//
//  if (xVariable.bIsInt()) {
//    vTrace("[%s%s %s] %lld\n"_Stz,
//           AM::pGrid()->pThisNode()->szNodeNm(),
//           m_pTask->m_pJob->m_bWasReceived ? "(partpt)" : "(origin)",
//           m_pTask->m_pJob ? (Stz*)m_pTask->szJobName() : "<no job>"_Stz,
//
//           xVariable.iInt());
//  }
//  else {
//    vTrace("[%s%s %s] %s\n"_Stz,
//           AM::pGrid()->pThisNode()->szNodeNm(),
//           m_pTask->m_pJob->m_bWasReceived ? "(partpt)" : "(origin)",
//           m_pTask->m_pJob ? (Stz*)m_pTask->szJobName() : "<no job>"_Stz,
//
//           xVariable.szStz());
//  }
//
//  EXIT();
//}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Step::vDotPut(void)
{
  ENTRY();
  
  CellSingle xVariable;
  m_pDataStack->vPopVal(xVariable,__func__);
  
  if (xVariable.bIsIntVal())
    vTrace("%lld"_Stz,xVariable.iGetColVal());
  else
    vTrace("%s"_Stz,xVariable.szStz());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .\n -- print new line                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDotNewline(void)
{
  ENTRY();
  
  vTrace("\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .verbose -- print top value on stack if arg.m_bVerbose                     */
/*----------------------------------------------------------------------------*/
void Step::vDotVerbose(void)
{
  ENTRY();
  
  CellSingle xVariable;
  m_pDataStack->vPopVal(xVariable,__func__);
  
  if (AM::bVerbose()) {
    vTrace("[%p %s] %s\n"_Stz,
           m_pTask,
           m_pTask->szJobName(),
           xVariable.szStz());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
