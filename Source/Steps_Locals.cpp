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

  GenVar xVariable;
  
  for (;;) {
    
    // get name of local variable
    m_pCompiler->vNextToken();
    
    // end of locals declaration?
    if (m_pCompiler->szToken()->bEqual("|"_Stz))
      break;

    // test for first use of locals|
    if ( ! m_pCompiler->m_nLocals ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Stz);
      m_oDicStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncodeCall("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(m_pCompiler->szToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vKind(WK_LOCAL);
    pWordLocal->vLocalNo(m_pCompiler->m_nLocals);
    
    // encode the word
    xVariable = m_pCompiler->m_nLocals;
    pWordLocal->vEncodePush(xVariable);
    pWordLocal->vEncodeCall("local.fetch"_Stz);
    pWordLocal->vEncodeCall("exit_rt"_Stz);

    // initialise the local from dstack
    pCurrentWord->vEncodePush(xVariable);
    pCurrentWord->vEncodeCall("local.store"_Stz);
    
    m_pCompiler->m_nLocals++;
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
/* to -- encode code for: store value into local variable           immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(void)
{
  ENTRY(); // ENTRY("vLocalsTo"_Stz);
  
  bool bSemanticTo = false;
  
  Word* pCurrentWord = m_pCompiler->pCurrentWord();
  
  // advance to token after "to"
  m_pCompiler->vNextToken();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // is is a semantic ename?
  
  if (m_pCompiler->szToken()->bSame("/semantic"_Stz) ||
      m_pCompiler->szToken()->bSame("/session"_Stz)) {
    
    bSemanticTo = true;
    m_pCompiler->vNextToken();
  }
  else if (m_pCompiler->szToken()->bSame("File0[]"_Stz)) {
    pCurrentWord->vEncodeCall("File0[].set"_Stz);
    EXIT();
    return;
  }
  else if (m_pCompiler->szToken()->bSame("Operand[]"_Stz)) {
    pCurrentWord->vEncodeCall("Operand[].set"_Stz);
    EXIT();
    return;
  }
  else if (m_pCompiler->szToken()->bSame("StreamDef[]"_Stz)) {
    pCurrentWord->vEncodeCall("StreamDef[].set"_Stz);
    EXIT();
    return;
  }
  else if (m_pCompiler->szToken()->bSame("Subset[]"_Stz)) {
    pCurrentWord->vEncodeCall("Subset[].set"_Stz);
    EXIT();
    return;
  }
  else if (m_pCompiler->szToken()->bSame("Table[]"_Stz)) {
    pCurrentWord->vEncodeCall("Table[].set"_Stz);
    EXIT();
    return;
  }
  else if (m_pCompiler->szToken()->bSame("TablePtn[]"_Stz)) {
    pCurrentWord->vEncodeCall("TablePtn[].set"_Stz);
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get name of local from token stream
  s32 sLocalNm(m_pCompiler->szToken());
  
  // attempt to find the word describing the local
  Word* pLocalWord = m_oDicStack.pFindWord(sLocalNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no such local or <metaSymbol>
  
  if (! pLocalWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "No such local: %s\n"_Stz,sLocalNm.azChar());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle a meta-symbol
  
  if (pLocalWord->pCategory()
      && pLocalWord->pCategory()->szWordNm()->bSame("<<jobArgTag>>"_Stz)) {
    
    Word* pCurrentWord = m_pCompiler->pCurrentWord();
    
    GenVar xMetaNm;
    xMetaNm = sLocalNm;
    
    pCurrentWord->vEncodePush(xMetaNm);
    pCurrentWord->vEncodeCall("swap"_Stz);

    if (bSemanticTo) {
      pCurrentWord->vEncodeCall("job::this"_Stz);
      pCurrentWord->vEncodeCall("job.session@"_Stz);
      pCurrentWord->vEncodeCall("session.enameSet@"_Stz);
    }
    else {
      pCurrentWord->vEncodeCall("job::this"_Stz);
      pCurrentWord->vEncodeCall("job.enameSet@"_Stz);
    }
    
    pCurrentWord->vEncodeCall("enameSet.upsertStr"_Stz);
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle a local

  GenVar xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncodePush(xVar_LocalNo);
  pCurrentWord->vEncodeCall("local.store"_Stz);
  
  EXIT();
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
