/*============================================================================*/
/* Steps_TinyC.cpp -- TinyC step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TinyC(void)
{
  vAddStep("TinyC[].compile"_Stz,      &Step::vTinyCCompile);
  vAddStep("TinyC[].delete"_Stz,       &Step::vTinyCDelete);
  vAddStep("TinyC[].find"_Stz,         &Step::vTinyCFind);
  vAddStep("TinyC[].new"_Stz,          &Step::vTinyCNew);
  vAddStep("TinyC[].relocate"_Stz,     &Step::vTinyCRelocate);
  vAddStep("TinyC[].symbols"_Stz,      &Step::vTinyCSymbols);
}

/*----------------------------------------------------------------------------*/
/* TinyC[].compile -- compile given source using TinyC                        */
/*----------------------------------------------------------------------------*/
void Step::vTinyCCompile(void)
{
  ENTRY();

  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;
  
  s48K sSource;
  m_pDataStack->vPopStr(sSource,__func__);

  bool bClean = pTinyC->bCompile(sSource.azChar());
  if (bClean)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].delete -- delete TinyC                                             */
/*----------------------------------------------------------------------------*/
void Step::vTinyCDelete(void)
{
  ENTRY();
    
  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;
  m_p_TaskLogicUnit->m_xpTinyC = nullptr;
  
  pTinyC->vEnd();

  delete pTinyC;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].find -- find symbol                                                */
/*----------------------------------------------------------------------------*/
void Step::vTinyCFind(void)
{
  ENTRY();

  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;

  s32 sFunNm;
  m_pDataStack->vPopStr(sFunNm,__func__);

  bool (*afFun)(void*);
  afFun = (bool (*)(void*)) pTinyC->pFindSymbol(sFunNm.azChar());

  m_pDataStack->vPushInt((i8)afFun);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].new -- new TinyC                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTinyCNew(void)
{
  ENTRY();
  
  TinyC* pTinyC = new TinyC;
  m_p_TaskLogicUnit->m_xpTinyC = pTinyC;

  pTinyC->vBegin();

  pTinyC->vAcquire(pTinyC,TinyC::vTinyError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].relocate -- relocate                                               */
/*----------------------------------------------------------------------------*/
void Step::vTinyCRelocate(void)
{
  ENTRY();
   
  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;

  if (pTinyC->bRelocate())
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].symbols -- symbol set                                              */
/*----------------------------------------------------------------------------*/
void Step::vTinyCSymbols(void)
{
  ENTRY();
    
  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;

  i8 iSymSet = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
