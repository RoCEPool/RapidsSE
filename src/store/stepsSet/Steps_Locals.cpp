/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "TagConstants.h"
#include "LocalVars.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Locals(void)
{
  vAddStep("locals|"_Stz,            &Step::vLocalsBar,        true);
  vAddStep("params|"_Stz,            &Step::vLocalsBar,        true);
  vAddStep("to"_Stz,                 &Step::vLocalsTo,         true);

  vAddStep("locals.framePop"_Stz,    &Step::vLocalsFramePop);
  vAddStep("locals.framePush"_Stz,   &Step::vLocalsFramePush);

  vAddStep("local.fetch"_Stz,        &Step::vLocalsFetch);
  vAddStep("local.store"_Stz,        &Step::vLocalsStore);
}


/*----------------------------------------------------------------------------*/
/* locals| -- encode code for: set of local variables              immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(void)
{
  ENTRY();
  
  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  Dic* pDic = m_oDicStack.pPick(0);

  SenCell xVariable;
  
  for (;;) {
    
    // get name of local variable
    eEC eError = EC_NONE;
    bool b = m_pInput->bReadToken(eError);
    UNUSED(b);

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
      xVariable = (i8)12345678;
      pCurrentWord->vEncodePush(xVariable);
      
      m_pSenCompiler->vLocalsMark();
      
      pCurrentWord->vEncodeCall("locals.framePush"_Stz);
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
/* "to" -- encode code for: store value into local var / reg        immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(void)
{
  ENTRY();

  bool b;
  eEC eError = EC_NONE;

  Word* pCurrentWord = m_pSenCompiler->pCurrentWord();
  
  // advance to token after to
  b = m_pInput->bReadToken(eError);
  UNUSED(b);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // is is a semantic ename?
  
  Stz* szTerm = m_pInput->szChars();
  
  if (szTerm->bSame("AggTimer[]"_Stz)) {
    pCurrentWord->vEncodeCall("AggTimer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("BufferId[]"_Stz)) {
    pCurrentWord->vEncodeCall("BufferId[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("CodeBlock[]"_Stz)) {
    pCurrentWord->vEncodeCall("CodeBlock[].set"_Stz);
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
  else if (szTerm->bSame("Dynamic[]"_Stz)) {
    pCurrentWord->vEncodeCall("Dynamic[].set"_Stz);
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
  else if (szTerm->bSame("ExitOperand[]"_Stz)) {
    pCurrentWord->vEncodeCall("ExitOperand[].set"_Stz);
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
  else if (szTerm->bSame("Fun4[]"_Stz)) {
    pCurrentWord->vEncodeCall("Fun4[].set"_Stz);
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
  else if (szTerm->bSame("LibInfo[]"_Stz)) {
    pCurrentWord->vEncodeCall("LibInfo[].set"_Stz);
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
  else if (szTerm->bSame("PdsCursor[]"_Stz)) {
    pCurrentWord->vEncodeCall("PdsCursor[].set"_Stz);
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
	else if (szTerm->bSame("RowBufferErr[]"_Stz)) {
		pCurrentWord->vEncodeCall("RowBufferErr[].set"_Stz);
		EXIT();
		return;
	}
	else if (szTerm->bSame("RowBufferWarn[]"_Stz)) {
		pCurrentWord->vEncodeCall("RowBufferWarn[].set"_Stz);
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
  else if (szTerm->bSame("StreamDefOut[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamDefOut[].set"_Stz);
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
	else if (szTerm->bSame("TableDef[]"_Stz)) {
		pCurrentWord->vEncodeCall("TableDef[].set"_Stz);
		EXIT();
		return;
	}
	else if (szTerm->bSame("TableDefWarn[]"_Stz)) {
		pCurrentWord->vEncodeCall("TableDefWarn[].set"_Stz);
		EXIT();
		return;
	}
	else if (szTerm->bSame("TableDefErr[]"_Stz)) {
		pCurrentWord->vEncodeCall("TableDefErr[].set"_Stz);
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
  else if (szTerm->bSame("Text[]"_Stz)) {
    pCurrentWord->vEncodeCall("Text[].set"_Stz);
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

  SenCell xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncodePush(xVar_LocalNo);
  pCurrentWord->vEncodeCall("local.store"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* locals.framePop -- pop locals frame                                        */
/*----------------------------------------------------------------------------*/

#ifdef WITH_LOCALS_OOM
void Step::vLocalsFramePop(void)
{
  ENTRY();
    
  RASSERT(0 == m_pReturnStack->nValues() ||
          CN_FIPVAL == m_pReturnStack->aTopVal()->eType());

  if (! m_pLocalVars)
    vSeeFrameStack();
  
  RASSERT(m_pLocalVars);
  
  void* aDiscardPoint_return;
  if (0 == m_pReturnStack->nValues())
    aDiscardPoint_return = nullptr;
  else
    aDiscardPoint_return = m_pReturnStack->aTopVal();
  
  m_pLocalVars->vFramePop(aDiscardPoint_return);

  EXIT();
}
/*----------------------------------------------------------------------------*/
#else
void Step::vLocalsFramePop(void)
{
  ENTRY();
  
  LocalFrame* pLocalsFrame = m_pLocalFrame;
  
  if ( ! pLocalsFrame ) THROW_DSTACK();
  
  m_pLocalFrame = m_pLocalFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}
