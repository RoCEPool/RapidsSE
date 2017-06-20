/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Word(void)
{
  vAddPrimitive("allParticipants"_Str, &vAllParticipants,  true );
  vAddPrimitive("allot"_Str,           &vAllot,            false);
  vAddPrimitive("cell"_Str,            &vCell,             false);
  vAddPrimitive(":"_Str,               &vColon,            false);
  vAddPrimitive(","_Str,               &vComma,            false);
  vAddPrimitive("compile,"_Str,        &vCompileComma,     false);
  vAddPrimitive("create"_Str,          &vCreate,           false);
  vAddPrimitive(">create"_Str,         &vCreateFrom,       false);
  vAddPrimitive("dic?"_Str,            &vDicWhich,         false);
  vAddPrimitive("does>"_Str,           &vDoes,             true );
  vAddPrimitive("does_rt"_Str,         &vDoes_RT,          false);
  vAddPrimitive("find"_Str,            &vFind,             false);
  vAddPrimitive("findInCurrent"_Str,   &vFindInCurrent,    false);
  vAddPrimitive("forget"_Str,          &vForget,           false);
  vAddPrimitive("##:"_Str,             &vHashHashColon,    true );
  vAddPrimitive("indirect"_Str,        &vIndirect,         true );
  vAddPrimitive("immediate"_Str,       &vImmediate,        true );
  vAddPrimitive("localCalls"_Str,      &vLocalCalls,       true );
  vAddPrimitive("locate"_Str,          &vLocate,           false);
  vAddPrimitive("oneParticipant"_Str,  &vOneParticipant,   true );
  vAddPrimitive("opDic"_Str,           &vOpDic,            true );
  vAddPrimitive("planDic"_Str,         &vPlanDicTask,      true );
  vAddPrimitive("postpone"_Str,        &vPostpone,         true );
  vAddPrimitive("see"_Str,             &vSee,              false);
  vAddPrimitive(";"_Str,               &vSemicolon,        true );
  vAddPrimitive("subDic"_Str,          &vSubDic,           true );
  vAddPrimitive("'"_Str,               &vTick,             true );
  vAddPrimitive("word.dump"_Str,       &vWordDump,         false);
  vAddPrimitive("word.forget"_Str,     &vWordForget,       false);
  vAddPrimitive("word.immediate@"_Str, &vWordImmediateGet, false);
  vAddPrimitive("word.immediate!"_Str, &vWordImmediateSet, false);
  vAddPrimitive("word.isImmediate"_Str,&vWordIsImmediate,  false);
  vAddPrimitive("word.isPrimitive"_Str,&vWordIsPrimitive,  false);
  vAddPrimitive("word.kind@"_Str,      &vWordKindGet,      false);
  vAddPrimitive("word.kind!"_Str,      &vWordKindSet,      false);
  vAddPrimitive("word.name"_Str,       &vWordName,         false);
  vAddPrimitive("word.prev"_Str,       &vWordPrev,         false);
  vAddPrimitive("word.this"_Str,       &vWordThis,         false);
  vAddPrimitive("word.type@"_Str,      &vWordTypeGet,      false);
  vAddPrimitive("word.type!"_Str,      &vWordTypeSet,      false);
}

