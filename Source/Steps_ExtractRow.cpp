/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ExtractRow(void)
{
  vAddStep("extractRow.create"_Stz,            &Step::vExtractRowCreate);
  vAddStep("extractRow.delete"_Stz,            &Step::vExtractRowDelete);
}

/*----------------------------------------------------------------------------*/
/* extractRow.create -- create the extractRow                                 */
/*----------------------------------------------------------------------------*/
void Step::vExtractRowCreate(void)
{
  ENTRY(); // ENTRY("vExtractRowCreate"_Stz);
  
  ExtractRow* pExtractRow = new ExtractRow(*m_pRegSet->m_xpTable);
  
  m_pDataStack->vPushPtr(pExtractRow);
}

/*----------------------------------------------------------------------------*/
/* extractRow.delete -- delete the extractRow                                 */
/*----------------------------------------------------------------------------*/
void Step::vExtractRowDelete(void)
{
  ENTRY(); // ENTRY("vExtractRowDelete"_Stz);
  
  ExtractRow* pExtractRow = _pExtractRow(*m_pRegSet->m_xpRegister0);
  delete pExtractRow;
  
  m_pRegSet->m_xpRegister0 = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
