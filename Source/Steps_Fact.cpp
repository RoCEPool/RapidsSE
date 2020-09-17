/*============================================================================*/
/* Steps_Fact.cpp -- fact                                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Fact(void)
{
  vAddStep("Fact[]"_Stz,                &Step::vFactGet);
  
  vAddStep("Fact[].atrOf"_Stz,          &Step::vFactAtrOf);
  vAddStep("Fact[].attributeAppend"_Stz,&Step::vFactAttributeAppend);
  vAddStep("Fact[].delete"_Stz,         &Step::vFactDelete);
  vAddStep("Fact[].elementAppend"_Stz,  &Step::vFactElementAppend);
  vAddStep("Fact[].elmOf"_Stz,          &Step::vFactElmOf);
  vAddStep("Fact[].get"_Stz,            &Step::vFactGet);
  vAddStep("Fact[].new"_Stz,            &Step::vFactNew);
  vAddStep("Fact[].newAtr"_Stz,         &Step::vFactNewAtr);
  vAddStep("Fact[].newSet"_Stz,         &Step::vFactNewSet);
  vAddStep("Fact[].newTpl"_Stz,         &Step::vFactNewTpl);
  vAddStep("Fact[].set"_Stz,            &Step::vFactSet);
  vAddStep("Fact[].setInt"_Stz,         &Step::vFactSetInt);
  vAddStep("Fact[].setText"_Stz,        &Step::vFactSetText);
  vAddStep("Fact[].subOf"_Stz,          &Step::vFactSubOf);
}

/*----------------------------------------------------------------------------*/
/* Fact[].atrOf -- set current fact to be attribute of                        */
/*----------------------------------------------------------------------------*/
void Step::vFactAtrOf(void)
{
  ENTRY();
    
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);
  
  i8 iFactNo = m_pDataStack->iPopInt(__func__);
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].attributeAppend --                      */
/*----------------------------------------------------------------------------*/
void Step::vFactAttributeAppend(void)
{
  ENTRY();
  
  Fact* pAtrOf = _pFact(m_pDataStack->pPopPtr(__func__));

  Fact* pFact = * m_pLogicUnit->m_xpFact;

  pAtrOf->vAttributeAppend(pFact);
  
  EXIT();
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
/* Fact[].elementAppend --                      */
/*----------------------------------------------------------------------------*/
void Step::vFactElementAppend(void)
{
  ENTRY();
  
  Fact* pElmOf = _pFact(m_pDataStack->pPopPtr(__func__));

  Fact* pFact = * m_pLogicUnit->m_xpFact;

  pElmOf->vElementAppend(pFact);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].elmOf -- set current fact to be element of                          */
/*----------------------------------------------------------------------------*/
void Step::vFactElmOf(void)
{
  ENTRY();
    
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);
  
  i8 iFactNo = m_pDataStack->iPopInt(__func__);
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

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
    
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);
  
  //
  //  m_pLogicUnit->m_xpKbase = new Kbase(sKbaseNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].newSet -- create a new set fact                                     */
/*----------------------------------------------------------------------------*/
void Step::vFactNewSet(void)
{
  ENTRY();

  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);
  
  i8 iFactNo = m_pDataStack->iPopInt(__func__);
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  m_pLogicUnit->m_xpFact = new Fact(pKbase,iFactNo,sFactNm);

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

/*----------------------------------------------------------------------------*/
/* fact[].setInt -- set reg fact                                              */
/*----------------------------------------------------------------------------*/
void Step::vFactSetInt(void)
{
  ENTRY();
  
  Fact* pFact = _pFact(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpFact = pFact;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* fact[].setText -- set reg fact                                             */
/*----------------------------------------------------------------------------*/
void Step::vFactSetText(void)
{
  ENTRY();
  
  Fact* pFact = _pFact(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpFact = pFact;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fact[].subOf -- set current fact to be subclass of                         */
/*----------------------------------------------------------------------------*/
void Step::vFactSubOf(void)
{
  ENTRY();
  
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);
  
  i8 iFactNo = m_pDataStack->iPopInt(__func__);
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
