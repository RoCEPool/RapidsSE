/*============================================================================*/
/* Steps_Tub.cpp - Working tub of buffers                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

int iFun0(struct Combined_T* aCombinedT);
int iFun1(struct Combined_T* aCombinedT);
int iFun2(struct Combined_T* aCombinedT);
int iFun3(struct Combined_T* aCombinedT);

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Combined(void)
{
  vAddStep("Combined[]"_Stz,               &Step::vCombinedGet);
  vAddStep("Combined[].delete"_Stz,        &Step::vCombinedDelete);
  vAddStep("Combined[].execute"_Stz,       &Step::vCombinedExecute);
  vAddStep("Combined[].executeFun0"_Stz,   &Step::vCombinedExecuteFun0);
  vAddStep("Combined[].executeFun1"_Stz,   &Step::vCombinedExecuteFun1);
  vAddStep("Combined[].executeFun2"_Stz,   &Step::vCombinedExecuteFun2);
  vAddStep("Combined[].executeFun3"_Stz,   &Step::vCombinedExecuteFun3);
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

  aCombinedT->m_pTub       = pTub;

  if (pDataBuffer) {

    if (pDataBuffer->eGetClassNo() == CN_CRWBUFFER) {
      // CRWBUFFER
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);

      aCombinedT->m_aHeadLine0 = (HeadLine_T *) pCrwBuffer->aGetHead(0);
      aCombinedT->m_iHeadSize  = pCrwBuffer->nHeadSize();
      aCombinedT->m_nRows      = pCrwBuffer->nRows();
      aCombinedT->m_nCols      = pCrwBuffer->pStreamDef()->nColumns();
    }
    else {
      // ROWBUFFER
      RASSERT(pDataBuffer->eGetClassNo() == CN_ROWBUFFER);
      RowBuffer *pRowBuffer = _pRowBuffer(pDataBuffer);

      aCombinedT->m_aRowBuffer  = pRowBuffer;
      aCombinedT->m_racStrAreaT = (void*)RowBuffer::szGetStrArea(pRowBuffer);
      aCombinedT->m_nRows       = pRowBuffer->nRows();
    }
  }
  else {
    //EOSMARKER
    // nothing to do
  }

  aCombinedT->m_eStrInflateRule = SI_NONE;

  aCombinedT->m_aEntWrkT    = (void*)&m_p_TaskLogicUnit->m_oEntWrkT;
  aCombinedT->m_aEntIntT    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkT.aEnt();
  aCombinedT->m_aEntKeyT    = ((EntWrk*)aCombinedT->m_aEntWrkT)->aKey();

  aCombinedT->m_aEntWrkJ    = (void*)&m_p_TaskLogicUnit->m_oEntWrkJ;
  aCombinedT->m_aEntIntJ    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkJ.aEnt();
  aCombinedT->m_aEntKeyJ    = ((EntWrk*)aCombinedT->m_aEntWrkJ)->aKey();

  aCombinedT->m_aEntWrkPL  = (void*)&m_p_TaskLogicUnit->m_oEntWrkPL;
  aCombinedT->m_aEntIntPL  = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkPL.aEnt();
  aCombinedT->m_aEntKeyPL  = ((EntWrk*)aCombinedT->m_aEntWrkPL)->aKey();
  aCombinedT->m_bTrace     = iTrace;

  i8 iResult = afFun(aCombinedT);

  pTub->vInCrwBuffer(nullptr);

  // EOS marker handler done ?
  if ( afFun ==  m_p_TaskLogicUnit->m_afFun3 ) {
    IndexTree* pIndexTree = _pIndexTree(aCombinedT->m_aIndexTreeGby);
    delete pIndexTree;

    IndexDef* pIndexDef = _pIndexDef(aCombinedT->m_aIndexDefGby);
    delete pIndexDef;
  }
  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].ExecuteFun0 -- execute combined code for func0                  */
