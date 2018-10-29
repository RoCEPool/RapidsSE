/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Word(void)
{
  vAddStep("allot"_Stz,           &Task::vAllot);
  vAddStep("cell"_Stz,            &Task::vCell);
  vAddStep(":"_Stz,               &Task::vColon);
  vAddStep(","_Stz,               &Task::vComma);
  vAddStep("compile,"_Stz,        &Task::vCompileComma);
  vAddStep("create"_Stz,          &Task::vCreate);
  vAddStep("createTenants"_Stz,   &Task::vCreateTenants);
  vAddStep(">create"_Stz,         &Task::vCreateFrom);
  vAddStep(">createInMain"_Stz,   &Task::vCreateFromInMain);
  vAddStep(">createPersistent"_Stz, &Task::vCreateFromPersistent);
  vAddStep("createPersistent"_Stz,&Task::vCreatePersistent);
  vAddStep("dic?"_Stz,            &Task::vDicWhich);
  vAddStep("does>"_Stz,           &Task::vDoes,             true );
  vAddStep("does_rt"_Stz,         &Task::vDoes_RT);
  vAddStep("find"_Stz,            &Task::vFind);
  vAddStep("findInCurrent"_Stz,   &Task::vFindInCurrent);
  vAddStep("forget"_Stz,          &Task::vForget);
  vAddStep("##:"_Stz,             &Task::vHashHashColon,    true );
  vAddStep("immediate"_Stz,       &Task::vImmediate,        true );
  vAddStep("locate"_Stz,          &Task::vLocate);

  vAddStep("opDic"_Stz,           &Task::vOpDic,            true );
  vAddStep("planDic"_Stz,         &Task::vPlanDicTask,      true );
  vAddStep("postpone"_Stz,        &Task::vPostpone,         true );
  vAddStep("see"_Stz,             &Task::vSee);
  vAddStep(";"_Stz,               &Task::vSemicolon,        true );
  vAddStep("subDic"_Stz,          &Task::vSubDic,           true );
  vAddStep("tick"_Stz,            &Task::vTick,             true );
  vAddStep("word.dump"_Stz,       &Task::vWordDump);
  vAddStep("word.forget"_Stz,     &Task::vWordForget);
  vAddStep("word.immediate@"_Stz, &Task::vWordImmediateGet);
  vAddStep("word.immediate!"_Stz, &Task::vWordImmediateSet);
  vAddStep("word.isImmediate"_Stz,&Task::vWordIsImmediate);
  vAddStep("word.isPrimitive"_Stz,&Task::vWordIsPrimitive);
  vAddStep("word.kind@"_Stz,      &Task::vWordKindGet);
  vAddStep("word.kind!"_Stz,      &Task::vWordKindSet);
  vAddStep("word.name"_Stz,       &Task::vWordName);
  vAddStep("word.prev"_Stz,       &Task::vWordPrev);
  vAddStep("word.this"_Stz,       &Task::vWordThis);
  vAddStep("word.type@"_Stz,      &Task::vWordTypeGet);
  vAddStep("word.type!"_Stz,      &Task::vWordTypeSet);
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(MP_Task mpTask)
{
  ENTRY("vAllot"_Stz);
  
  i8 iBytes;
  
  if ( !  mpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot"_Stz);
  
  RASSERT(iBytes);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  char* aAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(aAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of cell                                                  */
/*----------------------------------------------------------------------------*/
void Task::vCell(MP_Task mpTask)
{
  ENTRY("vCell"_Stz);
  
  i8 nBytes = sizeof(void*);
  
  mpTask->m_pDataStack->vPushInt(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(MP_Task mpTask)
{
  ENTRY("vColon"_Stz);
  
  s32 sName;
  
  mpTask->m_pCompiler->m_bCompiling = true;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  sName = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(MP_Task mpTask)
{
  ENTRY("vComma"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("comma"_Stz);
  
  pCurrentWord->vEncode("pushValue"_Stz,&xValWrk);
  
  // I replaced this with POSTPONE exit where needed -ray
  //pCurrentWord->vEncode("exit"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Task::vCompileComma(MP_Task mpTask)
{
  ENTRY("vCompileComma"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,"_Stz);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(MP_Task mpTask)
{
ENTRY("vCreate"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());

  Word* pNewWord = new(pDic) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createPersistent -- create word in dictionary (persistent heap)            */
/*----------------------------------------------------------------------------*/
void Task::vCreatePersistent(MP_Task mpTask)
{
  ENTRY("vCreate"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  Word* pNewWord = new(pDic,true) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(MP_Task mpTask)
{
ENTRY("vCreateFrom"_Stz);

  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  if (! mpTask->m_pCompiler)
    mpTask->m_pCompiler = new Compiler;
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createPersistent -- create word in dic using string on stack (persistent) */
/*----------------------------------------------------------------------------*/
void Task::vCreateFromPersistent(MP_Task mpTask)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic,true) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >createInMain -- create word in main dictionary using string on stack      */
/*----------------------------------------------------------------------------*/
void Task::vCreateFromInMain(MP_Task mpTask)
{
  ENTRY("vCreateFrom"_Stz);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Stz);
  
  Dic* pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* createTenants -- create tenant word in dictionary                          */
/*----------------------------------------------------------------------------*/
void Task::vCreateTenants(MP_Task mpTask)
{
  ENTRY("vCreateTenant"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
//  Dic* pDic;
//  if (mpTask->m_xPathStack.iCount())
//    pDic = mpTask->m_xPathStack.pTop();
//  else
//    pDic = &gpAttachMap->m_xMainDic;
  
  //vTrace(": %s\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  Word* pNewWord = new(nullptr) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  mpTask->m_pCompiler->m_pCurrentWord = pNewWord;
  
  gpAttachMap->m_pTenants = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic? -- test & push which sub dic a colon defn is to be compiled into      */
/*----------------------------------------------------------------------------*/
void Task::vDicWhich(MP_Task mpTask)
{
  ENTRY("vDicWhich"_Stz);
  
  // get current plan
  Plan* pPlan = mpTask->m_pPlan;
  if ( ! pPlan )
    THROW("dic?: no current plan in task"_Stz);
  
  // sub dic is either latest op dic (if there is one), or the plan dic
  Dic* pSubDic;
  if (pPlan->pRegFrameTop())
    pSubDic = pPlan->pRegFrameTop()->pOpDic();
  else
    pSubDic = pPlan->pDic();
  
  // push the sub dic onto the path stack
  mpTask->m_xPathStack.vPush(pSubDic);
  
  // by incrementing m_nSubDic we are indicating to the ; word that
  // there is another temporary sub dic on the path stack
  // which needs to be discarded
  mpTask->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(MP_Task mpTask)
{
ENTRY("vDoes"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  pCurrentWord->vEncode("does_rt"_Stz);
  pCurrentWord->vEncode("exit"_Stz);
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Task::vDoes_RT(MP_Task mpTask)
{
  ENTRY("vDoes_RT"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  pCurrentWord->vEntryStep(&Task::vEnterObject);
  pCurrentWord->vEntryCell(mpTask->IP.m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(MP_Task mpTask)
{
ENTRY("vFind"_Stz);
  
  s256 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("find"_Stz);
 
  if (mpTask->m_iTracing) {
    i8 i = 0;
  }
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(sName);
  
  mpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find word in current dictionary                           */
/*----------------------------------------------------------------------------*/
void Task::vFindInCurrent(MP_Task mpTask)
{
  ENTRY("vFindInCurrent"_Stz);
  
  s256 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent"_Stz);
  
  if (mpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = mpTask->m_xPathStack.pFindInCurrent(sName);
  
  mpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Task::vForget(MP_Task mpTask)
{
ENTRY("vForget"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  Word* pRemovedWord = mpTask->m_xPathStack.pRemoveWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ##: -- encode paste of abstract colon defn into plan lambda text immediate */
/*----------------------------------------------------------------------------*/
void Task::vHashHashColon(MP_Task mpTask)
{
  ENTRY("vHashHashColon"_Stz);
  
  ValWrk xWsp;
  Cell* pOpFrameJumpArg = nullptr;
  Cell* pToColonJumpArg = nullptr;

  // get word currently being compiled
  Word* pW = mpTask->m_pCompiler->m_pCurrentWord;
  
  // get the name of the symbolic argument
  s32 sSymbolicArg;
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  sSymbolicArg = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  // encode what to execute at runtime
  
  // at runtime the dstack will be ( plan value )
  
                                  // at runtime we synthesise by:
  pW->vEncode("dup"_Stz);             //   ( value plan plan )
  pW->vEncode("-rot"_Stz);            //   ( plan value plan )
  
  pW->vEncode("plan.opFrame0"_Stz);   //   get top opframe of plan
  pW->vEncode("dup"_Stz);             //   dup
  pW->vEncode("jumpTrue"_Stz);        //   forward jump if we have an opFrame
  pOpFrameJumpArg = (Cell*)pW->pAppend((Cell*)nullptr);
  
  // we dont have an opFrame, only a plan
  pW->vEncode("drop"_Stz);            //   drop the null opFrame pointer
  xWsp = "planDic "_Stz;
  pW->vEncode("pushValue"_Stz,&xWsp); //   push "planDic "

  pW->vEncode("jump"_Stz);            //   forward jump to colon etc
  pToColonJumpArg = (Cell*)pW->pAppend((Cell*)nullptr);

  // we have an opFrame
  pOpFrameJumpArg->m_pJumpTo = pW->pLastCell();
  
  pW->vEncode("opFrame.name"_Stz);    //   get name of opframe
  xWsp = "opDic "_Stz;
  pW->vEncode("pushValue"_Stz,&xWsp); //   push "opDic "
  pW->vEncode("str.prepend"_Stz);     //   form "opDic <opDicNm>"
  
  // we have an opFrame
  pToColonJumpArg->m_pJumpTo = pW->pLastCell();
  
  xWsp = " : "_Stz;
  pW->vEncode("pushValue"_Stz,&xWsp); //   push " : "
  pW->vEncode("str.cat"_Stz);         //   form "opDic <opDicNm> : "
  
  xWsp = sSymbolicArg;
  pW->vEncode("pushValue"_Stz,&xWsp); //   push "<symNm>"
  pW->vEncode("str.cat"_Stz);         //   form "opDic <opDicNm> : <symNm>"
  
  xWsp = " %V ; "_Stz;
  pW->vEncode("pushValue"_Stz,&xWsp); //   push " %V ; "
  pW->vEncode("str.cat"_Stz);         //   form "opDic <opDicNm> : <symNm> %V ; "
  
  pW->vEncode("str.format"_Stz);      //   replace %V to yield concrete defn
  
  // at runtime this should produce a code text of the form:
  //      "planDic : <symNm> <value> ; "
  //   or
  //      "opDic <opDicNm> : <symNm> <value> ; "
  
  pW->vEncode("swap"_Stz);               //   dstack is ( codeText plan )
  pW->vEncode("plan.lambda+"_Stz);       //   append to lambda text
  
  // at runtime the dstack will be ( )
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(MP_Task mpTask)
{
  ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Stz,pCurrentWord->sName().aChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(MP_Task mpTask)
{
  ENTRY("vLocate"_Stz);
  
  i8 iAddrAsInt;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAddrAsInt) )
    THROW_INVALID_DSTACK("locate"_Stz);
  
  void* pCellVoid = (void*)iAddrAsInt;
  Cell* pCell = _pCell(pCellVoid);
  
  Word* pLocatedWord = mpTask->m_xPathStack.pLocate(pCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    mpTask->m_pDataStack->vPushS32(sName);
  }
  else {
    i8 iZero = 0;
    mpTask->m_pDataStack->vPushInt(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opDic -- search for named op frame and push op dictionary onto path stack  */
/*----------------------------------------------------------------------------*/
void Task::vOpDic(MP_Task mpTask)
{
  ENTRY("vOpDic"_Stz);
  
  Dic* pOpDic = nullptr;

  s32 sNameOpFrame;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  sNameOpFrame = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  RegFrame* pOpFrame = mpTask->m_pPlan->pRegFrameFind(sNameOpFrame);
  //if ( ! pOpFrame )
  //  THROW("No such op frame: %s",sNameOpFrame.aChar());
  
  if ( pOpFrame ) {
    pOpDic = pOpFrame->pOpDic();
  }
  else {
    // create the frame
    pOpFrame = new RegFrame;
    pOpFrame->vName(sNameOpFrame);
    mpTask->m_pPlan->vRegFramePush(pOpFrame);
    
    // create a op dictionary
    s32 sNameDic;
    sNameDic.vSprint("<%s>"_Stz,sNameOpFrame.aChar());
    
    pOpDic = new Dic;
    pOpDic->vName(sNameDic);
    pOpFrame->vOpDic(pOpDic);
  }
  
  mpTask->m_xPathStack.vPush(pOpDic);
  
  mpTask->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planDic -- push plan dictionary onto path stack                            */
/*----------------------------------------------------------------------------*/
void Task::vPlanDicTask(MP_Task mpTask)
{
  ENTRY("vPlanDicTask"_Stz);
  
  Dic* pPlanDic = mpTask->m_pPlan->pDic();
  
  mpTask->m_xPathStack.vPush(pPlanDic);
  
  mpTask->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Task::vPostpone(MP_Task mpTask)
{
  ENTRY("vPostpone"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // get name of word to be called
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s "_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  if (pCalledWord->bImmediate()) {
    
    // put call to immediate word into this current definition
    
    // get indirect pointer to entry step of called word
    TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode(ppCalledEntryStep);
  }
  else {
  
    // put call to normal word into some future definition
    
    // copy indirect pointer into the current word
    pCurrentWord->vEncode("pushValue"_Stz,pCalledWord);
    if (WT_PRIMITIVE == pCalledWord->eWordType())
      pCurrentWord->vEncode("encodeCallPrimitive"_Stz);
    else
      pCurrentWord->vEncode("encodeCallRoutine"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSee(MP_Task mpTask)
{
  ENTRY("vSee"_Stz);
  
  s48K sWorkspace;
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
    return;
  }
  
  vTrace("See: %s\n"_Stz,pWord->sName().aChar());
  
  Cell* pDoesCell = pWord->pEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)pDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (pDoesCell < pDoesWord->pLastCell()) {
    
    Word* pCallsWord = mpTask->m_xPathStack.pFindStep(*pDoesCell->m_ppEntryStep);
    
    // jumps
    if (*pDoesCell->m_ppEntryStep == Task::vFlowJump ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpFalse ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Stz,pDoesCell,pCallsWord->sName().aChar());
      
      pDoesCell++;
      
      vTrace(" %p\n"_Stz,pDoesCell->m_pJumpTo);
      
      pDoesCell++;
      
      continue;
    }
    
    // push
    if ( *pDoesCell->m_ppEntryStep == Task::vPushValue ) {
      
      vTrace("%p: %s"_Stz,pDoesCell,pCallsWord->sName().aChar());
      
      pDoesCell++;
      
      ValWrk* pValue = reinterpret_cast<ValWrk*>(pDoesCell);
      
      if (CN_STRVAL == pValue->eClass()) {
        vTrace(" \"%s\"\n"_Stz,_pStrVal(pValue)->aChar());
      }
      else {
        ValLib::vToString(pValue,sWorkspace);
        vTrace(" %s\n"_Stz,sWorkspace.aChar());
      }
      
      i8 iBytes = ValLib::iSize(pValue);
      pDoesCell = pAdd<Cell*>(pDoesCell,iBytes);
      
      continue;
    }
    
    // enterRoutine
    if ( *pDoesCell->m_ppEntryStep == Task::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)pDoesCell->m_ppEntryStep & (u8)~0xFFFF)
                                 + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n"_Stz,pDoesCell,pCallsWord->sName().aChar());
      pDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n"_Stz,pDoesCell,pCallsWord->sName().aChar());
    
    pDoesCell++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSemicolon(MP_Task mpTask)
{
  ENTRY("vSemicolon"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  if (mpTask->m_pCompiler->m_nVariables) {
    
    // reset the number of local variables
    mpTask->m_pCompiler->m_nVariables = 0;
    
    // pop top dictionary
    mpTask->m_xPathStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Stz);
  }
  
  pCurrentWord->vEncode("exit"_Stz);
  
  mpTask->m_pCompiler->m_bCompiling = false;
      
  // discard subDic use
  for ( ; mpTask->m_pCompiler->m_nSubDic; mpTask->m_pCompiler->m_nSubDic--)
    mpTask->m_xPathStack.vDiscard();

  mpTask->m_pCompiler->m_pCurrentWord = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subDic -- push sub dictionary                                              */
/*----------------------------------------------------------------------------*/
void Task::vSubDic(MP_Task mpTask)
{
  ENTRY("vSubDic"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  s32 sNameSubDic;
  sNameSubDic = mpTask->m_pCompiler->m_pTokeniser->sToken();
  
  Word* pWordSubDic = mpTask->m_xPathStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    THROW("No such subDic: %s"_Stz,sNameSubDic.aChar());
  
  Dic* pDic = _pDic(pWordSubDic->aBodyArea());
  
  mpTask->m_xPathStack.vPush(pDic);

  mpTask->m_pCompiler->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(MP_Task mpTask)
{
  ENTRY("vTick"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  //  if ( ! bExtracted ) {
  //    mpTask->m_sException.vSprint("No name: ' ");
  //    mpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No word: %s "_Stz,mpTask->m_pCompiler->m_pTokeniser->sToken().aChar());
  
  if (mpTask->m_pCompiler->m_bCompiling) {
    Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
    pCurrentWord->vEncode("pushValue"_Stz,pWord);
  }
  else {
    mpTask->m_pDataStack->vPushObject(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Task::vWord(MP_Task mpTask)
{
  ENTRY("vWord"_Stz);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(MP_Task mpTask)
{
ENTRY("vWordForget"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  Word* pRemovedWord = mpTask->m_xPathStack.pRemoveWord(pWord->sName());
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(MP_Task mpTask)
{
ENTRY("vWordDump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateGet(MP_Task mpTask)
{
  ENTRY("vWordImmediateGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  mpTask->m_pDataStack->vPushInt(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateSet(MP_Task mpTask)
{
  ENTRY("vWordImmediateSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! mpTask->m_pDataStack->bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!"_Stz);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vWordIsImmediate(MP_Task mpTask)
{
  ENTRY("vWordIsImmediate"_Stz);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isPrimitive -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Task::vWordIsPrimitive(MP_Task mpTask)
{
  ENTRY("vWordIsPrimitive"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isPrimitive"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(MP_Task mpTask)
{
ENTRY("vWordKindGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  Word* pWordKind = pWord->pKind();
  
  mpTask->m_pDataStack->vPushObject(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(MP_Task mpTask)
{
ENTRY("vWordKindSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Stz);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(MP_Task mpTask)
{
ENTRY("vWordName"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName());
  
  mpTask->m_pDataStack->vPushS32(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordPrev(MP_Task mpTask)
{
  ENTRY("vWordPrev"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  mpTask->m_pDataStack->vPushObject(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(MP_Task mpTask)
{
ENTRY("vWordThis"_Stz);
  
  Word* pWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  mpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeGet(MP_Task mpTask)
{
  ENTRY("vWordTypeGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  eWT eWordType = pWord->eWordType();
  
  mpTask->m_pDataStack->vPushInt(eWordType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type! -- word type set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeSet(MP_Task mpTask)
{
  ENTRY("vWordTypeSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Stz);
  Word* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! mpTask->m_pDataStack->bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Stz);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
