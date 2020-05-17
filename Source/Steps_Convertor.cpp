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

  vAddStep("convertor.convert"_Stz,   &Step::vConvertorConvert);
  vAddStep("convertor.delete"_Stz,    &Step::vConvertorDelete);
  vAddStep("convertor.inspect"_Stz,   &Step::vConvertorInspect);
}

/*----------------------------------------------------------------------------*/
/* convertor::new -- new a convertor                                          */
/*----------------------------------------------------------------------------*/
void Step::vConvertorNew(void)
{
  ENTRY();
  
  Convertor* pConvertor = new Convertor(AM::pFmt());
  
  m_pDataStack->vPushPtr(pConvertor);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convertor.convert -- convert                                               */
/*----------------------------------------------------------------------------*/
void Step::vConvertorConvert(void)
{
  ENTRY();

  Convertor* pConvertor = _pConvertor(m_pDataStack->pPopPtr(__func__));

  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pConvertor->bConvert(azSrc);
  
  EXIT();
}

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

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  s48K sDef;
  pStreamDef->vAsString(sDef);
  vTrace("%s\n"_Stz,sDef.szStz());

  char* azSrc  = (char*)pCsvBuffer->ayDataArea();
  
  bool bResult = pConvertor->bInspect(azSrc);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
