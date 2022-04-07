/*============================================================================*/
/* Steps_StreamDef.cpp -- StreamDef step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"
#include "ExprEval2.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"
#include "StreamDefRht.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDefRht(void)
{
  vAddStep("StreamDefRht[]"_Stz,                 &Step::vStreamDefRhtGet);
  vAddStep("StreamDefRht[]\\clear"_Stz,          &Step::vStreamDefRhtGetAndClear);
  vAddStep("StreamDefRht[].delete"_Stz,          &Step::vStreamDefRhtDelete);
  vAddStep("StreamDefRht[].duplicate"_Stz,       &Step::vStreamDefRhtDuplicate);
  vAddStep("StreamDefRht[].get"_Stz,             &Step::vStreamDefRhtGet);
  vAddStep("StreamDefRht[].isNull"_Stz,          &Step::vStreamDefRhtIsNull);
  vAddStep("StreamDefRht[].new"_Stz,             &Step::vStreamDefRhtNew);
  vAddStep("StreamDefRht[].newFromTable"_Stz,    &Step::vStreamDefRhtNewFromTable);
  vAddStep("StreamDefRht[].print"_Stz,           &Step::vStreamDefRhtPrint);
  vAddStep("StreamDefRht[].selectColumns"_Stz,   &Step::vStreamDefRhtSelectColumns);
  vAddStep("StreamDefRht[].set"_Stz,             &Step::vStreamDefRhtSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].delete -- delete streamDef                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpStreamDefRht);
  
  m_pLogicUnit->m_xpStreamDefRht = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].duplicate -- new a duplicate streamDefRht                   */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtDuplicate(void)
{
  ENTRY();

  ASSERT (* m_pLogicUnit->m_xpStreamDefRht);

  StreamDefRht* pStreamDefRht = new StreamDefRht(* m_pLogicUnit->m_xpStreamDefRht);

  m_pDataStack->vPushPtr(pStreamDefRht);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].get -- get streamDef                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpStreamDefRht);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].getAndClear -- get streamDef and set to nullptr             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpStreamDefRht);

  m_pLogicUnit->m_xpStreamDefRht = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].isNull -- is streamDef null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpStreamDefRht.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].new -- new a stream def from another streamDef              */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtNew(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpStreamDefRht.bIsNull());

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpStreamDefRht = new StreamDefRht(pStreamDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].newFromTable -- new a stream definition from table          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtNewFromTable(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpStreamDefRht.bIsNull());

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpStreamDefRht = new StreamDefRht(pTable->pStreamDef());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].print -- print streamDef                                    */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtPrint(void)
{
  ENTRY();
    
  StreamDefRht* pStreamDefRht = _pStreamDefRht(*m_pLogicUnit->m_xpStreamDefRht);

  pStreamDefRht->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].selectColumns -- generate select columns from expr          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtSelectColumns(void)
{
  ENTRY();

  StreamDef* pStreamDefIn = _pStreamDef(*m_pLogicUnit->m_xpStreamDefRht);

  s4K sExpr;
  m_pDataStack->vPopStr(sExpr, __func__);

  eEC eError = ExprEval2::eGenerateSelectColumns(pStreamDefIn, sExpr, &m_pLogicUnit->m_uSelectColumns);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefRht[].set -- set streamDef                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefRhtSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpStreamDefRht = _pStreamDefRht(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
