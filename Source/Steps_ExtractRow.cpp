/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"
#include "ePX.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ExtractRow(void)
{
  vAddStep("extractRow.create"_Stz,            &Step::vExtractRowCreate);
}

/*----------------------------------------------------------------------------*/
/* extractRow.create[] -- create the extractRow                               */
/*----------------------------------------------------------------------------*/
void Step::vExtractRowCreate(void)
{
  ENTRY(); // ENTRY("vExtractRowCreate"_Stz);
  
  ExtractRow* pExtractRow = new ExtractRow(*m_pRegSet->m_xpTable);
  
  m_pDataStack->vPushPtr(pExtractRow);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
