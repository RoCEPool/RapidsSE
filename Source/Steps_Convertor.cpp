/*============================================================================*/
/* Steps_Convertor.cpp - csv record convertor                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Convertor(void)
{
  vAddStep("toRsb::new"_Stz,      &Step::vConvertorNew);

  vAddStep("toRsb.delete"_Stz,    &Step::vConvertorDelete);
  vAddStep("toRsb.inspect"_Stz,   &Step::vConvertorInspect);
  
  vAddStep("toRsb[].convert"_Stz,  &Step::vConvertorRegConvert);
  vAddStep("toRsb[].create"_Stz,   &Step::vConvertorRegCreate);
  vAddStep("toRsb[].delete"_Stz,   &Step::vConvertorRegDelete);
}

/*----------------------------------------------------------------------------*/
/* convertor::new -- new a convertor                                          */
/*----------------------------------------------------------------------------*/
void Step::vConvertorNew(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  ToRsb* pConvertor = new ToRsb(pStreamDef,AM::pFmt());
  
  m_pDataStack->vPushPtr(pConvertor);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* toRsb.delete -- delete                                                 */
/*----------------------------------------------------------------------------*/
void Step::vConvertorDelete(void)
{
  ENTRY();
  
  ToRsb* pConvertor = _pToRsb(m_pDataStack->pPopPtr(__func__));

  delete pConvertor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb.inspect -- inspect without actually converting                   */
/*----------------------------------------------------------------------------*/
void Step::vConvertorInspect(void)
{
  ENTRY();

  ToRsb* pConvertor = _pToRsb(m_pDataStack->pPopPtr(__func__));
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pConvertor->bInspect(azSrc);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convertor.convert -- convert csv buffer                                    */
/*----------------------------------------------------------------------------*/
void Step::vConvertorRegConvert(void)
{
  ENTRY();

  ToRsb* pConvertor = *m_pRegSet->m_xpConvertor;
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpOperandA);
  
  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pConvertor->bConvert(azSrc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* convertor[].create -- create a convertor                                   */
/*----------------------------------------------------------------------------*/
void Step::vConvertorRegCreate(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = m_pRegSet->m_xpTable->pStreamDef();
  
  ToRsb* pConvertor = new ToRsb(pStreamDef,AM::pFmt());
      
  m_pRegSet->m_xpConvertor = pConvertor;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* convertor[].delete -- delete the convertor                                 */
/*----------------------------------------------------------------------------*/
void Step::vConvertorRegDelete(void)
{
  ENTRY();
  
  ToRsb* pConvertor = *m_pRegSet->m_xpConvertor;
  delete pConvertor;
  
  m_pRegSet->m_xpConvertor = nullptr;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
