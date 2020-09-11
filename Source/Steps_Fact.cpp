/*============================================================================*/
/* Steps_Fact.cpp -- fact                                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Fact(void)
{
  vAddStep("Fact[]"_Stz,        &Step::vFactGet);
  
  vAddStep("Fact[].delete"_Stz, &Step::vFactDelete);
  vAddStep("Fact[].get"_Stz,    &Step::vFactGet);
  vAddStep("Fact[].new"_Stz,    &Step::vFactNew);
  vAddStep("Fact[].set"_Stz,    &Step::vFactSet);
}

/*----------------------------------------------------------------------------*/
/* Fact[].delete -- delete fact                                               */
/*----------------------------------------------------------------------------*/
void Step::vFactDelete(void)
{
  ENTRY();
  
  Fact* pFact = * m_pLogicUnit->m_xpFact;
  m_pLogicUnit->m_xpFact = nullptr;
  
  delete pFact;
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[] -- get pointer to fact                                              */
/*----------------------------------------------------------------------------*/
void Step::vFactGet(void)
{
  ENTRY();
  
  Fact* pFact = * m_pLogicUnit->m_xpFact;

  m_pDataStack->vPushPtr(pFact);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].new -- create a new fact                                            */
/*----------------------------------------------------------------------------*/
void Step::vFactNew(void)
{
  ENTRY();
  
//  s32 sKbaseNm;
//  m_pDataStack->vPopStr(sKbaseNm,__func__);
//
//  m_pLogicUnit->m_xpKbase = new Kbase(sKbaseNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* fact[].set -- set reg fact                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFactSet(void)
{
  ENTRY();
  
  Fact* pFact = _pFact(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpFact = pFact;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
