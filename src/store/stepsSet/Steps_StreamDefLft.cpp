/*============================================================================*/
/* Steps_StreamDef.cpp -- StreamDef step group                                */
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
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDefLft(void)
{
  vAddStep("StreamDefLft[]"_Stz,                 &Step::vStreamDefLftGet);
  vAddStep("StreamDefLft[]\\clear"_Stz,          &Step::vStreamDefLftGetAndClear);
  vAddStep("StreamDefLft[].asSrcStreamDef"_Stz,  &Step::vStreamDefLftAsSrcStreamDef);
  vAddStep("StreamDefLft[].asStr"_Stz,           &Step::vStreamDefLftAsStr);
  vAddStep("StreamDefLft[].columnCount@"_Stz,    &Step::vStreamDefLftColumnCountFetch);
  vAddStep("StreamDefLft[].column@"_Stz,         &Step::vStreamDefLftColumnFetch);
  vAddStep("StreamDefLft[].delete"_Stz,          &Step::vStreamDefLftDelete);
  vAddStep("StreamDefLft[].duplicate"_Stz,       &Step::vStreamDefLftDuplicate);
  vAddStep("StreamDefLft[].get"_Stz,             &Step::vStreamDefLftGet);
  vAddStep("StreamDefLft[].isNull"_Stz,          &Step::vStreamDefLftIsNull);
  vAddStep("StreamDefLft[].name"_Stz,            &Step::vStreamDefLftName);
  vAddStep("StreamDefLft[].new"_Stz,             &Step::vStreamDefLftNew);
  vAddStep("StreamDefLft[].newToCsvStream"_Stz,  &Step::vStreamDefLftNewToCsv);
  vAddStep("StreamDef::parse"_Stz,               &Step::vStreamDefLftParse);
  vAddStep("StreamDefLft[].pipeIn"_Stz,          &Step::vStreamDefLftPipeIn);
  vAddStep("StreamDefLft[].print"_Stz,           &Step::vStreamDefLftPrint);
  vAddStep("StreamDefLft[].rowSize"_Stz,         &Step::vStreamDefLftRowSize);
  vAddStep("StreamDefLft[].selectColumns"_Stz,   &Step::vStreamDefLftSelectColumns);
  vAddStep("StreamDefLft[].set"_Stz,             &Step::vStreamDefLftSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].asStr -- get streamDef as a string                          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftAsStr(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = *m_pLogicUnit->m_xpStreamDefLft;
  
  sSTMT sStreamDef;
  
  pStreamDef->vAsString(sStreamDef.pString());
  
  m_pDataStack->vPushStr(sStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].asSrcStreamDef -- use this stream def as src stream def     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftAsSrcStreamDef(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = *m_pLogicUnit->m_xpStreamDefLft;

  StreamDef* pStreamDefOld = _pStreamDef(m_pLogicUnit->m_pDevLftRow->m_pStreamDef);
  if (pStreamDefOld)
    delete pStreamDefOld;

  m_pLogicUnit->m_pDevLftRow->m_pStreamDef = pStreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].column@ -- get column from streamDef                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftColumnFetch(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pLogicUnit->m_xpStreamDefLft);
                                      
  i8 iColumn = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumn));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].columnCount@ -- get column count from streamDef             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftColumnCountFetch(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pLogicUnit->m_xpStreamDefLft);
  
  m_pDataStack->vPushInt(pStreamDef->nColumns());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].delete -- delete streamDef                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpStreamDefLft);
  
  m_pLogicUnit->m_xpStreamDefLft = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].duplicate -- new a duplicate stream def from Table[]        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftDuplicate(void)
{
  ENTRY();
  
  ASSERT (* m_p_TaskLogicUnit->m_xpStreamDefLft);
  
  StreamDef* pStreamDef = new StreamDef(* m_pLogicUnit->m_xpStreamDefLft);
  
  m_pDataStack->vPushPtr(pStreamDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].get -- get streamDef                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpStreamDefLft);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].selectColumns -- generate select columns from expr          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftSelectColumns(void)
{
  ENTRY();

  StreamDef* pStreamDefIn = _pStreamDef(*m_pLogicUnit->m_xpStreamDefLft);

  s4K sExpr;
  m_pDataStack->vPopStr(sExpr, __func__);

  eEC eError = ExprEval::eGenerateSelectColumns(pStreamDefIn, sExpr, &m_pLogicUnit->m_uSelectColumns);

    m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].getAndClear -- get streamDef and set to nullptr             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpStreamDefLft);

  m_pLogicUnit->m_xpStreamDefLft = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].isNull -- is streamDef null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpStreamDefLft.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].name -- get streamDef name                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftName(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pLogicUnit->m_xpStreamDefLft);
  
  s32 sName = pStreamDef->szStreamDefNm();
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].new -- new a stream def from Table[]                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftNew(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpStreamDefLft.bIsNull());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpStreamDefLft = new StreamDef(pTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].newToCsvStream -- new a ToCsv streamDef                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftNewToCsv(void)
{
  ENTRY();

  // create a new streamDef
  StreamDef* pStreamDef = new StreamDef;

  // create an integer column
//  pStreamDef->m_xaColumn[0].vColumnNo(0);
//  pStreamDef->m_xaColumn[0].vColumnNm("row"_Stz);
//  pStreamDef->m_xaColumn[0].vColumnType(CT_VARCHAR);
//  pStreamDef->m_xaColumn[0].vDatum(DM_VCH);
//
//  pStreamDef->m_nColumns = 1;

  ASSERT(0 == pStreamDef->nColumns());
  pStreamDef->vAddColumn("row"_Stz,CT_VARCHAR,0,0);
  
  m_pLogicUnit->m_xpStreamDefLft = pStreamDef;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* streamDef::parse -- parse Create Table statement and produce a streamDef   */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftParse(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  sSTMT sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  StreamDef* pStreamDef = new StreamDef;
  
  eEC eResult = pStreamDef->eParseCmd(sCmd);
  
  if (eResult != EC_NONE) {
    
    pJob->vJobError(eResult);
    
    // delete tabledef
    delete pStreamDef;
    
    pStreamDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].pipeIn -- move pipein operand into StreamDefLft[]           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftPipeIn(void)
{
  ENTRY();
  
  Operand* pOperand = * m_pLogicUnit->m_xpPipein;

  m_pLogicUnit->m_xpPipein = nullptr;
    
  m_pLogicUnit->m_xpStreamDefLft = _pStreamDef(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].print -- print streamDef                                    */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftPrint(void)
{
  ENTRY();
    
  StreamDef* pStreamDef = _pStreamDef(*m_pLogicUnit->m_xpStreamDefLft);
  
  pStreamDef->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].rowSize -- get row size from streamDef                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftRowSize(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpStreamDefLft);
  StreamDef* pStreamDef = _pStreamDef(*m_pLogicUnit->m_xpStreamDefLft);
  
  m_pDataStack->vPushInt(Row::nRowProper(pStreamDef->nColumns()));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefLft[].set -- set streamDef                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefLftSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpStreamDefLft = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
