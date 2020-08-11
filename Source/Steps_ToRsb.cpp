/*============================================================================*/
/* Steps_ToRsb.cpp - csv to rsb convertor                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRsb(void)
{
  vAddStep("toRsb[].completed"_Stz,   &Step::vToRsbRegCompleted);
  vAddStep("toRsb[].convert"_Stz,     &Step::vToRsbRegConvert);
  vAddStep("toRsb[].create"_Stz,      &Step::vToRsbRegCreate);
  vAddStep("toRsb[].dangling"_Stz,    &Step::vToRsbRegDangling);
  vAddStep("toRsb[].delete"_Stz,      &Step::vToRsbRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toRsb[].fullRsbBuffer -- full rsb buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegCompleted(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  RsbBuffer* pRsbBuffer = pToRsb->pFullRsbBuffer();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegConvert(void)
{
  ENTRY();

  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pLogicUnit->m_xpOperandA);
  
  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pToRsb->bConvertCsv(azSrc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].create -- create toRsb                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegCreate(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = m_pLogicUnit->m_xpTable->pStreamDef();
  
  bool bPtnHash = true;
  ToRsb* pToRsb = new ToRsb(pStreamDef,AM::pFmt(),bPtnHash);
      
  m_pLogicUnit->m_xpToRsb = pToRsb;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].danglingRsbBuffer -- fetch rsb buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDangling(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  
  RsbBuffer* pRsbBuffer = pToRsb->pDanglingRsbBuffer();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].delete -- delete toRsb                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDelete(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pLogicUnit->m_xpToRsb;
  m_pLogicUnit->m_xpToRsb = nullptr;
  
  delete pToRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
