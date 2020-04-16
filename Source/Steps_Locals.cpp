/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
/*============================================================================*/

#include "Steps.h"

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
  ENTRY(); // ENTRY("vLocalFetch"_Stz);
  
  if ( ! m_pLocalsFrame ) THROW_DSTACK();
  
  i8 iLocal = m_pDataStack->iPopInt(__func__);
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->vPushVal(pLocal->aAnyValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Step::vLocalStore(void)
{
  ENTRY(); // ENTRY("vLocalStore"_Stz);
  
  if ( ! m_pLocalsFrame ) THROW_DSTACK();
  
  i8 iLocal = m_pDataStack->iPopInt(__func__);
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->vPopVal(pLocal->aAnyValue(),__func__);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- encode code for: set of local variables              immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(void)
{
  ENTRY(); // ENTRY("vLocalsBar"_Stz);
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  Dic* pDic = m_oDicStack.pPick(0);

  Datum xVariable;
  
  for (;;) {
    
    // get name of local variable
    m_pTokeniser->vReadToken_nil();

    // end of locals declaration?
    if (m_pTokeniser->szToken()->bEqual("|"_Stz))
      break;

    // test for first use of locals|
    i8 nLcl = m_pCompiler->nLocals();
    if ( ! nLcl ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Stz);
      m_oDicStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncodeCall("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(m_pTokeniser->szToken());
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
    m_pCompiler->vLocals(nLcl);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPop(void)
{
  ENTRY(); // ENTRY("vLocalsPop"_Stz);
  
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
  ENTRY(); // ENTRY("vLocalsPush"_Stz);
  
  LocalChain* pLocalsFrame = new LocalChain;
  
  pLocalsFrame->vDiscardPoint(m_pReturnStack->aTopVal());
  
  pLocalsFrame->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* "to" -- encode code for: store value into local var / reg        immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(void)
{
  ENTRY(); // ENTRY("vLocalsTo"_Stz);
  
  bool bSessionLevelTag = false;
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // advance to token after to
  m_pTokeniser->vReadToken_nil();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // is is a semantic ename?
  
  Stz* szTerm = m_pTokeniser->szToken();
  
  if (szTerm->bSame("/session"_Stz)) {
    
    bSessionLevelTag = true;
    m_pTokeniser->vReadToken_nil();
  }
  else if (szTerm->bSame("AggTimer[]"_Stz)) {
    pCurrentWord->vEncodeCall("AggTimer[].set"_Stz);
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
  else if (szTerm->bSame("File0[]"_Stz)) {
    pCurrentWord->vEncodeCall("File0[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Index[]"_Stz)) {
    pCurrentWord->vEncodeCall("Index[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("IndexPtn[]"_Stz)) {
    pCurrentWord->vEncodeCall("IndexPtn[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("InfoSet[]"_Stz)) {
    pCurrentWord->vEncodeCall("InfoSet[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("InfoSetTable[]"_Stz)) {
    pCurrentWord->vEncodeCall("InfoSetTable[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Operand[]"_Stz)) {
    pCurrentWord->vEncodeCall("Operand[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("OperandB[]"_Stz)) {
    pCurrentWord->vEncodeCall("OperandB[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("OperandC[]"_Stz)) {
    pCurrentWord->vEncodeCall("OperandC[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("OperandOoO[]"_Stz)) {
    pCurrentWord->vEncodeCall("OperandOoO[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Pipein[]"_Stz)) {
    pCurrentWord->vEncodeCall("Pipein[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Register0[]"_Stz)) {
    pCurrentWord->vEncodeCall("Register0[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Row[]"_Stz)) {
    pCurrentWord->vEncodeCall("Row[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamDef[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamDef[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("StreamSetTable[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamSetTable[].set"_Stz);
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
  else if (szTerm->bSame("Timer[]"_Stz)) {
    pCurrentWord->vEncodeCall("Timer[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("WriteBehind[]"_Stz)) {
    pCurrentWord->vEncodeCall("WriteBehind[].set"_Stz);
    EXIT();
    return;
  }
  else if (szTerm->bSame("Xmits[]"_Stz)) {
    pCurrentWord->vEncodeCall("Xmits[].set"_Stz);
    EXIT();
    return;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get name of local from token stream
  s32 sLocalNm(m_pTokeniser->szToken());
  
  // attempt to find the word describing the local
  Word* pLocalWord = m_oDicStack.pFindWord(sLocalNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no such local or <metaSymbol>
  
  if (! pLocalWord && !bSessionLevelTag)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "No such local: %s\n"_Stz,sLocalNm.azChar());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle a meta-symbol
  
  if (bSessionLevelTag) {
    
    RASSERT(szTerm->bSame(C_TAG_PATH));

    Word* pCurrentWord = m_pCompiler->pCurrentWord();
    
    Datum xMetaNm;
    xMetaNm = sLocalNm;
    
    pCurrentWord->vEncodePush(xMetaNm);
    pCurrentWord->vEncodeCall("swap"_Stz);

    pCurrentWord->vEncodeCall("job[]"_Stz);
    pCurrentWord->vEncodeCall("job.session@"_Stz);
    pCurrentWord->vEncodeCall("session.enameSet@"_Stz);
    
    pCurrentWord->vEncodeCall("enameSet.upsertStr"_Stz);
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle a local

  Datum xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncodePush(xVar_LocalNo);
  pCurrentWord->vEncodeCall("local.store"_Stz);
  
  EXIT();
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
