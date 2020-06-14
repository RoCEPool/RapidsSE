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
  vAddStep("tableDef::syntax"_Stz, &Step::vTableDefSyntax);
}

/*----------------------------------------------------------------------------*/
/* tableDef::parse -- parse Create Table statement and produce a TableDef     */
/*----------------------------------------------------------------------------*/
void Step::vTableDefParse(void)
{
  ENTRY();
  
  Job* pJob_Local = _pJob_Local(m_pDataStack->pPopPtr(__func__));
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  TableDef* pTableDef = new TableDef;
  
  eEC eResult = pTableDef->eParse(sCmd);
  
  if (eResult != EC_NONE) {
    
    // at this stage shouldn't be a syntax error
    ASSERT(eResult != EC_INVALID_COMMAND);
    
    pJob_Local->vJobError(eResult);
    
    // delete tabledef
    delete pTableDef;
    
    pTableDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pTableDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tableDef::syntax -- parse Create Table statement and check sytax           */
/*----------------------------------------------------------------------------*/
void Step::vTableDefSyntax(void)
{
  ENTRY();
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  eEC eResult = TableDef::eSyntax(sCmd);
  
  m_pDataStack->vPushInt(eResult);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
