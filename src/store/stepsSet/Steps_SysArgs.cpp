/*============================================================================*/
/* Steps_SysArgs.cpp - SysArgs (system arguments) step group                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SysArgs(void)
{
  vAddStep("sysargs::definedNodes"_Stz,  &Step::vSysArgsDefinedNodes);
  vAddStep("sysargs::print"_Stz,         &Step::vSysArgsPrint);
  vAddStep("sysargs::tagSet@"_Stz,       &Step::vSysArgsTagSet);
}

/*----------------------------------------------------------------------------*/
/* sysargs::definedNodes -- get number of nodes defined on command line       */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsDefinedNodes(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::SYSARGS::nDefinedNodes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::print -- print system arguments                                   */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsPrint(void)
{
  ENTRY();
  
  AM::vPrintStartupInfo();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sysargs::tagSet -- get sysargs tag set                                     */
/*----------------------------------------------------------------------------*/
void Step::vSysArgsTagSet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(AM::SYSARGS::pTagSet());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
