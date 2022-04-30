/*============================================================================*/
/* Steps_Procedure.cpp - procedure step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Procedure.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Procedure(void)
{
  vAddStep("procedure::add"_Stz,  &Step::vProcedureAdd);
}

/*----------------------------------------------------------------------------*/
/* process::getPid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Step::vProcedureAdd(void)
{
  ENTRY();
  
  s48K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  s48K sCmdTagStr;
  m_pDataStack->vPopStr(sCmdTagStr,__func__);

  TokenVec* pTokenVec = TokenVec::pTokeniseSen(sCmd);
  
  Procedure::vAddProcedure(pTokenVec);
  
  delete pTokenVec;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
