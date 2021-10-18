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
  vAddStep("Combined[].inflateCrw"_Stz,    &Step::vCombinedInflateCrw);
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
/* Combined[].inflateCrw -- inflate a crw buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vCombinedInflateCrw(void)
{
  ENTRY();
  
  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  CrwTubInflate_T* aCrwTubInflate = pCombined->aCrwTubInflate();
    
  
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
