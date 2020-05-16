/*============================================================================*/
/* Steps_Convert.cpp - csv record convertor                                   */
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
}

/*----------------------------------------------------------------------------*/
/* convertor::new -- new a convertor                                          */
/*----------------------------------------------------------------------------*/
void Step::vConvertorNew(void)
{
  ENTRY();
  
  Convert* pConvert = new Convert(AM::pFmt());
  
  m_pDataStack->vPushPtr(pConvert);
    
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

  Convert* pConvert = _pConvert(m_pDataStack->pPopPtr(__func__));

  bool bResult = pConvert->bConvert(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* convertor.delete -- delete                                                 */
/*----------------------------------------------------------------------------*/
void Step::vConvertorDelete(void)
{
  ENTRY();
  
  Convert* pConvert = _pConvert(m_pDataStack->pPopPtr(__func__));

  delete pConvert;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