/*----------------------------------------------------------------------------*/
/* allParticipants -- create an allParticipant word                 immediate */
/*----------------------------------------------------------------------------*/
void Task::vAllParticipants(MP_Task mpTask)
{
  ENTRY("vAllParticipants"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vDistributed(DT_ALLPARTICIPANTS);
  
  // get number of parameters
  mpTask->m_pTokeniser->vNextToken();
  
  // convert string to integer
  Str* aToken = mpTask->m_pTokeniser->sToken();
  i8 iParams = aToken->iToI8();
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  // get result routine name to be called after all participants have responded
  mpTask->m_pTokeniser->vNextToken();
  aToken = mpTask->m_pTokeniser->sToken();
  
  // set the distributed result routine name in the word
  pCurrentWord->vFinalRoutine(aToken);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(MP_Task mpTask)
{
  ENTRY("vAllot"_Str);
  
  i8 iBytes;
  
  if ( !  mpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  char* aAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(aAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of cell                                                  */
/*----------------------------------------------------------------------------*/
void Task::vCell(MP_Task mpTask)
{
  ENTRY("vCell"_Str);
  
  i8 nBytes = sizeof(void*);
  
  mpTask->m_pDataStack->vPushInt(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(MP_Task mpTask)
{
  ENTRY("vColon"_Str);
  
  s32 sName;
  
  mpTask->m_bCompiling = true;
  
  mpTask->m_pTokeniser->vNextToken();
  sName = mpTask->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(MP_Task mpTask)
{
  ENTRY("vComma"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("comma"_Str);
  
  pCurrentWord->vEncode("pushValue"_Str,&xValWrk);
  pCurrentWord->vEncode("exit"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Task::vCompileComma(MP_Task mpTask)
{
  ENTRY("vCompileComma"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,"_Str);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(MP_Task mpTask)
{
ENTRY("vCreate"_Str);
  
  mpTask->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  
  Word* pNewWord = new(pDic) Word(mpTask->m_pTokeniser->sToken());
  
  mpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(MP_Task mpTask)
{
ENTRY("vCreateFrom"_Str);

  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Str);
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic? -- test & push which sub dic a colon defn is to be compiled into      */
/*----------------------------------------------------------------------------*/
void Task::vDicWhich(MP_Task mpTask)
{
  ENTRY("vDicWhich"_Str);
  
  // get current plan
  Plan* pPlan = mpTask->m_pPlan;
  if ( ! pPlan )
    THROW("dic?: no current plan in task"_Str);
  
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
  mpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(MP_Task mpTask)
{
ENTRY("vDoes"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  pCurrentWord->vEncode("does_rt"_Str);
  pCurrentWord->vEncode("exit"_Str);
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Task::vDoes_RT(MP_Task mpTask)
{
  ENTRY("vDoes_RT"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  pCurrentWord->vEntryStep(&Task::vEnterObject);
  pCurrentWord->vEntryCell(mpTask->IP.m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(MP_Task mpTask)
{
ENTRY("vFind"_Str);
  
  s256 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("find"_Str);
 
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
  ENTRY("vFindInCurrent"_Str);
  
  s256 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent"_Str);
  
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
ENTRY("vForget"_Str);
  
  mpTask->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s"_Str,mpTask->m_pTokeniser->sToken().s());
  
  bool bResult = mpTask->m_xPathStack.bForget(mpTask->m_pTokeniser->sToken());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ##: -- encode paste of abstract colon defn into plan lambda text immediate */
/*----------------------------------------------------------------------------*/
void Task::vHashHashColon(MP_Task mpTask)
{
  ENTRY("vHashHashColon"_Str);
  
  ValWrk xWsp;
  Cell* pOpFrameJumpArg = nullptr;
  Cell* pToColonJumpArg = nullptr;

  // get word currently being compiled
  Word* pW = mpTask->m_pCurrentWord;
  
  // get the name of the symbolic argument
  s32 sSymbolicArg;
  mpTask->m_pTokeniser->vNextToken();
  sSymbolicArg = mpTask->m_pTokeniser->sToken();
  
  // encode what to execute at runtime
  
  // at runtime the dstack will be ( plan value )
  
                                  // at runtime we synthesise by:
  pW->vEncode("dup"_Str);             //   ( value plan plan )
  pW->vEncode("-rot"_Str);            //   ( plan value plan )
  
  pW->vEncode("plan.opFrame0"_Str);   //   get top opframe of plan
  pW->vEncode("dup"_Str);             //   dup
  pW->vEncode("jumpTrue"_Str);        //   forward jump if we have an opFrame
  pOpFrameJumpArg = (Cell*)pW->pAppend((Cell*)nullptr);
  
  // we dont have an opFrame, only a plan
  pW->vEncode("drop"_Str);            //   drop the null opFrame pointer
  xWsp = "planDic "_Str;
  pW->vEncode("pushValue"_Str,&xWsp); //   push "planDic "

  pW->vEncode("jump"_Str);            //   forward jump to colon etc
  pToColonJumpArg = (Cell*)pW->pAppend((Cell*)nullptr);

  // we have an opFrame
  pOpFrameJumpArg->m_pJumpTo = pW->pLastCell();
  
  pW->vEncode("opFrame.name"_Str);    //   get name of opframe
  xWsp = "opDic "_Str;
  pW->vEncode("pushValue"_Str,&xWsp); //   push "opDic "
  pW->vEncode("str.prepend"_Str);     //   form "opDic <opDicNm>"
  
  // we have an opFrame
  pToColonJumpArg->m_pJumpTo = pW->pLastCell();
  
  xWsp = " : "_Str;
  pW->vEncode("pushValue"_Str,&xWsp); //   push " : "
  pW->vEncode("str.cat"_Str);         //   form "opDic <opDicNm> : "
  
  xWsp = sSymbolicArg;
  pW->vEncode("pushValue"_Str,&xWsp); //   push "<symNm>"
  pW->vEncode("str.cat"_Str);         //   form "opDic <opDicNm> : <symNm>"
  
  xWsp = " %V ; "_Str;
  pW->vEncode("pushValue"_Str,&xWsp); //   push " %V ; "
  pW->vEncode("str.cat"_Str);         //   form "opDic <opDicNm> : <symNm> %V ; "
  
  pW->vEncode("str.format"_Str);      //   replace %V to yield concrete defn
  
  // at runtime this should produce a code text of the form:
  //      "planDic : <symNm> <value> ; "
  //   or
  //      "opDic <opDicNm> : <symNm> <value> ; "
  
  pW->vEncode("swap"_Str);               //   dstack is ( codeText plan )
  pW->vEncode("plan.lambda+"_Str);       //   append to lambda text
  
  // at runtime the dstack will be ( )
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* indirect -- create an indirect word                              immediate */
/*----------------------------------------------------------------------------*/
void Task::vIndirect(MP_Task mpTask)
{
  ENTRY("vIndirect"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vIndirect(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(MP_Task mpTask)
{
  ENTRY("vImmediate"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localCalls -- set current word as a locals calls only word       immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalCalls(MP_Task mpTask)
{
  ENTRY("vLocalCalls"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  pCurrentWord->vDistributed(DT_LOCALCALLS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(MP_Task mpTask)
{
  ENTRY("vLocate"_Str);
  
  i8 iAddrAsInt;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iAddrAsInt) )
    THROW_INVALID_DSTACK("locate"_Str);
  
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
/* oneParticipant -- create a oneParticipant word                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vOneParticipant(MP_Task mpTask)
{
  ENTRY("vAllParticipants"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vDistributed(DT_ONEPARTICIPANT);
  
  // get number of parameters
  mpTask->m_pTokeniser->vNextToken();
  
  // convert token to integer
  i8 iParams = mpTask->m_pTokeniser->sToken().iToI8();
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opDic -- search for named op frame and push op dictionary onto path stack  */
/*----------------------------------------------------------------------------*/
void Task::vOpDic(MP_Task mpTask)
{
  ENTRY("vOpDic"_Str);
  
  Dic* pOpDic = nullptr;

  s32 sNameOpFrame;
  
  mpTask->m_pTokeniser->vNextToken();
  sNameOpFrame = mpTask->m_pTokeniser->sToken();
  
  RegFrame* pOpFrame = mpTask->m_pPlan->pRegFrameFind(sNameOpFrame);
  //if ( ! pOpFrame )
  //  THROW("No such op frame: %s",sNameOpFrame.s());
  
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
    sNameDic.vSprint("<%s>"_Str,sNameOpFrame.s());
    
    pOpDic = new Dic;
    pOpDic->vName(sNameDic);
    pOpFrame->vOpDic(pOpDic);
  }
  
  mpTask->m_xPathStack.vPush(pOpDic);
  
  mpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planDic -- push plan dictionary onto path stack                            */
/*----------------------------------------------------------------------------*/
void Task::vPlanDicTask(MP_Task mpTask)
{
  ENTRY("vPlanDicTask"_Str);
  
  Dic* pPlanDic = mpTask->m_pPlan->pDic();
  
  mpTask->m_xPathStack.vPush(pPlanDic);
  
  mpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Task::vPostpone(MP_Task mpTask)
{
  ENTRY("vPostpone"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // get name of word to be called
  mpTask->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s "_Str,mpTask->m_pTokeniser->sToken().s());
  
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
    pCurrentWord->vEncode("pushValue"_Str,pCalledWord);
    if (WT_PRIMITIVE == pCalledWord->eWordType())
      pCurrentWord->vEncode("encodeCallPrimitive"_Str);
    else
      pCurrentWord->vEncode("encodeCallRoutine"_Str);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSee(MP_Task mpTask)
{
  ENTRY("vSee"_Str);
  
  s48K sWorkspace;
  
  mpTask->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Str,mpTask->m_pTokeniser->sToken().s());
    return;
  }
  
  vTrace("See: %s\n"_Str,pWord->sName().s());
  
  Cell* pDoesCell = pWord->pEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)pDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (pDoesCell < pDoesWord->pLastCell()) {
    
    Word* pCallsWord = mpTask->m_xPathStack.pFindStep(*pDoesCell->m_ppEntryStep);
    
    // jumps
    if (*pDoesCell->m_ppEntryStep == Task::vFlowJump ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpFalse ||
        *pDoesCell->m_ppEntryStep == Task::vFlowJumpTrue ) {
      
      vTrace("%p: %s"_Str,pDoesCell,pCallsWord->sName().s());
      
      pDoesCell++;
      
      vTrace(" %p\n"_Str,pDoesCell->m_pJumpTo);
      
      pDoesCell++;
      
      continue;
    }
    
    // push
    if ( *pDoesCell->m_ppEntryStep == Task::vPushValue ) {
      
      vTrace("%p: %s"_Str,pDoesCell,pCallsWord->sName().s());
      
      pDoesCell++;
      
      ValWrk* pValue = reinterpret_cast<ValWrk*>(pDoesCell);
      
      if (CN_STRVAL == pValue->eClass()) {
        vTrace(" \"%s\"\n"_Str,_pStrVal(pValue)->s());
      }
      else {
        ValLib::vToString(pValue,sWorkspace);
        vTrace(" %s\n"_Str,sWorkspace.s());
      }
      
      i8 iBytes = ValLib::iSize(pValue);
      pDoesCell = pAdd<Cell*>(pDoesCell,iBytes);
      
      continue;
    }
    
    // enterRoutine
    if ( *pDoesCell->m_ppEntryStep == Task::vEnterRoutine ) {
      
      Word* pCallsWord = (Word*)( ((u8)pDoesCell->m_ppEntryStep & (u8)~0xFFFF)
                                 + sizeof(HeapBuffer));
      
      vTrace("%p: %s\n"_Str,pDoesCell,pCallsWord->sName().s());
      pDoesCell++;
      continue;
    }
    
    // other
    vTrace("%p: %s\n"_Str,pDoesCell,pCallsWord->sName().s());
    
    pDoesCell++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSemicolon(MP_Task mpTask)
{
  ENTRY("vSemicolon"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  if (mpTask->m_nVariables) {
    
    // reset the number of local variables
    mpTask->m_nVariables = 0;
    
    // pop top dictionary
    mpTask->m_xPathStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Str);
  }
  
  pCurrentWord->vEncode("exit"_Str);
  
  mpTask->m_bCompiling = false;
  
  if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed() ||
      DT_ONEPARTICIPANT  == pCurrentWord->eDistributed()) {
    
    s32 sTildeName;
    sTildeName = "~"_Str;
    sTildeName.vConcat(pCurrentWord->sName());
    
    // create new word for tilde of current word
    Dic* pDic;
    if (mpTask->m_xPathStack.iCount())
      pDic = mpTask->m_xPathStack.pTop();
    else
      pDic = gpMainDic;
    Word* pWordTilde = new(pDic) Word(sTildeName);
    // ** do not set as current word **
    
    // encode the word
    ValWrk xValWrk;
    
    xValWrk = sTildeName;
    pWordTilde->vEncode("pushValue"_Str,&xValWrk);
    pWordTilde->vEncode("tracePoint"_Str);
    
    xValWrk = pCurrentWord->iParams();
    pWordTilde->vEncode("pushValue"_Str,&xValWrk);
    pWordTilde->vEncode("put$$"_Str);
    
    xValWrk = pCurrentWord->sName();
    pWordTilde->vEncode("pushValue"_Str,&xValWrk);
    
    xValWrk = "theIndirectCall"_Str;
    pWordTilde->vEncode("pushValue"_Str,&xValWrk);
    pWordTilde->vEncode("tracePoint"_Str);
    
    pWordTilde->vEncode(">>"_Str);
    
    if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed())
      pWordTilde->vEncode(">>allParticipants>"_Str);
    else
      pWordTilde->vEncode(">>oneParticipant>"_Str);
    
    xValWrk = sTildeName;
    pWordTilde->vEncode("pushValue"_Str,&xValWrk);
    pWordTilde->vEncode("tracePoint"_Str);
    
    pWordTilde->vEncode((Str*)pCurrentWord->pFinalRoutine());
    
    pWordTilde->vEncode("exit"_Str);
  }
    
  // discard subDic use
  for ( ; mpTask->m_nSubDic; mpTask->m_nSubDic--)
    mpTask->m_xPathStack.vDiscard();

  mpTask->m_pCurrentWord = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subDic -- push sub dictionary                                              */
/*----------------------------------------------------------------------------*/
void Task::vSubDic(MP_Task mpTask)
{
  ENTRY("vSubDic"_Str);
  
  mpTask->m_pTokeniser->vNextToken();
  
  s32 sNameSubDic;
  sNameSubDic = mpTask->m_pTokeniser->sToken();
  
  Word* pWordSubDic = mpTask->m_xPathStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    THROW("No such subDic: %s"_Str,sNameSubDic.s());
  
  Dic* pDic = _pDic(pWordSubDic->aBodyArea());
  
  mpTask->m_xPathStack.vPush(pDic);

  mpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(MP_Task mpTask)
{
  ENTRY("vTick"_Str);
  
  mpTask->m_pTokeniser->vNextToken();
  
  //  if ( ! bExtracted ) {
  //    mpTask->m_sException.vSprint("No name: ' ");
  //    mpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No word: %s "_Str,mpTask->m_pTokeniser->sToken().s());
  
  if (mpTask->m_bCompiling) {
    Word* pCurrentWord = mpTask->m_pCurrentWord;
    pCurrentWord->vEncode("pushValue"_Str,pWord);
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
  ENTRY("vWord"_Str);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(MP_Task mpTask)
{
ENTRY("vWordForget"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget"_Str);
  Word* pWord = _pWord(pWordObject);
  
  bool bResult = mpTask->m_xPathStack.bForget(pWord->sName());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(MP_Task mpTask)
{
ENTRY("vWordDump"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump"_Str);
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateGet(MP_Task mpTask)
{
  ENTRY("vWordImmediateGet"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@"_Str);
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
  ENTRY("vWordImmediateSet"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! mpTask->m_pDataStack->bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!"_Str);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vWordIsImmediate(MP_Task mpTask)
{
  ENTRY("vWordIsImmediate"_Str);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate"_Str);
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
  ENTRY("vWordIsPrimitive"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isPrimitive"_Str);
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
ENTRY("vWordKindGet"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet"_Str);
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
ENTRY("vWordKindSet"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Str);
  Word* pWord = _pWord(pWordObject);
  
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Str);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(MP_Task mpTask)
{
ENTRY("vWordName"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name"_Str);
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
  ENTRY("vWordPrev"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev"_Str);
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
ENTRY("vWordThis"_Str);
  
  Word* pWord = mpTask->m_pCurrentWord;
  
  mpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeGet(MP_Task mpTask)
{
  ENTRY("vWordTypeGet"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet"_Str);
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
  ENTRY("vWordTypeSet"_Str);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! mpTask->m_pDataStack->bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Str);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
