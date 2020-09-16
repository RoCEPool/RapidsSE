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
  vAddStep("Fact[].newAtr"_Stz, &Step::vFactNewAtr);
  vAddStep("Fact[].newSet"_Stz, &Step::vFactNewSet);
  vAddStep("Fact[].newTpl"_Stz, &Step::vFactNewTpl);
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
/* Fact[].newAtr -- create a new attribute fact                               */
/*----------------------------------------------------------------------------*/
void Step::vFactNewAtr(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].newSet -- create a new set fact                                     */
/*----------------------------------------------------------------------------*/
void Step::vFactNewSet(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].newTpl -- create a new tuple fact                                   */
/*----------------------------------------------------------------------------*/
void Step::vFactNewTpl(void)
{
  ENTRY();
    
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
