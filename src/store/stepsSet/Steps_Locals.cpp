/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Locals(void)
{
  vAddStep("local.fetch"_Stz,     &Step::vLocalFetch);
  vAddStep("local.store"_Stz,     &Step::vLocalStore);
  vAddStep("locals|"_Stz,         &Step::vLocalsBar,        true);
  vAddStep("locals.pop"_Stz,      &Step::vLocalsPop);
  vAddStep("locals.push"_Stz,     &Step::vLocalsPush);
  vAddStep("to"_Stz,              &Step::vLocalsTo,         true);
  
  vAddStep("params|"_Stz,         &Step::vLocalsBar,        true);
}

/*----------------------------------------------------------------------------*/
/* local.fetch -- push locals[n] onto dstack                                  */
/*----------------------------------------------------------------------------*/
void Step::vLocalFetch(void)
{
  ENTRY();
  
  if ( ! m_pLocalsFrame ) THROW_DSTACK();
  
  i8 iLocal = m_pDataStack->iPopInt(__func__);
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->vPushItem(pLocal->aTypedCell());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Step::vLocalStore(void)
{
  ENTRY();
  
  if ( ! m_pLocalsFrame ) THROW_DSTACK();
  
  i8 iLocal = m_pDataStack->iPopInt(__func__);
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->vPopItem(pLocal->aTypedCell(),__func__);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- encode code for: set of local variables              immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  Dic* pDic = m_oDicStack.pPick(0);

  CellSingle xVariable;
  
  for (;;) {
    
    // get name of local variable
    m_pInput->vReadToken();

    // end of locals declaration?
    if (m_pInput->szChars()->bEqual("|"_Stz))
      break;

    // test for first use of locals|
    i8 nLcl = m_pSenCompiler->nLocals();
    if ( ! nLcl ) {
      
      // create a locals dictionary
      pDic = new Dic("<localsDic>"_Stz);
      m_oDicStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncodeCall("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(m_pInput->szChars());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWord(WK_LOCAL);
    pWordLocal->vLocalNo(nLcl);
    
    // encode the word
    xVariable = nLcl;
    pWordLocal->vEncodePush(xVariable);
    pWordLocal->vEncodeCall("local.fetch"_Stz);
    pWordLocal->vEncodeCall("exit_rt"_Stz);

    // initialise the local from dstack
    pCurrentWord->vEncodePush(xVariable);
    pCurrentWord->vEncodeCall("local.store"_Stz);
    
    nLcl++;
    m_pSenCompiler->vLocals(nLcl);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPop(void)
{
  ENTRY();
  
  LocalChain* pLocalsFrame = m_pLocalsFrame;
  
  if ( ! pLocalsFrame ) THROW_DSTACK();
  
  m_pLocalsFrame = m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPush(void)
{
  ENTRY();
  
  LocalChain* pLocalsFrame = new LocalChain;
  
  pLocalsFrame->vDiscardPoint((TypedCell*)m_pReturnStack->aTopVal());
  
  pLocalsFrame->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* "to" -- encode code for: store value into local var / reg        immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(void)
{
  ENTRY();
    
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  // advance to token after to
  m_pInput->vReadToken();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // is is a semantic ename?
  
  Stz* szTerm = m_pInput->szChars();
  
  if (szTerm->bSame("AggTimer[]"_Stz)) {
    pCurrentWord->vEncodeCall("AggTimer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("BktMap[]"_Stz)) {
    pCurrentWord->vEncodeCall("BktMap[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("BucketGet[]"_Stz)) {
    pCurrentWord->vEncodeCall("BucketGet[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("CounterA[]"_Stz)) {
    pCurrentWord->vEncodeCall("CounterA[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter0[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter0[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter1[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter1[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter2[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter2[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter3[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter3[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter4[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter4[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter5[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter5[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter6[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter6[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Counter7[]"_Stz)) {
    pCurrentWord->vEncodeCall("Counter7[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("CrwBuffer[]"_Stz)) {
    pCurrentWord->vEncodeCall("CrwBuffer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("CsvBuffer[]"_Stz)) {
    pCurrentWord->vEncodeCall("CsvBuffer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("DataBuffer[]"_Stz)) {
    pCurrentWord->vEncodeCall("DataBuffer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Deck[]"_Stz)) {
    pCurrentWord->vEncodeCall("Deck[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("ErrorCode[]"_Stz)) {
    pCurrentWord->vEncodeCall("ErrorCode[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("ExprEval[]"_Stz)) {
    pCurrentWord->vEncodeCall("ExprEval[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("ErrorMarker[]"_Stz)) {
    pCurrentWord->vEncodeCall("ErrorMarker[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("File0[]"_Stz)) {
    pCurrentWord->vEncodeCall("File0[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Fun0[]"_Stz)) {
    pCurrentWord->vEncodeCall("Fun0[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Fun1[]"_Stz)) {
    pCurrentWord->vEncodeCall("Fun1[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Fun2[]"_Stz)) {
    pCurrentWord->vEncodeCall("Fun2[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Fun3[]"_Stz)) {
    pCurrentWord->vEncodeCall("Fun3[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Grid[]"_Stz)) {
    pCurrentWord->vEncodeCall("Grid[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Index[]"_Stz)) {
    pCurrentWord->vEncodeCall("Index[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("IndexDef[]"_Stz)) {
    pCurrentWord->vEncodeCall("IndexDef[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("IndexDefRlu[]"_Stz)) {
    pCurrentWord->vEncodeCall("IndexDefRlu[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("IndexStream[]"_Stz)) {
    pCurrentWord->vEncodeCall("IndexStream[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("IndexTreeRlu[]"_Stz)) {
    pCurrentWord->vEncodeCall("IndexTreeRlu[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("InfoSet[]"_Stz)) {
    pCurrentWord->vEncodeCall("InfoSet[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("InfoSetRow[]"_Stz)) {
    pCurrentWord->vEncodeCall("InfoSetRow[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("InfoSetTable[]"_Stz)) {
    pCurrentWord->vEncodeCall("InfoSetTable[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Marker[]"_Stz)) {
    pCurrentWord->vEncodeCall("Marker[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("MemoryHog[]"_Stz)) {
    pCurrentWord->vEncodeCall("MemoryHog[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("MetaGet[]"_Stz)) {
    pCurrentWord->vEncodeCall("MetaGet[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("NamedBuffer[]"_Stz)) {
    pCurrentWord->vEncodeCall("NamedBuffer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Node[]"_Stz)) {
    pCurrentWord->vEncodeCall("Node[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Operand[]"_Stz)) {
    pCurrentWord->vEncodeCall("Operand[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("OperandOoO[]"_Stz)) {
    pCurrentWord->vEncodeCall("OperandOoO[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("PipeIn[]"_Stz)) {
    pCurrentWord->vEncodeCall("PipeIn[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("PathIdentifier[]"_Stz)) {
    pCurrentWord->vEncodeCall("PathIdentifier[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Parcel[]"_Stz)) {
    pCurrentWord->vEncodeCall("Parcel[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Pipeline[]"_Stz)) {
    pCurrentWord->vEncodeCall("Pipeline[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("PipeSpec[]"_Stz)) {
    pCurrentWord->vEncodeCall("PipeSpec[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("PlanOperand[]"_Stz)) {
    pCurrentWord->vEncodeCall("PlanOperand[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Pot[]"_Stz)) {
    pCurrentWord->vEncodeCall("Pot[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("PtnStream[]"_Stz)) {
    pCurrentWord->vEncodeCall("PtnStream[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("RecvdObject[]"_Stz)) {
    pCurrentWord->vEncodeCall("RecvdObject[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("RowOnce[]"_Stz)) {
    pCurrentWord->vEncodeCall("RowOnce[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("RowBuffer[]"_Stz)) {
    pCurrentWord->vEncodeCall("RowBuffer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("SendNode[]"_Stz)) {
    pCurrentWord->vEncodeCall("SendNode[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Sends[]"_Stz)) {
    pCurrentWord->vEncodeCall("Sends[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("PtnDef[]"_Stz)) {
    pCurrentWord->vEncodeCall("PtnDef[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Socket[]"_Stz)) {
    pCurrentWord->vEncodeCall("Socket[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Stage[]"_Stz)) {
    pCurrentWord->vEncodeCall("Stage[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Stats[]"_Stz)) {
    pCurrentWord->vEncodeCall("Stats[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Stream[]"_Stz)) {
    pCurrentWord->vEncodeCall("Stream[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamDefLft[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamDefLft[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamDefRht[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamDefRht[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamSet[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamSet[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamDefOut[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamDefOut[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamSetTable[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamSetTable[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StringA[]"_Stz)) {
    pCurrentWord->vEncodeCall("StringA[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("SubPipe[]"_Stz)) {
    pCurrentWord->vEncodeCall("SubPipe[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Subset[]"_Stz)) {
    pCurrentWord->vEncodeCall("Subset[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Table[]"_Stz)) {
    pCurrentWord->vEncodeCall("Table[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("TablePtn[]"_Stz)) {
    pCurrentWord->vEncodeCall("TablePtn[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("TagSet[]"_Stz)) {
    pCurrentWord->vEncodeCall("TagSet[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Timer[]"_Stz)) {
    pCurrentWord->vEncodeCall("Timer[].set"_Stz);
    EXIT();
    return;
  }
  
  else if (szTerm->bSame("Vertex[]"_Stz)) {
    pCurrentWord->vEncodeCall("Vertex[].set"_Stz);
    EXIT();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get name of local from token stream
  s32 sLocalNm(m_pInput->szChars());
  
  // attempt to find the word describing the local
  Word* pLocalWord = m_oDicStack.pFindWord(sLocalNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no such local or <metaSymbol>
  
  if (! pLocalWord)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "No such local: %s\n"_Stz,sLocalNm.azChar());
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle a local

  CellSingle xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncodePush(xVar_LocalNo);
  pCurrentWord->vEncodeCall("local.store"_Stz);
  
  EXIT();
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
