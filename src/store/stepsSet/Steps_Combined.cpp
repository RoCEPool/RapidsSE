/*============================================================================*/
/* Steps_Combined.cpp - Working tub of buffers                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Index.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Combined(void)
{
  vAddStep("Combined[]"_Stz,               &Step::vCombinedGet);
  vAddStep("Combined[].delete"_Stz,        &Step::vCombinedDelete);
  vAddStep("Combined[].execute"_Stz,       &Step::vCombinedExecute);
  vAddStep("Combined[].hashRows@"_Stz,     &Step::vCombinedHashRowsFetch);
  vAddStep("Combined[].indexT"_Stz,        &Step::vCombinedIndexT);
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

  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //pTub->vInCrwBuffer(pCrwBuffer);
  
  //RowPix* pRowPix = pCrwBuffer->pStreamDef()->pRowPix();
  //pTub->vRowPix(pRowPix);

  aCombinedT->m_pTub        = pTub;
  aCombinedT->m_iProcessRow = 1;

  if (pDataBuffer) {

    if (pDataBuffer->eGetClassNo() == CN_CRWBUFFER) {
      // CRWBUFFER
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);

      aCombinedT->m_aHeadLine0 = (HeadLine_T *) pCrwBuffer->aGetHead(0);
      aCombinedT->m_iHeadSize  = pCrwBuffer->nHeadSize();
      aCombinedT->m_nRows      = pCrwBuffer->nRows();
      //aCombinedT->m_nCols      = pCrwBuffer->pStreamDef()->nColumns();
    }
    else {
      // ROWBUFFER
      RASSERT(pDataBuffer->eGetClassNo() == CN_ROWBUFFER);
      RowBuffer *pRowBuffer = _pRowBuffer(pDataBuffer);

      aCombinedT->m_aRowBuffer  = pRowBuffer;
      aCombinedT->m_racStrAreaT = (char*)RowBuffer::szGetStrArea(pRowBuffer);
      aCombinedT->m_nRows       = pRowBuffer->nRows();
    }
  }
  else {
    //EOSMARKER
    // nothing to do
  }
  //IndexTree* pIndexTree = (IndexTree*)aCombinedT->m_aIndexTreeGby;
  //pIndexTree->vDump(false);

  aCombinedT->m_eStrInflateRule = SI_NONE;
  aCombinedT->m_bTrace     = iTrace;

  i8 iResult = afFun(aCombinedT);

  pTub->vInCrwBuffer(nullptr);

  // EOS marker handler done ?
  if ( afFun ==  m_p_TaskLogicUnit->m_afFun3 ) {
    IndexTree* pIndexTree = _pIndexTree(aCombinedT->m_aIndexTreeGby);
    delete pIndexTree;
    aCombinedT->m_aIndexTreeGby = nullptr;

    IndexDef* pIndexDef = _pIndexDef(aCombinedT->m_aIndexDefGby);
    delete pIndexDef;
    aCombinedT->m_aIndexDefGby = nullptr;
  }
  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].hashRows@ -- fetch hashRows                                     */
/*----------------------------------------------------------------------------*/
void Step::vCombinedHashRowsFetch(void) {
  ENTRY();

  Combined *pCombined = *m_p_TaskLogicUnit->m_xpCombined;
  Combined_T *aCombinedT = pCombined->aCombinedT();

  m_pDataStack->vPushInt(aCombinedT->m_iHashRows);
}

/*----------------------------------------------------------------------------*/
/* Combined[].indexT -- get the Combined indexT                               */
/*----------------------------------------------------------------------------*/
void Step::vCombinedIndexT(void)
{
  ENTRY();

  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  Index* pIndex = _pIndex(pCombined->aCombinedT()->m_aIndexT);

  m_pDataStack->vPushPtr(pIndex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].new -- new an Combined                                          */
/*----------------------------------------------------------------------------*/
void Step::vCombinedNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpCombined = new Combined(m_p_TaskLogicUnit);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
