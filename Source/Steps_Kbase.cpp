/*============================================================================*/
/* Steps_Kbase.cpp -- kbase                                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Kbase(void)
{
  vAddStep("Kbase[]"_Stz,        &Step::vKbasePointer);
  
  vAddStep("Kbase[].delete"_Stz, &Step::vKbaseDelete);
  vAddStep("Kbase[].new"_Stz,    &Step::vKbaseNew);
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
/* Kbase[] -- return pointer to kbase                                         */
/*----------------------------------------------------------------------------*/
void Step::vKbasePointer(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  m_pDataStack->vPushPtr(pKbase);

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
