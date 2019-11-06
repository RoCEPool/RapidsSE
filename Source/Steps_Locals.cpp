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
  ENTRY("vLocalFetch"_Stz);
  
  if ( ! m_pLocalsFrame )
    THROW_DSTACK();
  
  i8 iLocal;
  if ( ! m_pDataStack->bPopI8(iLocal) )
    THROW_DSTACK();
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->vPushLocal(pLocal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Step::vLocalStore(void)
{
  ENTRY("vLocalStore"_Stz);
  
  if ( ! m_pLocalsFrame )
    THROW_DSTACK();
  
  i8 iLocal;
  if ( ! m_pDataStack->bPopI8(iLocal) )
    THROW_DSTACK();
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->bPopLocal(pLocal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- encode code for: set of local variables              immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(void)
{
ENTRY("vLocalsBar"_Stz);
  
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
      pCurrentWord->vEncode("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(m_pCompiler->szToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vKind(WK_LOCAL);
    pWordLocal->vLocalNo(m_pCompiler->m_nLocals);
    
    // encode the word
    xVariable = m_pCompiler->m_nLocals;
    pWordLocal->vEncode("pushValue"_Stz,&xVariable);
    pWordLocal->vEncode("local.fetch"_Stz);
    pWordLocal->vEncode("exit_rt"_Stz);

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue"_Stz,&xVariable);
    pCurrentWord->vEncode("local.store"_Stz);
    
    m_pCompiler->m_nLocals++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPop(void)
{
ENTRY("vLocalsPop"_Stz);
  
  LocalChain* pLocalsFrame = m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_DSTACK();
  
  m_pLocalsFrame = m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPush(void)
{
ENTRY("vLocalsPush"_Stz);
  
  LocalChain* pLocalsFrame = new LocalChain;
  
  pLocalsFrame->vDiscardPoint(m_pReturnStack->aPeekValue());
  
  pLocalsFrame->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- encode code for: store value into local variable           immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(void)
{
ENTRY("vLocalsTo"_Stz);
  
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
      && pLocalWord->pCategory()->szWordNm()->bSame("<<metaSymbol>>"_Stz)) {
    
    Word* pCurrentWord = m_pCompiler->pCurrentWord();
    
    GenVar xMetaNm;
    xMetaNm = sLocalNm;
    
    pCurrentWord->vEncode("pushValue"_Stz,&xMetaNm);
    pCurrentWord->vEncode("swap"_Stz);

    if (bSemanticTo) {
      pCurrentWord->vEncode("job::this"_Stz);
      pCurrentWord->vEncode("job.session@"_Stz);
      pCurrentWord->vEncode("session.enameSet@"_Stz);
    }
    else {
      pCurrentWord->vEncode("job::this"_Stz);
      pCurrentWord->vEncode("job.enameSet@"_Stz);
    }
    
    pCurrentWord->vEncode("enameSet.upsertStr"_Stz);
    
    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // handle a local

  GenVar xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncode("pushValue"_Stz,&xVar_LocalNo);
  pCurrentWord->vEncode("local.store"_Stz);
  
  EXIT();
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
