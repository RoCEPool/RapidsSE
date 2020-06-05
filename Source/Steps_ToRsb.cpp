/*============================================================================*/
/* Steps_ToRsb.cpp - csv to rsb convertor                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRsb(void)
{
  vAddStep("toRsb::new"_Stz,          &Step::vToRsbNew);

  vAddStep("toRsb.delete"_Stz,        &Step::vToRsbDelete);
  vAddStep("toRsb.inspect"_Stz,       &Step::vToRsbInspect);
  
  vAddStep("toRsb[].convert"_Stz,     &Step::vToRsbRegConvert);
  vAddStep("toRsb[].create"_Stz,      &Step::vToRsbRegCreate);
  vAddStep("toRsb[].delete"_Stz,      &Step::vToRsbRegDelete);
  vAddStep("toRsb[].rsbBuffer@"_Stz,  &Step::vToRsbRegRsbBufferFetch);
  
  vAddStep("toRsb[].danglingRsbBuffer"_Stz, &Step::vToRsbRegDanglingRsbBuffer);
  vAddStep("toRsb[].fullRsbBuffer"_Stz,     &Step::vToRsbRegFullRsbBuffer);
}

/*----------------------------------------------------------------------------*/
/* toRsb::new -- new toRsb                                                    */
/*----------------------------------------------------------------------------*/
void Step::vToRsbNew(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  ToRsb* pToRsb = new ToRsb(pStreamDef,AM::pFmt());
  
  m_pDataStack->vPushPtr(pToRsb);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* toRsb.delete -- delete                                                     */
/*----------------------------------------------------------------------------*/
void Step::vToRsbDelete(void)
{
  ENTRY();
  
  ToRsb* pToRsb = _pToRsb(m_pDataStack->pPopPtr(__func__));

  delete pToRsb;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb.inspect -- inspect without actually converting                       */
/*----------------------------------------------------------------------------*/
void Step::vToRsbInspect(void)
{
  ENTRY();

  ToRsb* pToRsb = _pToRsb(m_pDataStack->pPopPtr(__func__));
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pToRsb->bInspect(azSrc);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* toRsb[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegConvert(void)
{
  ENTRY();

  ToRsb* pToRsb = *m_pRegSet->m_xpToRsb;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  
  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pToRsb->bConvert(azSrc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].create -- create toRsb                                             */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegCreate(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = m_pRegSet->m_xpTable->pStreamDef();
  
  ToRsb* pToRsb = new ToRsb(pStreamDef,AM::pFmt());
      
  m_pRegSet->m_xpToRsb = pToRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].delete -- delete the toRsb                                         */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDelete(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pRegSet->m_xpToRsb;
  delete pToRsb;
  
  m_pRegSet->m_xpToRsb = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].rsbBuffer@ -- fetch rsb buffer                                     */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegRsbBufferFetch(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pRegSet->m_xpToRsb;
  
  RsbBuffer* pRsbBuffer = pToRsb->pRsbBuffer();
  pToRsb->vRsbBuffer(nullptr);
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].danglingRsbBuffer -- fetch rsb buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegDanglingRsbBuffer(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pRegSet->m_xpToRsb;
  
  RsbBuffer* pRsbBuffer = pToRsb->pDanglingRsbBuffer();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].fullRsbBuffer -- full rsb buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToRsbRegFullRsbBuffer(void)
{
  ENTRY();
  
  ToRsb* pToRsb = *m_pRegSet->m_xpToRsb;
  
  RsbBuffer* pRsbBuffer = pToRsb->pFullRsbBuffer();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