#endif

/*----------------------------------------------------------------------------*/
/* locals.framePush -- push locals frame                                      */
/*----------------------------------------------------------------------------*/

#ifdef WITH_LOCALS_OOM
void Step::vLocalsFramePush(void)
{
  ENTRY();
  
  i8 nLocals = m_pDataStack->iPopInt(__func__);
  RASSERT(nLocals > 0 && nLocals <= 24);
  
  if (! m_pLocalVars)
    m_pLocalVars = new LocalVars(m_pLocalStrs);


  TypeValueArea* axDiscardPoint = m_pReturnStack->aTopVal();
  
  RASSERT(0 == m_pReturnStack->nValues() ||
          CN_FIPVAL == axDiscardPoint->eType());
  
  if (! m_pLocalVars->bFramePush(nLocals,axDiscardPoint)) {
    vSeeFrameStack();
    NEVER_REACH;
  }
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
#else
void Step::vLocalsFramePush(void)
{
  ENTRY();
  
  if (m_iDebugMark)
    vDebugPoint();

  i8 nLocals = m_pDataStack->iPopInt(__func__);
  RASSERT(nLocals > 0 && nLocals <= 24);
  
  //if (nLocals > 4)
  //  vTrace("nLocals %lld\n"_Stz,nLocals);

  LocalFrame* pLocalsFrame = new LocalFrame;
  
  pLocalsFrame->vDiscardPoint((TypeValueArea*)m_pReturnStack->aTopVal());
  
  pLocalsFrame->vNext(m_pLocalFrame);
  
  m_pLocalFrame = pLocalsFrame;
  
  EXIT();
}
#endif


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* locals.fetch -- push local[n] onto dstack                                  */
/*----------------------------------------------------------------------------*/

#ifdef WITH_LOCALS_OOM
void Step::vLocalsFetch(void)
{
  ENTRY();

  RASSERT(m_pLocalVars);
  
  //m_pLocalVars->vVerify();

  i8 i_Local = m_pDataStack->iPopInt(__func__);

  LocalVar* axLocalVar = m_pLocalVars->axLocalVar(i_Local);

//  vTrace("Pushing i_Local %lld\n"_Stz,i_Local);
//  vSeeFrameStack();
  m_pDataStack->vPushLocalVar(axLocalVar);

  //m_pLocalVars->vVerify();
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
#else
void Step::vLocalsFetch(void)
{
  ENTRY();
  
  if ( ! m_pLocalFrame ) THROW_DSTACK();
  
  i8 i_Local = m_pDataStack->iPopInt(__func__);
  
  SenCell* pSenCell = m_pLocalFrame->pSenCell(i_Local);
  
  m_pDataStack->vPushSenCell(pSenCell);
  
  EXIT();
}
#endif

/*----------------------------------------------------------------------------*/
/* localsStore -- pop dstack into local[n]                                    */
/*----------------------------------------------------------------------------*/

#ifdef WITH_LOCALS_OOM
void Step::vLocalsStore(void)
{
  ENTRY();

  RASSERT(m_pLocalVars);
  
  //m_pLocalVars->vVerify();

  i8 i_Local = m_pDataStack->iPopInt(__func__);

  LocalVar* axLocalVar = m_pLocalVars->axLocalVar(i_Local);

//  vTrace("Popping i_Local %lld\n"_Stz,i_Local);
//  vSeeFrameStack();
  m_pDataStack->vPopLocalVar(axLocalVar,__func__);

  //m_pLocalVars->vVerify();

  EXIT();
}
/*----------------------------------------------------------------------------*/
#else
void Step::vLocalsStore(void)
{
  ENTRY();
  
  //static i8 nMaxSoFar = 0;
  
  if ( ! m_pLocalFrame ) THROW_DSTACK();
  
  i8 i_Local = m_pDataStack->iPopInt(__func__);
  
  SenCell* pSenCell = m_pLocalFrame->pSenCell(i_Local);
  
  m_pDataStack->vPopSenCell(pSenCell,__func__);
  
  if (CN_STZVAL == pSenCell->eType()) {
    i8 nStzSize = pSenCell->nStzSize();
//    if (nStzSize > nMaxSoFar) {
//      nMaxSoFar = nStzSize;
//      vTrace("CN_STZVAL %lldB\n%s\n"_Stz,
//             nStzSize,
//             pSenCell->szStz());
//      vSeeFrameStack();
//    }
    
    RASSERT(nStzSize < 500);
  }
 
  EXIT();
}
#endif


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
