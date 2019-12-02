/*============================================================================*/
/* Steps_StreamDef.cpp -- StreamDef step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDef(void)
{
  vAddStep("StreamDef[].get"_Stz,                &Step::vStreamDefGet);
  vAddStep("StreamDef[].newCountStream"_Stz,     &Step::vStreamDefNewCount);
  vAddStep("StreamDef[].newToCsvStream"_Stz,     &Step::vStreamDefNewToCsv);
  vAddStep("StreamDef[].pipeOut"_Stz,            &Step::vStreamDefPipeOut);
  vAddStep("StreamDef[].set"_Stz,                &Step::vStreamDefSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].get -- get streamDef                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefGet(void)
{
  ENTRY(); // ENTRY("vStreamDefGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].newCount -- new a countOp streamDef                            */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefNewCount(void)
{
  ENTRY(); // ENTRY("vStreamDefNewCountStream"_Stz);
  
  // create a new streamDef
  StreamDef* pStreamDef = new StreamDef;
  
  // create an integer column
  pStreamDef->m_xaColumn[0].vColumnNo(0);
  pStreamDef->m_xaColumn[0].vColumnNm("count"_Stz);
  pStreamDef->m_xaColumn[0].vColumnType(CT_INTEGER);
  pStreamDef->m_xaColumn[0].vDatumType(DM_INT);
  
  pStreamDef->m_nDefSets++;
  pStreamDef->m_nDefColumns[0] = 1;
  pStreamDef->m_i_DefStart[0] = 0;
  
  m_pRegSet->m_xpStreamDef = pStreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].newToCsvStream -- new a ToCsvOp streamDef                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefNewToCsv(void)
{
  ENTRY(); // ENTRY("vStreamDefNewToCsvStream"_Stz);
  
  // create a new streamDef
  StreamDef* pStreamDef = new StreamDef;
  
  // create an integer column
  pStreamDef->m_xaColumn[0].vColumnNo(0);
  pStreamDef->m_xaColumn[0].vColumnNm("row"_Stz);
  pStreamDef->m_xaColumn[0].vColumnType(CT_VARCHAR);
  pStreamDef->m_xaColumn[0].vDatumType(DM_VCH);
  
  pStreamDef->m_nDefSets++;
  pStreamDef->m_nDefColumns[0] = 1;
  pStreamDef->m_i_DefStart[0] = 0;
  
  m_pRegSet->m_xpStreamDef = pStreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].pipeOut -- append streamDef onto next operator in pipeline     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefPipeOut(void)
{
  ENTRY(); // ENTRY("vStreamDefPipeOut"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut(*m_pRegSet->m_xpStreamDef);
  
  m_pRegSet->m_xpStreamDef = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDef[].set -- set streamDef                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefSet(void)
{
  ENTRY(); // ENTRY("vStreamDefSet"_Stz);
  
  m_pRegSet->m_xpStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
