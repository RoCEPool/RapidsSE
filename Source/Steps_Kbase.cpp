/*============================================================================*/
/* Steps_Kbase.cpp -- kbase                                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Kbase(void)
{
  vAddStep("Kbase[]"_Stz,        &Step::vKbaseSet);
  
  vAddStep("Kbase[].delete"_Stz, &Step::vKbaseDelete);
  vAddStep("Kbase[].get"_Stz,    &Step::vKbaseGet);
  vAddStep("Kbase[].new"_Stz,    &Step::vKbaseNew);
  vAddStep("Kbase[].set"_Stz,    &Step::vKbaseSet);
  vAddStep("Kbase[].test"_Stz,   &Step::vKbaseTest);
}

/*----------------------------------------------------------------------------*/
/* Kbase[].delete -- delete kbase                                             */
/*----------------------------------------------------------------------------*/
void Step::vKbaseDelete(void)
{
  ENTRY();
  
  Kbase* m_xpKbase = * m_pLogicUnit->m_xpKbase;
  m_pLogicUnit->m_xpKbase = nullptr;
  
  delete m_xpKbase;
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].get -- return pointer to kbase                                     */
/*----------------------------------------------------------------------------*/
void Step::vKbaseGet(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  m_pDataStack->vPushPtr(pKbase);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].new -- create a new kbase                                          */
/*----------------------------------------------------------------------------*/
void Step::vKbaseNew(void)
{
  ENTRY();
  
  s32 sKbaseNm;
  m_pDataStack->vPopStr(sKbaseNm,__func__);
  
  m_pLogicUnit->m_xpKbase = new Kbase(sKbaseNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].set -- set reg Kbase                                               */
/*----------------------------------------------------------------------------*/
void Step::vKbaseSet(void)
{
  ENTRY();
  
  Kbase* pKbase = _pKbase(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpKbase = pKbase;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].test -- run kbase test                                             */
/*----------------------------------------------------------------------------*/
void Step::vKbaseTest(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  pKbase->vTest();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