/*----------------------------------------------------------------------------*/
void Step::vCombinedExecuteFun0(void)
{
  ENTRY();

  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  Combined_T* aCombinedT = pCombined->aCombinedT();

  i8 iTrace = m_pDataStack->iPopInt(__func__);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aCombinedT->m_pTub       = pTub;

  RASSERT(pDataBuffer->eGetClassNo() == CN_CRWBUFFER);

  // CRWBUFFER
  CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);

  aCombinedT->m_aHeadLine0 = (HeadLine_T *) pCrwBuffer->aGetHead(0);
  aCombinedT->m_iHeadSize  = pCrwBuffer->nHeadSize();
  aCombinedT->m_nRows      = pCrwBuffer->nRows();
  aCombinedT->m_nCols      = pCrwBuffer->pStreamDef()->nColumns();

  aCombinedT->m_eStrInflateRule = SI_NONE;

  aCombinedT->m_aEntWrkT    = (void*)&m_p_TaskLogicUnit->m_oEntWrkT;
  aCombinedT->m_aEntIntT    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkT.aEnt();
  aCombinedT->m_aEntKeyT    = ((EntWrk*)aCombinedT->m_aEntWrkT)->aKey();

  aCombinedT->m_aEntWrkJ    = (void*)&m_p_TaskLogicUnit->m_oEntWrkJ;
  aCombinedT->m_aEntIntJ    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkJ.aEnt();
  aCombinedT->m_aEntKeyJ    = ((EntWrk*)aCombinedT->m_aEntWrkJ)->aKey();

  aCombinedT->m_aEntWrkPL  = (void*)&m_p_TaskLogicUnit->m_oEntWrkPL;
  aCombinedT->m_aEntIntPL  = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkPL.aEnt();
  aCombinedT->m_aEntKeyPL  = ((EntWrk*)aCombinedT->m_aEntWrkPL)->aKey();
  aCombinedT->m_bTrace     = iTrace;

