/*============================================================================*/
/* Steps_StreamDefOut.cpp -- Stream Definition (out-going) step group         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDefOut(void)
{
  vAddStep("StreamDefOut[]"_Stz,            &Step::vStreamDefOutGet);
  vAddStep("StreamDefOut[].addColumn"_Stz,  &Step::vStreamDefOutAddColumn);
  vAddStep("StreamDefOut[].delete"_Stz,     &Step::vStreamDefOutDelete);
  vAddStep("StreamDefOut[].duplicate"_Stz,  &Step::vStreamDefOutDuplicate);
  vAddStep("StreamDefOut[].generate"_Stz,   &Step::vStreamDefOutGenerate);
  vAddStep("StreamDefOut[].isNull"_Stz,     &Step::vStreamDefOutIsNull);
  vAddStep("StreamDefOut[].join"_Stz,       &Step::vStreamDefOutJoin);
  vAddStep("StreamDefOut[].name@"_Stz,      &Step::vStreamDefOutNameFetch);
  vAddStep("StreamDefOut[].name!"_Stz,      &Step::vStreamDefOutNameStore);
  vAddStep("StreamDefOut[].new"_Stz,        &Step::vStreamDefOutNew);
  vAddStep("StreamDefOut[].newFromTable"_Stz,&Step::vStreamDefOutNewFromTable);
  vAddStep("StreamDefOut[].print"_Stz,      &Step::vStreamDefOutPrint);
  vAddStep("StreamDefOut[].set"_Stz,        &Step::vStreamDefOutSet);
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].addColumn -- add a column                                   */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutAddColumn(void)
{
  ENTRY();
    
  s32 sColNm;
  m_pDataStack->vPopStr(sColNm,__func__);

  eCT eColType = (eCT)m_pDataStack->iPopInt(__func__);
  i8 eColLength = (eCT)m_pDataStack->iPopInt(__func__);
  i8 eColScale = (eCT)m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  
  pStreamDefOut->vAddColumn(sColNm,eColType,eColLength,eColScale);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].delete -- delete stream definition out                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDelete(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  
  m_pLogicUnit->m_xpStreamDefOut = nullptr;

  if (pStreamDefOut->eGetClassNo() == CN_STREAMDEF)
    delete pStreamDefOut;
  else {
    StreamDefRht* pStreamDefRht = _pStreamDefRht(pStreamDefOut);
    delete pStreamDefRht;
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].duplicate -- new a duplicate stream def                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDuplicate(void)
{
  ENTRY();
  
  ASSERT (* m_p_TaskLogicUnit->m_xpStreamDefOut);
  
  StreamDef* pStreamDef = new StreamDef(* m_pLogicUnit->m_xpStreamDefOut);
  
  m_pDataStack->vPushPtr(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[] -- get stream definition out                                */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpStreamDefOut);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].generate -- generate the stream definition out from expr    */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutGenerate(void)
{
  ENTRY();

  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));

  s4K sExpr;
  m_pDataStack->vPopStr(sExpr, __func__);

  m_pLogicUnit->m_xpStreamDefOut = new StreamDef;
  //m_p_TaskLogicUnit->m_xpIndexDef     = new IndexDef;
  m_pLogicUnit->m_xpAggTypeStack = new IntStack;

  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  IntStack*  pAggTypeStack = * m_pLogicUnit->m_xpAggTypeStack;

  ePJ eProjectKind;

  eEC eError = ExprEval::eGenerateStreamDef(pStreamDefIn, pStreamDefOut, pIndexDef,
                                            pAggTypeStack, eProjectKind, sExpr, &m_pLogicUnit->m_uSelectColumns);
  m_pLogicUnit->m_eProjectKind = eProjectKind;

  //pStreamDefOut->vPrint();

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].isNull -- is streamDef null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpStreamDefOut.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].join -- generate the stream definition out for join         */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutJoin(void)
{
  ENTRY();

  StreamDef* pStreamDefRht = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefLft = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpStreamDefOut = new StreamDef;

  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;

  eEC eError = ExprEval::eJoinStreamDef(pStreamDefLft, pStreamDefRht, pStreamDefOut);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].name@ -- get name                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNameFetch(void)
{
  ENTRY();
    
  s32 sName = m_pLogicUnit->m_xpStreamDefOut->szStreamDefNm();
  
  m_pDataStack->vPushStz(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].name! -- set name                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNameStore(void)
{
  ENTRY();
    
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);

  m_pLogicUnit->m_xpStreamDefOut->vStreamDefNm(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].new -- new a stream definition out                          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNew(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpStreamDefOut = new StreamDef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].newFromTable -- new a stream definition out from table      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNewFromTable(void)
{
  ENTRY();

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpStreamDefOut = new StreamDef(pTable->pStreamDef());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].print -- print streamDefOut                                 */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutPrint(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = _pStreamDef(*m_pLogicUnit->m_xpStreamDefOut);
  
  pStreamDefOut->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].set -- set streamDefOut                                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

