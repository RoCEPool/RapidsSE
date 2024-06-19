/*============================================================================*/
/* Steps_IndexDef.cpp - index definition step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

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
  vAddStep("IndexDef[].newGlobalIndex"_Stz, &Step::vIndexDefNewGlobalIndex);
  vAddStep("indexDef::parse"_Stz,     &Step::vIndexDefParse);
  vAddStep("indexDef::parseTableName"_Stz, &Step::vIndexDefParseTableName);
  vAddStep("IndexDef[].set"_Stz,      &Step::vIndexDefSet);
  vAddStep("indexDef::syntax"_Stz,    &Step::vIndexDefSyntax);
  vAddStep("IndexDef[].tableName@"_Stz, &Step::vIndexDefTableNameFetch);
  vAddStep("IndexDef[].tableName!"_Stz, &Step::vIndexDefTableNameStore);
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].addColumn -- add a column to the indexDef                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefAddColumn(void)
{
  ENTRY();

  eEC eError = EC_NONE;

  StreamDef* pStreamDef = _pStreamDef(m_oDataStack.pPopPtr(__func__));

  s256 sColumnNm;
  m_oDataStack.vPopStr(sColumnNm, __func__);

  Column* pColumn = pStreamDef->pFindColumn(sColumnNm);

  if ( ! pColumn )
    eError = EC_INVALID_COLUMN;
  else {
		TypeId xTypeId = pColumn->xColumnTypeId();

    IndexDef* pIndexDef = * m_pLogicUnit->m_xpIndexDef;

    pIndexDef->vAddColumn(sColumnNm,xTypeId);
  }

  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].delete -- delete index definition                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefDelete(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = * m_pLogicUnit->m_xpIndexDef;
  
  m_pLogicUnit->m_xpIndexDef = nullptr;
  
  delete pIndexDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].get -- get indexDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefGet(void)
{
  ENTRY();
  
  m_oDataStack.vPushPtr(*m_pLogicUnit->m_xpIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].isNull -- is indexDef null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefIsNull(void)
{
  ENTRY();
  
  m_oDataStack.vPushInt(m_pLogicUnit->m_xpIndexDef.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].name! -- set name                                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefNameStore(void)
{
  ENTRY();
  
  s256 sName;
  m_oDataStack.vPopStr(sName, __func__);
  
  m_pLogicUnit->m_xpIndexDef->vIndexNm(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].new -- new a index definition                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpIndexDef = new IndexDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].newGlobalIndex -- new a global index definition                 */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefNewGlobalIndex(void)
{
  ENTRY();

  s256 sIndexName;

  Table* pTable = _pTable(m_oDataStack.pPopPtr(__func__));
  m_oDataStack.vPopStr(sIndexName, __func__);

  i8 iKeyCol = m_oDataStack.iPopInt(__func__);

  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn = pStreamDef->pGetColumn(iKeyCol);

  IndexDef* pIndexDef = new IndexDef;

  // set the index name
  pIndexDef->vIndexNm(sIndexName);

  // set the table name
  pIndexDef->vTableNm(pTable->szTableNm());

  // set the index as a global index
  pIndexDef->vIsGlobal(true);

  // add the key column to the IndexDef
  pIndexDef->vAddColumn(pColumn->szClassNm(), pColumn->xColumnTypeId());

  m_pLogicUnit->m_xpIndexDef = pIndexDef;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::parse -- parse Create Index statement and produce a IndexDef     */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParse(void)
{
  ENTRY();
  
  Table* pTable = _pTable(m_oDataStack.pPopPtr(__func__));
  
  s4K sCmd;
  m_oDataStack.vPopStr(sCmd, __func__);
  
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  IndexDef* pIndexDef = new IndexDef;
  
  eEC eResult = pIndexDef->eParse(pTable,sCmd);
  
  if (eResult != EC_NONE) {

    // push the error on the stack
    m_oDataStack.vPushInt(eResult);

    // push false
    m_oDataStack.vPushInt(false);

    // delete IndexDef
    delete pIndexDef;
    
    pIndexDef = nullptr;
  }
  else {
    /// push the indexDef
    m_oDataStack.vPushPtr(pIndexDef);

    // push false
    m_oDataStack.vPushInt(true);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].set -- set indexDef                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpIndexDef = _pIndexDef(m_oDataStack.pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::syntax -- parse Create Index statement and check syntax          */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefSyntax(void)
{
  ENTRY();
  
  s4K sCmd;
  m_oDataStack.vPopStr(sCmd, __func__);
  
  eEC eResult = IndexDef::eSyntax(sCmd);
  
  m_oDataStack.vPushInt(eResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexDef::parseTableName -- parse Create Index statement and get tableNm   */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefParseTableName(void)
{
  ENTRY();

  s4K sCmd;
  m_oDataStack.vPopStr(sCmd, __func__);
  
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //  vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  s256 sTableNm;
  
  eEC eResult = IndexDef::eParseTableName(sCmd, sTableNm);
  
  if (eResult != EC_NONE) {
    
    // push the error on the stack
    m_oDataStack.vPushInt(eResult);

    // push false
    m_oDataStack.vPushInt(false);
  }
  else {

    // push the table name
    m_oDataStack.vPushStr(sTableNm);

    // push true
    m_oDataStack.vPushInt(true);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].tableName@ -- fetch tableNm from indexDef                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefTableNameFetch(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = *m_pLogicUnit->m_xpIndexDef;
  
  s256 sTableNm = pIndexDef->szTableNm();
  
  m_oDataStack.vPushStr(sTableNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexDef[].tableName! -- store tableNm in indexDef                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDefTableNameStore(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = *m_pLogicUnit->m_xpIndexDef;
  
  s4K sTableName;
  m_oDataStack.vPopStr(sTableName, __func__);
  
  pIndexDef->vTableNm(sTableName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
