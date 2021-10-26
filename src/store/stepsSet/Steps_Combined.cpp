/*============================================================================*/
/* Steps_Tub.cpp - Working tub of buffers                                     */
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
void Step::vAddSteps_Combined(void)
{
  vAddStep("Combined[]"_Stz,               &Step::vCombinedGet);
  vAddStep("Combined[].delete"_Stz,        &Step::vCombinedDelete);
  vAddStep("Combined[].execute"_Stz,       &Step::vCombinedExecute);
  vAddStep("Combined[].new"_Stz,           &Step::vCombinedNew);
}

/*----------------------------------------------------------------------------*/
/* Combined[].delete -- delete the Combined                                   */
/*----------------------------------------------------------------------------*/
void Step::vCombinedDelete(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpCombined.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].get -- get the Combined                                         */
/*----------------------------------------------------------------------------*/
void Step::vCombinedGet(void)
{
  ENTRY();
  
  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  
  m_pDataStack->vPushPtr(pCombined);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].Execute -- execute combined code                                */
/*----------------------------------------------------------------------------*/
void Step::vCombinedExecute(void)
{
  ENTRY();
  
  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  Combined_T* aCombinedT = pCombined->aCombinedT();
    
  
  i8 iTrace = m_pDataStack->iPopInt(__func__);

  bool (*afFun)(void*);
  afFun = (bool (*)(void*))m_pDataStack->iPopInt(__func__);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //pTub->vInCrwBuffer(pCrwBuffer);
  
  //RowPix* pRowPix = pCrwBuffer->pStreamDef()->pRowPix();
  //pTub->vRowPix(pRowPix);

  aCombinedT->m_pTub       = pTub;
  if (pCrwBuffer) {
    aCombinedT->m_pRowSingle = new RowSingle(pCrwBuffer->pStreamDef());
    aCombinedT->m_aHeadLine0 = (HeadLine_T *) pCrwBuffer->aGetHead(0);
    aCombinedT->m_iHeadSize  = pCrwBuffer->nHeadSize();
    aCombinedT->m_nRows      = pCrwBuffer->nRows();
    aCombinedT->m_nCols      = pCrwBuffer->pStreamDef()->nColumns();
  }
  else {
    aCombinedT->m_pRowSingle = nullptr;
    aCombinedT->m_aHeadLine0 = nullptr;
    aCombinedT->m_iHeadSize  = C_UNKNOWN;
    aCombinedT->m_nRows      = 0;
    aCombinedT->m_nCols      = 0;
  }

  aCombinedT->m_eStrInflateRule = SI_NONE;
  aCombinedT->m_pEntWrk    = (void*)&m_p_TaskLogicUnit->m_oEntWrk;
  aCombinedT->m_aEntInt    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrk.aEnt();
  aCombinedT->m_pEntKey    = ((EntWrk*)aCombinedT->m_pEntWrk)->aKey();
  aCombinedT->m_bTrace     = iTrace;

  i8 iResult = afFun(aCombinedT);

  pTub->vInCrwBuffer(nullptr);

  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].new -- new an Combined                                          */
/*----------------------------------------------------------------------------*/
void Step::vCombinedNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpCombined = new Combined;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
