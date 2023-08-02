/*============================================================================*/
/* Steps_StreamDefOut.cpp -- Stream Definition (out-going) step group         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"
#include "ExprEval_v2.hpp"

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
  vAddStep("StreamDefOut[].isNull"_Stz,     &Step::vStreamDefOutIsNull);
  vAddStep("StreamDefOut[].name@"_Stz,      &Step::vStreamDefOutNameFetch);
  vAddStep("StreamDefOut[].name!"_Stz,      &Step::vStreamDefOutNameStore);
  vAddStep("StreamDefOut[].new"_Stz,        &Step::vStreamDefOutNew);
  vAddStep("StreamDefOut[].newFromTable"_Stz,&Step::vStreamDefOutNewFromTable);
  vAddStep("StreamDefOut[].print"_Stz,      &Step::vStreamDefOutPrint);
  vAddStep("StreamDefOut[].set"_Stz,        &Step::vStreamDefOutSet);

  vAddStep("StreamDefOut[].generate_V2"_Stz,   &Step::vStreamDefOutGenerate_V2);
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
  
  ASSERT (* m_pLogicUnit->m_xpStreamDefOut);
  
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
/* StreamDefOut[].generate_V2 -- generate the stream definition out from expr */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutGenerate_V2(void)
{
  ENTRY();

  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));

  s4K sExpr;
  m_pDataStack->vPopStr(sExpr, __func__);

  if (m_pLogicUnit->m_xpStreamDefOut.bIsNull())
    m_pLogicUnit->m_xpStreamDefOut = new StreamDef;

  m_pLogicUnit->m_xpPreExpStack = new IntStack;

  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  IntStack*  pPreExpStack = * m_pLogicUnit->m_xpPreExpStack;

  eEK eExprKind;

  eEC eError = ExprEval_v2::eGenerateStreamDef(pStreamDefIn, pStreamDefOut, pIndexDef,
                                             pPreExpStack, sExpr, &m_pLogicUnit->m_uSelectColumns);

  // TODO: V2 - ALKA REMOVED m_eExprKind from LU
  //m_pLogicUnit->m_eExprKind = eExprKind;

#ifdef WITH_DEBUG_VERSION

  vTrace("*****************************************************\n"_Stz);
  vTrace("* Intermediate Representation for OP: %10s    *\n"_Stz, m_pOp->pOpSpec()->szOpNm());
  vTrace("*****************************************************\n"_Stz);

  // dump the Expression
  vTrace("--------------------------------------\n"_Stz);
  vTrace("Expr: \"%s\"\n"_Stz, sExpr.szStz());

  // dump the Expression kind
  vTrace("eEK: %s\n"_Stz, aEK[eExprKind]);

  // dump the PreExp stack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("PREEXPSTACK:\n"_Stz);
  i8 nExpr = pPreExpStack->nItems() - 1;

  while ( nExpr >= 0) {
    i8 iValue = pPreExpStack->iPeek(nExpr--);
    vTrace("%s\n"_Stz, aPE[iValue]);
  }

  // dump the select column list
  vTrace("--------------------------------------\n"_Stz);
  vTrace("SELECT COLUMNS:\n"_Stz);
  for (i8 i_Col = 0; i_Col < 64; i_Col++) {

    // skip columns in join not needed
    if (BitLib::bIsOn(m_pLogicUnit->m_uSelectColumns, i_Col)) {
      vTrace("%s\n"_Stz, pStreamDefIn->pGetColumn(i_Col)->szColNm());
    }
  }

  // dump the streamDefOut
  vTrace("--------------------------------------\n"_Stz);
  vTrace("STREAMDEFOUT:\n"_Stz);
  pStreamDefOut->vPrint();

  // dump the indexDef
  vTrace("--------------------------------------\n"_Stz);
  vTrace("INDEXDEF:\n"_Stz);
  if (pIndexDef) {
    pIndexDef->vPrint();
  }

  vTrace("\n\n\n"_Stz);

#endif


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

