/*============================================================================*/
/* Steps_Procedure.cpp - procedure step group                                 */
/*============================================================================*/

#include "Steps.h"

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
  
  s48K sProcText;
  m_pDataStack->vPopStr(sProcText,__func__);
  
  TokenVec* pTokenVec = TokenVec::pTokenise(sProcText);
  
  Procedure::vAddProcedure(pTokenVec);
  
  delete pTokenVec;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
