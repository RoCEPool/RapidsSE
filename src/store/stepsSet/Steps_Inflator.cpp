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
void Step::vAddSteps_Inflator(void)
{
  vAddStep("Inflator[]"_Stz,               &Step::vInflatorGet);
  vAddStep("Inflator[].delete"_Stz,        &Step::vInflatorDelete);
  vAddStep("Inflator[].inflateCrw"_Stz,    &Step::vInflatorInflateCrw);
  vAddStep("Inflator[].new"_Stz,           &Step::vInflatorNew);
}

/*----------------------------------------------------------------------------*/
/* Inflator[].delete -- delete the inflator                                   */
/*----------------------------------------------------------------------------*/
void Step::vInflatorDelete(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpInflator.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Inflator[].get -- get the inflator                                         */
/*----------------------------------------------------------------------------*/
void Step::vInflatorGet(void)
{
  ENTRY();
  
  Inflator* pInflator = * m_p_TaskLogicUnit->m_xpInflator;
  
  m_pDataStack->vPushPtr(pInflator);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Inflator[].inflateCrw -- inflate a crw buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vInflatorInflateCrw(void)
{
  ENTRY();
  
  Inflator* pInflator = * m_p_TaskLogicUnit->m_xpInflator;
  CrwTubInflate_T* aCrwTubInflate = pInflator->aCrwTubInflate();
    
  
  i8 iTrace = m_pDataStack->iPopInt(__func__);

  bool (*afFun)(void*);
  afFun = (bool (*)(void*))m_pDataStack->iPopInt(__func__);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pTub->vInCrwBuffer(pCrwBuffer);
  
  RowPix* pRowPix = pCrwBuffer->pStreamDef()->pRowPix();
  pTub->vRowPix(pRowPix);

  aCrwTubInflate->m_pTub       = pTub;
  aCrwTubInflate->m_aHeadLine0 = (HeadLine_T*)pCrwBuffer->aGetHead(0);
  aCrwTubInflate->m_iHeadSize  = pCrwBuffer->nHeadSize();
  aCrwTubInflate->m_nRows      = pCrwBuffer->nRows();
  aCrwTubInflate->m_nCols      = pCrwBuffer->pStreamDef()->nColumns();
  aCrwTubInflate->m_eStrInflateRule = SI_NONE;
  aCrwTubInflate->m_bTrace     = iTrace;

  i8 iResult = afFun(aCrwTubInflate);

  pTub->vInCrwBuffer(nullptr);

  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Inflator[].new -- new an inflator                                          */
/*----------------------------------------------------------------------------*/
void Step::vInflatorNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpInflator = new Inflator;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
