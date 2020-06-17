/*============================================================================*/
/* Steps_StreamDef.cpp -- StreamDef step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegStreamDef(void)
{
  vAddStep("StreamDef[]"_Stz,                 &Step::vRegStreamDefGet);
  vAddStep("StreamDef[].asStr"_Stz,           &Step::vRegStreamDefAsStr);
  vAddStep("StreamDef[].columnCount@"_Stz,    &Step::vRegStreamDefColumnCountFetch);
  vAddStep("StreamDef[].column@"_Stz,         &Step::vRegStreamDefColumnFetch);
  vAddStep("StreamDef[].delete"_Stz,          &Step::vRegStreamDefDelete);
  vAddStep("StreamDef[].duplicate"_Stz,       &Step::vRegStreamDefDuplicate);
  vAddStep("StreamDef[].get"_Stz,             &Step::vRegStreamDefGet);
  vAddStep("StreamDef[].isNull"_Stz,          &Step::vRegStreamDefIsNull);
  vAddStep("StreamDef[].name"_Stz,            &Step::vRegStreamDefName);
  vAddStep("StreamDef[].newCountStream"_Stz,  &Step::vRegStreamDefNewCount);
  vAddStep("StreamDef[].new"_Stz,             &Step::vRegStreamDefNew);
  vAddStep("StreamDef[].newToCsvStream"_Stz,  &Step::vRegStreamDefNewToCsv);
  vAddStep("StreamDef::parse"_Stz,            &Step::vRegStreamDefParse);
  vAddStep("StreamDef[].pipeOut"_Stz,         &Step::vRegStreamDefPipeOut);
  vAddStep("StreamDef[].rowSize"_Stz,         &Step::vRegStreamDefRowSize);
  vAddStep("StreamDef[].set"_Stz,             &Step::vRegStreamDefSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].asStr -- get streamDef as a string                             */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefAsStr(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = *m_pRegSet->m_xpStreamDef;
  
  sSTMT sStreamDef;
  
  pStreamDef->vAsString(sStreamDef.pString());
  
  m_pDataStack->vPushStr(sStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].column@ -- get column from streamDef                           */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefColumnFetch(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pRegSet->m_xpStreamDef);
                                      
  i8 iColumn = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumn));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].columnCount@ -- get column count from streamDef                */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefColumnCountFetch(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pRegSet->m_xpStreamDef);
  
  m_pDataStack->vPushInt(pStreamDef->nColumns(C_ALL));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].delete -- delete streamDef                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pRegSet->m_xpStreamDef);
  
  m_pRegSet->m_xpStreamDef = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].duplicate -- new a duplicate stream def from Table[]           */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefDuplicate(void)
{
  ENTRY();
  
  ASSERT (* m_pRegSet->m_xpStreamDef);
  
  StreamDef* pStreamDef = new StreamDef(* m_pRegSet->m_xpStreamDef);
  
  m_pDataStack->vPushPtr(pStreamDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].get -- get streamDef                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].isNull -- is streamDef null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpStreamDef.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].name -- get streamDef name                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefName(void)
{
  ENTRY();
  
  StreamDef* pStreamDef = _pStreamDef(*m_pRegSet->m_xpStreamDef);
  
  s32 sName = pStreamDef->szName();
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].new -- new a stream def from Table[]                           */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefNew(void)
{
  ENTRY();
  
  ASSERT (m_pRegSet->m_xpStreamDef.bIsNull());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  m_pRegSet->m_xpStreamDef = new StreamDef(pTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].newCount -- new a countOp streamDef                            */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefNewCount(void)
{
  ENTRY();
  
  // create a new streamDef
  StreamDef* pStreamDef = new StreamDef;
  
  // create an integer column
  pStreamDef->m_xaColumn[0].vColumnNo(0);
  pStreamDef->m_xaColumn[0].vColumnNm("count"_Stz);
  pStreamDef->m_xaColumn[0].vColumnType(CT_INTEGER);
  pStreamDef->m_xaColumn[0].vDatum(DM_INT);
  
  pStreamDef->m_nDefSets++;
  pStreamDef->m_nDefColumns[0] = 1;
  pStreamDef->m_i_DefStart[0] = 0;
  
  m_pRegSet->m_xpStreamDef = pStreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].newToCsvStream -- new a ToCsv streamDef                        */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefNewToCsv(void)
{
  ENTRY();
  
  // create a new streamDef
  StreamDef* pStreamDef = new StreamDef;
  
  // create an integer column
  pStreamDef->m_xaColumn[0].vColumnNo(0);
  pStreamDef->m_xaColumn[0].vColumnNm("row"_Stz);
  pStreamDef->m_xaColumn[0].vColumnType(CT_VARCHAR);
  pStreamDef->m_xaColumn[0].vDatum(DM_VCH);
  
  pStreamDef->m_nDefSets++;
  pStreamDef->m_nDefColumns[0] = 1;
  pStreamDef->m_i_DefStart[0] = 0;
  
  m_pRegSet->m_xpStreamDef = pStreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* streamDef::parse -- parse Create Table statement and produce a streamDef   */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefParse(void)
{
  ENTRY();
  
  Job* pJob_Local = _pJob_Local(m_pDataStack->pPopPtr(__func__));
  
  sSTMT sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  //vTrace("Stmt: %s\n"_Stz,sCmd.szStz());
  //vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  
  StreamDef* pStreamDef = new StreamDef;
  
  eEC eResult = pStreamDef->eParse(sCmd);
  
  if (eResult != EC_NONE) {
    
    // at this stage shouldn't be a syntax error
    //ASSERT(eResult != EC_INVALID_COMMAND);
    
    pJob_Local->vJobError(eResult);
    
    // delete tabledef
    delete pStreamDef;
    
    pStreamDef = nullptr;
  }
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].pipeOut -- append streamDef onto next operator in pipeline     */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpStreamDef);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpStreamDef,m_pTask);
  
  m_pRegSet->m_xpStreamDef = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].rowSize -- get row size from streamDef                         */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefRowSize(void)
{
  ENTRY();
  
  ASSERT(*m_pRegSet->m_xpStreamDef);
  StreamDef* pStreamDef = _pStreamDef(*m_pRegSet->m_xpStreamDef);
  
  m_pDataStack->vPushInt(Row::nSizeBytes(pStreamDef));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].set -- set streamDef                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegStreamDefSet(void)
{
  ENTRY();
  
  m_pRegSet->m_xpStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
