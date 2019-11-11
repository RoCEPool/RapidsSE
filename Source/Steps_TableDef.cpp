/*============================================================================*/
/* Steps_TableDef.cpp - table definition step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TableDef(void)
{
  vAddStep("tableDef::parse"_Stz, &Step::vTableDefParse);
}

/*----------------------------------------------------------------------------*/
/* tableDef::parse -- parse Create Table statement and produce a TableDef     */
/*----------------------------------------------------------------------------*/
void Step::vTableDefParse(void)
{
  ENTRY("vTableDefParse"_Stz);
  
  s4K sCmd;
  if (! m_pDataStack->bPopS4K(sCmd))
    THROW_DSTACK();

  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  TableDef* pTableDef = new TableDef;
  
  eEC eResult = pTableDef->eParse(sCmd);
  
//  delete pTableDef;
//  pTableDef = nullptr;
  
  m_pDataStack->vPushPointer(pTableDef);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
