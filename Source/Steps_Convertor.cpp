/*============================================================================*/
/* Steps_Convertor.cpp - csv record convertor                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Convertor(void)
{
  vAddStep("convertor::new"_Stz,      &Step::vConvertorNew);

  vAddStep("convertor.delete"_Stz,    &Step::vConvertorDelete);
  vAddStep("convertor.inspect"_Stz,   &Step::vConvertorInspect);
  
  vAddStep("convertor[].convert"_Stz,  &Step::vConvertorRegConvert);
  vAddStep("convertor[].create"_Stz,   &Step::vConvertorRegCreate);
  vAddStep("convertor[].delete"_Stz,   &Step::vConvertorRegDelete);
}

/*----------------------------------------------------------------------------*/
/* convertor::new -- new a convertor                                          */
/*----------------------------------------------------------------------------*/
void Step::vConvertorNew(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  Convertor* pConvertor = new Convertor(pStreamDef,AM::pFmt());
  
  m_pDataStack->vPushPtr(pConvertor);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convertor.delete -- delete                                                 */
/*----------------------------------------------------------------------------*/
void Step::vConvertorDelete(void)
{
  ENTRY();
  
  Convertor* pConvertor = _pConvertor(m_pDataStack->pPopPtr(__func__));

  delete pConvertor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* convertor.inspect -- inspect without actually converting                   */
/*----------------------------------------------------------------------------*/
void Step::vConvertorInspect(void)
{
  ENTRY();

  Convertor* pConvertor = _pConvertor(m_pDataStack->pPopPtr(__func__));
  
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

  Convertor* pConvertor = *m_pRegSet->m_xpConvertor;
  
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
  
  Convertor* pConvertor = new Convertor(pStreamDef,AM::pFmt());
      
  m_pRegSet->m_xpConvertor = pConvertor;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* convertor[].delete -- delete the convertor                                 */
/*----------------------------------------------------------------------------*/
void Step::vConvertorRegDelete(void)
{
  ENTRY();
  
  Convertor* pConvertor = *m_pRegSet->m_xpConvertor;
  delete pConvertor;
  
  m_pRegSet->m_xpConvertor = nullptr;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
