/*============================================================================*/
/* Steps_Dynamic.cpp - Dynamic code                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Index.hpp"
#include "Op.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dynamic(void)
{
  vAddStep("Dynamic[]"_Stz,               &Step::vDynamicGet);
  vAddStep("Dynamic[].delete"_Stz,        &Step::vDynamicDelete);
  vAddStep("Dynamic[].execute"_Stz,       &Step::vDynamicExecute);
  vAddStep("Dynamic[].hashRows@"_Stz,     &Step::vDynamicHashRowsFetch);
  vAddStep("Dynamic[].IsIndexScan@"_Stz,  &Step::vDynamicIsIndexScanFetch);
  vAddStep("Dynamic[].IsIndexScan!"_Stz,  &Step::vDynamicIsIndexScanStore);
  vAddStep("Dynamic[].new"_Stz,           &Step::vDynamicNew);
  vAddStep("Dynamic[].streamDefOut"_Stz,  &Step::vDynamicStreamDefOut);
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].delete -- delete the Dynamic                                   */
/*----------------------------------------------------------------------------*/
void Step::vDynamicDelete(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpDynamic.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].get -- get the Dynamic                                         */
/*----------------------------------------------------------------------------*/
void Step::vDynamicGet(void)
{
  ENTRY();
  
  Dynamic* pDynamic = * m_pLogicUnit->m_xpDynamic;
  
  m_pDataStack->vPushPtr(pDynamic);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].Execute -- execute Dynamic code                                  */
/*----------------------------------------------------------------------------*/
void Step::vDynamicExecute(void)
{
  ENTRY();

  i8 iResult;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get DynamicT
  Dynamic* pDynamic = * m_pLogicUnit->m_xpDynamic;
  Dynamic_T* aDynamicT = pDynamic->aDynamicT();

  vTrace("Stage: %s %p \n"_Stz, m_pOp->pStage()->szStageNm(), aDynamicT);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the trace flag
  i8 iTrace = m_pDataStack->iPopInt(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the function pointer
  bool (*afFun)(void*);
  afFun = (bool (*)(void*))m_pDataStack->iPopInt(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the data buffer
  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // setup DynamicT
  aDynamicT->m_pPot = * m_pLogicUnit->m_xpPot;
  aDynamicT->m_pTub = * m_pLogicUnit->m_xpTub;
  aDynamicT->m_iErrorCode = (i8)EC_NONE;
  aDynamicT->m_eStrInflateRule = SI_STRAREA_ADR;
  aDynamicT->m_bTrace     = iTrace;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DataBuffer ?
  if (pDataBuffer) {

    // CRWBUFFER ?
    if (pDataBuffer->eGetClassNo() == CN_CRWBUFFER) {

      CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);

      aDynamicT->m_aHeadLine0 = (HeadLine_T *) pCrwBuffer->aGetHead(0);
      aDynamicT->m_iHeadSize  = pCrwBuffer->nHeadSize();
      aDynamicT->m_nRows      = pCrwBuffer->nRows();
    }
    else {
      // ROWBUFFER
      RASSERT(pDataBuffer->eGetClassNo() == CN_ROWBUFFER);

      RowBuffer *pRowBuffer = _pRowBuffer(pDataBuffer);

      aDynamicT->m_aRowBuffer  = pRowBuffer;
      aDynamicT->m_xRowInfoIT.m_acStrArea = (char*)pRowBuffer->szStzAllot_begin();
      aDynamicT->m_nRows       = pRowBuffer->nRows();
    }
  }

  /***************************************************
  if ( afFun ==  m_pLogicUnit->m_afFun3 ) {
    // invoke with null databuffer (ordAgg)
    if (aDynamicT->m_iHasOrdAggRow) {
      aDynamicT->m_iOrdAggDone = 1;
      if (aDynamicT->m_iIndexScan)
        iResult = m_pLogicUnit->m_afFun2(aDynamicT);
      else
        iResult = m_pLogicUnit->m_afFun1(aDynamicT);
    }
  }
  *******************************************************/

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // execute the dynamic function
  iResult = afFun(aDynamicT);

  // EOS marker handler done ?
  if ( afFun ==  m_pLogicUnit->m_afFun3 ) {
    IndexTree* pIndexTree = _pIndexTree(aDynamicT->m_xIndexInfoPL.m_aIndexTree);
    delete pIndexTree;
    aDynamicT->m_xIndexInfoPL.m_aIndexTree = nullptr;

    IndexDef* pIndexDef = _pIndexDef(aDynamicT->m_xIndexInfoPL.m_aIndexDef);
    delete pIndexDef;
    aDynamicT->m_xIndexInfoPL.m_aIndexDef = nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push error
  if ( !iResult )
    m_pDataStack->vPushInt(aDynamicT->m_iErrorCode);

  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].hashRows@ -- fetch hashRows                                      */
/*----------------------------------------------------------------------------*/
void Step::vDynamicHashRowsFetch(void) {
  ENTRY();

  Dynamic *pDynamic = *m_pLogicUnit->m_xpDynamic;
  Dynamic_T *aDynamicT = pDynamic->aDynamicT();

  m_pDataStack->vPushInt(aDynamicT->m_iPot);
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].IsIndexScan! -- store the Dynamic IsIndexScan                    */
/*----------------------------------------------------------------------------*/
void Step::vDynamicIsIndexScanStore(void)
{
  ENTRY();

  Dynamic* pDynamic = * m_pLogicUnit->m_xpDynamic;
  bool bIndexScan = m_pDataStack->iPopBol(__func__);

  pDynamic->aDynamicT()->m_bIndexScan = bIndexScan;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].IsIndexScan@ -- fetch the Dynamic IsIndexScan                    */
/*----------------------------------------------------------------------------*/
void Step::vDynamicIsIndexScanFetch(void)
{
  ENTRY();

  Dynamic* pDynamic = * m_pLogicUnit->m_xpDynamic;

  m_pDataStack->vPushBol(pDynamic->aDynamicT()->m_bIndexScan);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].new -- new an Dynamic                                            */
/*----------------------------------------------------------------------------*/
void Step::vDynamicNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpDynamic = new Dynamic(m_pLogicUnit);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Dynamic[].streamDefOut -- get the streamDefOut                             */
/*----------------------------------------------------------------------------*/
void Step::vDynamicStreamDefOut(void)
{
  ENTRY();

  Dynamic* pDynamic = *m_pLogicUnit->m_xpDynamic;

  m_pDataStack->vPushPtr((Object*)pDynamic->aDynamicT()->m_aStreamDefOut);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
