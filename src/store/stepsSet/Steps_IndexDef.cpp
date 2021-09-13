/*============================================================================*/
/* Steps_IndexDef.cpp - index definition step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "Node_I.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexDef(void)
{
  vAddStep("IndexDef[]"_Stz,          &Step::vIndexDefGet);
  vAddStep("IndexDef[].addColumn"_Stz,&Step::vIndexDefAddColumn);
  vAddStep("IndexDef[].delete"_Stz,   &Step::vIndexDefDelete);
  vAddStep("IndexDef[].isNull"_Stz,   &Step::vIndexDefIsNull);
  vAddStep("IndexDef[].name!"_Stz,    &Step::vIndexDefNameStore);
  vAddStep("IndexDef[].new"_Stz,      &Step::vIndexDefNew);
  vAddStep("indexDef::parse"_Stz,     &Step::vIndexDefParse);
  vAddStep("indexDef::parseOLD"_Stz,  &Step::vIndexDefParseOLD);
  vAddStep("indexDef::parseTableName"_Stz, &Step::vIndexDefParseTableName);
  vAddStep("IndexDef[].set"_Stz,      &Step::vIndexDefSet);
  vAddStep("indexDef::syntax"_Stz,    &Step::vIndexDefSyntax);
  vAddStep("IndexDef[].tableName@"_Stz, &Step::vIndexDefTableNameFetch);
  vAddStep("IndexDef[].tableName!"_Stz, &Step::vIndexDefTableNameStore);
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].addColumn -- add a columnto the indexDef                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefAddColumn(void)
{
  ENTRY();
  
  s32 sColumnNm;
  m_pDataStack->vPopStr(sColumnNm,__func__);
  
  eCT eColType = (eCT)m_pDataStack->iPopInt(__func__);
  
  IndexDef* pIndexDef = * m_p_TaskLogicUnit->m_xpIndexDef;
  
  pIndexDef->vAddColumn(sColumnNm,eColType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].delete -- delete index definition                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefDelete(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = * m_p_TaskLogicUnit->m_xpIndexDef;
  
  m_p_TaskLogicUnit->m_xpIndexDef = nullptr;
  
  delete pIndexDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].get -- get indexDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].isNull -- is indexDef null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpIndexDef.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].name! -- set name                                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefNameStore(void)
{
  ENTRY();
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  m_p_TaskLogicUnit->m_xpIndexDef->vIndexNm(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].new -- new a index definition                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpIndexDef = new IndexDef;
  
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
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
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
    pJob->vJobError(eResult);
    
    // delete IndexDef
    delete pIndexDef;
    
    pIndexDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].set -- set indexDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefSet(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  
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
/* indexDef::parseTableName -- parse Create Index statement and get tableNm   */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParseTableName(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  s32 sTableNm;
  
  eEC eResult = IndexDef::eParseTableName(sCmd, sTableNm);
  
  if (eResult != EC_NONE) {
    
    // set the error in the job
    pJob->vJobError(eResult);
  }
  
  m_pDataStack->vPushStr(sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].tableName@ -- fetch tableNm from indexDef                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefTableNameFetch(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = *m_p_TaskLogicUnit->m_xpIndexDef;
  
  s32 sTableNm = pIndexDef->szTableNm();
  
  m_pDataStack->vPushStr(sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].tableName! -- store tableNm in indexDef                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefTableNameStore(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = *m_p_TaskLogicUnit->m_xpIndexDef;
  
  s4K sTableName;
  m_pDataStack->vPopStr(sTableName,__func__);
  
  pIndexDef->vTableNm(sTableName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
