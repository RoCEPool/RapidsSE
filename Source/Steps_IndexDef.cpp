/*============================================================================*/
/* Steps_IndexDef.cpp - index definition step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexDef(void)
{
  vAddStep("indexDef::parse"_Stz,  &Step::vIndexDefParse);
  vAddStep("indexDef::syntax"_Stz, &Step::vIndexDefSyntax);
}

/*----------------------------------------------------------------------------*/
/* indexDef::parse -- parse Create Index statement and produce a IndexDef     */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParse(void)
{
  ENTRY(); // ENTRY("vIndexDefParse"_Stz);
  
  Job* pJob_CreateIndex = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

//  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
//  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
//  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);

  IndexDef* pIndexDef = new IndexDef;
  
  eEC eResult = pIndexDef->eParse(pTable,sCmd);
  
  if (eResult != EC_NONE) {
    
    // set the syntax error in the job
    pJob_CreateIndex->vSyntaxError(eResult);
    
    pJob_CreateIndex->vParticipantHasError(true);
    pJob_CreateIndex->vErrorCode(eResult);
    
    // delete IndexDef
    delete pIndexDef;
    
    pIndexDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pIndexDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::syntax -- parse Create Index statement and check sytax           */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefSyntax(void)
{
  ENTRY(); // ENTRY("vIndexDefSyntax"_Stz);
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  eEC eResult = IndexDef::eSyntax(sCmd);
  
  m_pDataStack->vPushInt(eResult);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