//  i8 iResult = iFun0(aCombinedT);
//
//  pTub->vInCrwBuffer(nullptr);
//
//  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].ExecuteFun1 -- execute combined code                            */
/*----------------------------------------------------------------------------*/
void Step::vCombinedExecuteFun1(void)
{
  ENTRY();

  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  Combined_T* aCombinedT = pCombined->aCombinedT();

  i8 iTrace = m_pDataStack->iPopInt(__func__);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // ROWBUFFER
  RASSERT(pDataBuffer->eGetClassNo() == CN_ROWBUFFER);
  RowBuffer *pRowBuffer = _pRowBuffer(pDataBuffer);

  aCombinedT->m_aRowBuffer  = pRowBuffer;
  aCombinedT->m_racStrAreaT = (void*)RowBuffer::szGetStrArea(pRowBuffer);
  aCombinedT->m_nRows       = pRowBuffer->nRows();

  aCombinedT->m_eStrInflateRule = SI_NONE;

  aCombinedT->m_aEntWrkT    = (void*)&m_p_TaskLogicUnit->m_oEntWrkT;
  aCombinedT->m_aEntIntT    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkT.aEnt();
  aCombinedT->m_aEntKeyT    = ((EntWrk*)aCombinedT->m_aEntWrkT)->aKey();

  aCombinedT->m_aEntWrkJ    = (void*)&m_p_TaskLogicUnit->m_oEntWrkJ;
  aCombinedT->m_aEntIntJ    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkJ.aEnt();
  aCombinedT->m_aEntKeyJ    = ((EntWrk*)aCombinedT->m_aEntWrkJ)->aKey();

  aCombinedT->m_aEntWrkPL  = (void*)&m_p_TaskLogicUnit->m_oEntWrkPL;
  aCombinedT->m_aEntIntPL  = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkPL.aEnt();
  aCombinedT->m_aEntKeyPL  = ((EntWrk*)aCombinedT->m_aEntWrkPL)->aKey();
  aCombinedT->m_bTrace     = iTrace;

//  i8 iResult = iFun1(aCombinedT);
//
//  pTub->vInCrwBuffer(nullptr);
//
//  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].ExecuteFun2 -- execute combined code                            */
/*----------------------------------------------------------------------------*/
void Step::vCombinedExecuteFun2(void)
{
  ENTRY();

  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  Combined_T* aCombinedT = pCombined->aCombinedT();


  i8 iTrace = m_pDataStack->iPopInt(__func__);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aCombinedT->m_pTub       = pTub;

  aCombinedT->m_eStrInflateRule = SI_NONE;

  aCombinedT->m_aEntWrkT    = (void*)&m_p_TaskLogicUnit->m_oEntWrkT;
  aCombinedT->m_aEntIntT    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkT.aEnt();
  aCombinedT->m_aEntKeyT    = ((EntWrk*)aCombinedT->m_aEntWrkT)->aKey();

  aCombinedT->m_aEntWrkJ    = (void*)&m_p_TaskLogicUnit->m_oEntWrkJ;
  aCombinedT->m_aEntIntJ    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkJ.aEnt();
  aCombinedT->m_aEntKeyJ    = ((EntWrk*)aCombinedT->m_aEntWrkJ)->aKey();

  aCombinedT->m_aEntWrkPL  = (void*)&m_p_TaskLogicUnit->m_oEntWrkPL;
  aCombinedT->m_aEntIntPL  = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkPL.aEnt();
  aCombinedT->m_aEntKeyPL  = ((EntWrk*)aCombinedT->m_aEntWrkPL)->aKey();
  aCombinedT->m_bTrace     = iTrace;

//  i8 iResult = iFun2(aCombinedT);
//
//  pTub->vInCrwBuffer(nullptr);
//
//  m_pDataStack->vPushInt(iResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Combined[].ExecuteFun3 -- execute combined code                            */
/*----------------------------------------------------------------------------*/
void Step::vCombinedExecuteFun3(void)
{
  ENTRY();

  Combined* pCombined = * m_p_TaskLogicUnit->m_xpCombined;
  Combined_T* aCombinedT = pCombined->aCombinedT();


  i8 iTrace = m_pDataStack->iPopInt(__func__);


  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  DataBuffer* pDataBuffer = _pDataBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aCombinedT->m_pTub       = pTub;


  aCombinedT->m_eStrInflateRule = SI_NONE;

  aCombinedT->m_aEntWrkT    = (void*)&m_p_TaskLogicUnit->m_oEntWrkT;
  aCombinedT->m_aEntIntT    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkT.aEnt();
  aCombinedT->m_aEntKeyT    = ((EntWrk*)aCombinedT->m_aEntWrkT)->aKey();

  aCombinedT->m_aEntWrkJ    = (void*)&m_p_TaskLogicUnit->m_oEntWrkJ;
  aCombinedT->m_aEntIntJ    = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkJ.aEnt();
  aCombinedT->m_aEntKeyJ    = ((EntWrk*)aCombinedT->m_aEntWrkJ)->aKey();

  aCombinedT->m_aEntWrkPL  = (void*)&m_p_TaskLogicUnit->m_oEntWrkPL;
  aCombinedT->m_aEntIntPL  = (Ent_int*)m_p_TaskLogicUnit->m_oEntWrkPL.aEnt();
  aCombinedT->m_aEntKeyPL  = ((EntWrk*)aCombinedT->m_aEntWrkPL)->aKey();
  aCombinedT->m_bTrace     = iTrace;

  //i8 iResult = iFun3(aCombinedT);

  pTub->vInCrwBuffer(nullptr);

  IndexTree* pIndexTree = _pIndexTree(aCombinedT->m_aIndexTreeGby);
  delete pIndexTree;

  IndexDef* pIndexDef = _pIndexDef(aCombinedT->m_aIndexDefGby);
  delete pIndexDef;

  //m_pDataStack->vPushInt(iResult);

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
