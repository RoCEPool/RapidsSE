/*============================================================================*/
/* Steps_Dot.cpp - dot step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "TagConstants.h"
#include "HeadsUp.hpp"
#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dot(void)
{
  vAddStep("."_Stz,            &Step::vDot);
  vAddStep(".boot"_Stz,        &Step::vDotBoot);
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
  
//  HeadsUp oHeadsUp((Stz*)__func__);
//  MY::pTask()->vSeeFrameStack();
  
  CellSingle xVariable;
  m_pDataStack->vPopItem(xVariable,__func__);
  
  if (xVariable.bIsIntVal()) {
    i8 iVal = xVariable.iGetColVal();
    vDotPrint("%lld"_Stz,iVal);
  }
  else {
    Stz* szVal = xVariable.szStz();
    vDotPrint("%s"_Stz,szVal);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .boot -- print top value on stack                                          */
/*----------------------------------------------------------------------------*/
void Step::vDotBoot(void)
{
  ENTRY();
  
  CellSingle xVariable;
  m_pDataStack->vPopItem(xVariable,__func__);
  
  if (AM::SHOW::bBoot()) {

    if (xVariable.bIsIntVal()) {
      vDotPrint("%lld"_Stz,xVariable.iGetColVal());
    }
    else {
      vDotPrint("%s"_Stz,xVariable.szStz());
    }
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
//void Step::vDot(void)
//{
//  ENTRY();
//
//  CellSingle xVariable;
//  m_pDataStack->vPopItem(xVariable,__func__);
//
//  s32 sSide;
//
//  if (xVariable.bIsInt()) {
//    vTrace("[%s%s %s] %lld\n"_Stz,
//           AM::pGrid()->pThisNode()->szNodeNm(),
//           m_pJob->m_bWasReceived ? "(partpt)" : "(origin)",
//           m_pJob ? (Stz*)m_pTask->szJobName() : "<no job>"_Stz,
//
//           xVariable.iInt());
//  }
//  else {
//    vTrace("[%s%s %s] %s\n"_Stz,
//           AM::pGrid()->pThisNode()->szNodeNm(),
//           m_pJob->m_bWasReceived ? "(partpt)" : "(origin)",
//           m_pJob ? (Stz*)m_pTask->szJobName() : "<no job>"_Stz,
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
  m_pDataStack->vPopItem(xVariable,__func__);
  
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
  m_pDataStack->vPopItem(xVariable,__func__);
  
  if (AM::TRACE::bVerbose(C_VB_BOOT,C_VB_SEN)) {
    vTrace("[%p %s] %s\n"_Stz,
           this,
           szTaskNm(),
           xVariable.szStz());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
