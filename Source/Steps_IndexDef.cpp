/*============================================================================*/
/* Steps_IndexDef.cpp - index definition step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexDef(void)
{
  vAddStep("indexDef[]"_Stz,          &Step::vIndexDefGet);
  vAddStep("indexDef[].isNull"_Stz,   &Step::vIndexDefIsNull);
  vAddStep("indexDef::parse"_Stz,     &Step::vIndexDefParse);
  vAddStep("indexDef::parseOLD"_Stz,  &Step::vIndexDefParseOLD);
  vAddStep("indexDef[].set"_Stz,      &Step::vIndexDefSet);
  vAddStep("indexDef::syntax"_Stz,    &Step::vIndexDefSyntax);
  vAddStep("indexDef[].tableName@"_Stz, &Step::vIndexDefTableNameFetch);
}

/*----------------------------------------------------------------------------*/
/* indexDef[].get -- get indexDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef[].isNull -- is indexDef null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpIndexDef.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::parseOLD -- parse Create Index statement and produce a IndexDef  */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParseOLD(void)
{
  ENTRY();
  
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
    
    // at this stage shouldn't be a syntax error
    ASSERT(eResult != EC_INVALID_COMMAND);
    
    // set the error in the job
    pJob_CreateIndex->vJobError(eResult);
    
    // delete IndexDef
    delete pIndexDef;
    
    pIndexDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pIndexDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::parse -- parse Create Index statement and produce a IndexDef     */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParse(void)
{
  ENTRY();
  
  Job* pJob_Local = _pJob_Local(m_pDataStack->pPopPtr(__func__));
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  IndexDef* pIndexDef = new IndexDef;
  
  eEC eResult = pIndexDef->eParse(pTable,sCmd);
  
  
  if (eResult != EC_NONE) {
    
    // set the error in the job
    pJob_Local->vJobError(eResult);
    
    // delete IndexDef
    delete pIndexDef;
    
    pIndexDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef[].set -- set indexDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefSet(void)
{
  ENTRY();
  
  m_pRegSet->m_xpIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::syntax -- parse Create Index statement and check sytax           */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefSyntax(void)
{
  ENTRY();
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  eEC eResult = IndexDef::eSyntax(sCmd);
  
  m_pDataStack->vPushInt(eResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef.tableName@ -- fetch tableNm from indexDef                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefTableNameFetch(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = *m_pRegSet->m_xpIndexDef;
  
  s32 sTableNm = pIndexDef->szTableNm();
  
  m_pDataStack->vPushStr(sTableNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
