/*============================================================================*/
/* Steps_ToRsb.cpp - csv to rsb convertor                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToRsb(void)
{
  vAddStep("toRsb::new"_Stz,      &Step::vToRsbNew);

  vAddStep("toRsb.delete"_Stz,    &Step::vToRsbDelete);
  vAddStep("toRsb.inspect"_Stz,   &Step::vToRsbInspect);
  
  vAddStep("toRsb[].convert"_Stz, &Step::vToRsbRegConvert);
  vAddStep("toRsb[].create"_Stz,  &Step::vToRsbRegCreate);
  vAddStep("toRsb[].delete"_Stz,  &Step::vToRsbRegDelete);
}

/*----------------------------------------------------------------------------*/
/* convertor::new -- new a convertor                                          */
/*----------------------------------------------------------------------------*/
void Step::vToRsbNew(void)
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
/* toRsb.delete -- delete                                                     */
/*----------------------------------------------------------------------------*/
void Step::vToRsbDelete(void)
{
  ENTRY();
  
  ToRsb* pConvertor = _pToRsb(m_pDataStack->pPopPtr(__func__));

  delete pConvertor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb.inspect -- inspect without actually converting                       */
/*----------------------------------------------------------------------------*/
void Step::vToRsbInspect(void)
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
void Step::vToRsbRegConvert(void)
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
void Step::vToRsbRegCreate(void)
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
void Step::vToRsbRegDelete(void)
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
