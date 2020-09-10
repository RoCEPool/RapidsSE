/*============================================================================*/
/* Steps_TokenVec.cpp - token vector step group                               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TokenVec(void)
{
  vAddStep("tokenVec::delete"_Stz,  &Step::vTokenVecDelete);
  vAddStep("tokenVec::new"_Stz,     &Step::vTokenVecDelete);
}

/*----------------------------------------------------------------------------*/
/* tokenVec::delete -- delete an instance of tokenVec                         */
/*----------------------------------------------------------------------------*/
void Step::vTokenVecDelete(void)
{
  ENTRY();
  
  TokenVec* pTknVec = _pTokenVec(m_pDataStack->pPopPtr(__func__));

  delete pTknVec;
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tokenVec::new -- create a new instance of tokenVec                         */
/*----------------------------------------------------------------------------*/
void Step::vTokenVecNew(void)
{
  ENTRY();

  TokenVec* pTknVec = new TokenVec;
  
  m_pDataStack->vPushPtr(pTknVec);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
