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
  vAddStep("tokenVec::new"_Stz,     &Step::vTokenVecNew);
  
  vAddStep("tokenVec.dump"_Stz,     &Step::vTokenVecDump);
  vAddStep("tokenVec.tokenise"_Stz, &Step::vTokenVecTokenise);
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
/* tokenVec.dump -- dump given tokenVec                                       */
/*----------------------------------------------------------------------------*/
void Step::vTokenVecDump(void)
{
  ENTRY();
  
  TokenVec* pTknVec = _pTokenVec(m_pDataStack->pPopPtr(__func__));

  pTknVec->vDumpVec(0);
    
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

/*----------------------------------------------------------------------------*/
/* tokenVec::tokenise -- tokenise given string                                */
/*----------------------------------------------------------------------------*/
void Step::vTokenVecTokenise(void)
{
  ENTRY();

  TokenVec* pTknVec = _pTokenVec(m_pDataStack->pPopPtr(__func__));
  
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  pTknVec->vTokenise(sString);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
