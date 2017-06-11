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
void Task::vAllParticipants(NNP_Task nnpTask)
{
  ENTRY("vAllParticipants"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vDistributed(DT_ALLPARTICIPANTS);
  
  // get number of parameters
  nnpTask->m_pTokeniser->vNextToken();
  
  // convert string to integer
  Str* aToken = nnpTask->m_pTokeniser->sToken();
  i8 iParams = aToken->iToI8();
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  // get result routine name to be called after all participants have responded
  nnpTask->m_pTokeniser->vNextToken();
  aToken = nnpTask->m_pTokeniser->sToken();
  
  // set the distributed result routine name in the word
  pCurrentWord->vFinalRoutine(aToken);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(NNP_Task nnpTask)
{
  ENTRY("vAllot"_Str);
  
  i8 iBytes;
  
  if ( !  nnpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  char* aAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(aAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cell -- push size of cell                                                  */
/*----------------------------------------------------------------------------*/
void Task::vCell(NNP_Task nnpTask)
{
  ENTRY("vCell"_Str);
  
  i8 nBytes = sizeof(void*);
  
  nnpTask->m_pDataStack->vPushInt(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(NNP_Task nnpTask)
{
  ENTRY("vColon"_Str);
  
  s32 sName;
  
  nnpTask->m_bCompiling = true;
  
  nnpTask->m_pTokeniser->vNextToken();
  sName = nnpTask->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (nnpTask->m_xPathStack.iCount())
    pDic = nnpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  Word* pNewWord = new(pDic) Word(sName);
  
  nnpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(NNP_Task nnpTask)
{
  ENTRY("vComma"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  ValWrk xValWrk;
  if ( ! nnpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("comma"_Str);
  
  pCurrentWord->vEncode("pushValue"_Str,&xValWrk);
  pCurrentWord->vEncode("exit"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Task::vCompileComma(NNP_Task nnpTask)
{
  ENTRY("vCompileComma"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("compile,"_Str);
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
  
  pCurrentWord->vEncode(ppCalledEntryStep);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(NNP_Task nnpTask)
{
ENTRY("vCreate"_Str);
  
  nnpTask->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (nnpTask->m_xPathStack.iCount())
    pDic = nnpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  
  Word* pNewWord = new(pDic) Word(nnpTask->m_pTokeniser->sToken());
  
  nnpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(NNP_Task nnpTask)
{
ENTRY("vCreateFrom"_Str);

  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK(">create"_Str);
  
  Dic* pDic;
  if (nnpTask->m_xPathStack.iCount())
    pDic = nnpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  
  Word* pNewWord = new(pDic) Word(sName);
  
  nnpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic? -- test & push which sub dic a colon defn is to be compiled into      */
/*----------------------------------------------------------------------------*/
void Task::vDicWhich(NNP_Task nnpTask)
{
  ENTRY("vDicWhich"_Str);
  
  // get current plan
  Plan* pPlan = nnpTask->m_pPlan;
  if ( ! pPlan )
    THROW("dic?: no current plan in task"_Str);
  
  // sub dic is either latest op dic (if there is one), or the plan dic
  Dic* pSubDic;
  if (pPlan->pRegFrameTop())
    pSubDic = pPlan->pRegFrameTop()->pOpDic();
  else
    pSubDic = pPlan->pDic();
  
  // push the sub dic onto the path stack
  nnpTask->m_xPathStack.vPush(pSubDic);
  
  // by incrementing m_nSubDic we are indicating to the ; word that
  // there is another temporary sub dic on the path stack
  // which needs to be discarded
  nnpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(NNP_Task nnpTask)
{
ENTRY("vDoes"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  pCurrentWord->vEncode("does_rt"_Str);
  pCurrentWord->vEncode("exit"_Str);
  
  //pCurrentWord->vEncode("pushBody");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does_rt -- runtime for does>                                               */
/*----------------------------------------------------------------------------*/
void Task::vDoes_RT(NNP_Task nnpTask)
{
  ENTRY("vDoes_RT"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  pCurrentWord->vEntryStep(&Task::vEnterObject);
  pCurrentWord->vEntryCell(nnpTask->IP.m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(NNP_Task nnpTask)
{
ENTRY("vFind"_Str);
  
  s256 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("find"_Str);
 
  if (nnpTask->m_iTracing) {
    i8 i = 0;
  }
  
  Word* pWord = nnpTask->m_xPathStack.pFindWord(sName);
  
  nnpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find word in current dictionary                           */
/*----------------------------------------------------------------------------*/
void Task::vFindInCurrent(NNP_Task nnpTask)
{
  ENTRY("vFindInCurrent"_Str);
  
  s256 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("findInCurrent"_Str);
  
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->m_xPathStack.pFindInCurrent(sName);
  
  nnpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Task::vForget(NNP_Task nnpTask)
{
ENTRY("vForget"_Str);
  
  nnpTask->m_pTokeniser->vNextToken();
  
  Word* pWord = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s"_Str,nnpTask->m_pTokeniser->sToken().s());
  
  bool bResult = nnpTask->m_xPathStack.bForget(nnpTask->m_pTokeniser->sToken());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ##: -- encode paste of abstract colon defn into plan lambda text immediate */
/*----------------------------------------------------------------------------*/
void Task::vHashHashColon(NNP_Task nnpTask)
{
  ENTRY("vHashHashColon"_Str);
  
  ValWrk xWsp;
  Cell* pOpFrameJumpArg = nullptr;
  Cell* pToColonJumpArg = nullptr;

  // get word currently being compiled
  Word* pW = nnpTask->m_pCurrentWord;
  
  // get the name of the symbolic argument
  s32 sSymbolicArg;
  nnpTask->m_pTokeniser->vNextToken();
  sSymbolicArg = nnpTask->m_pTokeniser->sToken();
  
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
void Task::vIndirect(NNP_Task nnpTask)
{
  ENTRY("vIndirect"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vIndirect(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(NNP_Task nnpTask)
{
  ENTRY("vImmediate"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localCalls -- set current word as a locals calls only word       immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalCalls(NNP_Task nnpTask)
{
  ENTRY("vLocalCalls"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  pCurrentWord->vDistributed(DT_LOCALCALLS);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(NNP_Task nnpTask)
{
  ENTRY("vLocate"_Str);
  
  i8 iAddrAsInt;
  if ( ! nnpTask->m_pDataStack->bPopAsInt(iAddrAsInt) )
    THROW_INVALID_DSTACK("locate"_Str);
  
  void* pCellVoid = (void*)iAddrAsInt;
  Cell* pCell = _pCell(pCellVoid);
  
  Word* pLocatedWord = nnpTask->m_xPathStack.pLocate(pCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    nnpTask->m_pDataStack->vPushS32(sName);
  }
  else {
    i8 iZero = 0;
    nnpTask->m_pDataStack->vPushInt(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* oneParticipant -- create a oneParticipant word                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vOneParticipant(NNP_Task nnpTask)
{
  ENTRY("vAllParticipants"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Str,pCurrentWord->sName().s());
  
  pCurrentWord->vDistributed(DT_ONEPARTICIPANT);
  
  // get number of parameters
  nnpTask->m_pTokeniser->vNextToken();
  
  // convert token to integer
  i8 iParams = nnpTask->m_pTokeniser->sToken().iToI8();
  
  // set the number of parameters in the current word
  pCurrentWord->vParams(iParams);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opDic -- search for named op frame and push op dictionary onto path stack  */
/*----------------------------------------------------------------------------*/
void Task::vOpDic(NNP_Task nnpTask)
{
  ENTRY("vOpDic"_Str);
  
  Dic* pOpDic = nullptr;

  s32 sNameOpFrame;
  
  nnpTask->m_pTokeniser->vNextToken();
  sNameOpFrame = nnpTask->m_pTokeniser->sToken();
  
  RegFrame* pOpFrame = nnpTask->m_pPlan->pRegFrameFind(sNameOpFrame);
  //if ( ! pOpFrame )
  //  THROW("No such op frame: %s",sNameOpFrame.s());
  
  if ( pOpFrame ) {
    pOpDic = pOpFrame->pOpDic();
  }
  else {
    // create the frame
    pOpFrame = new RegFrame;
    pOpFrame->vName(sNameOpFrame);
    nnpTask->m_pPlan->vRegFramePush(pOpFrame);
    
    // create a op dictionary
    s32 sNameDic;
    sNameDic.vSprint("<%s>"_Str,sNameOpFrame.s());
    
    pOpDic = new Dic;
    pOpDic->vName(sNameDic);
    pOpFrame->vOpDic(pOpDic);
  }
  
  nnpTask->m_xPathStack.vPush(pOpDic);
  
  nnpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planDic -- push plan dictionary onto path stack                            */
/*----------------------------------------------------------------------------*/
void Task::vPlanDicTask(NNP_Task nnpTask)
{
  ENTRY("vPlanDicTask"_Str);
  
  Dic* pPlanDic = nnpTask->m_pPlan->pDic();
  
  nnpTask->m_xPathStack.vPush(pPlanDic);
  
  nnpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Task::vPostpone(NNP_Task nnpTask)
{
  ENTRY("vPostpone"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // get name of word to be called
  nnpTask->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s "_Str,nnpTask->m_pTokeniser->sToken().s());
  
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
void Task::vSee(NNP_Task nnpTask)
{
  ENTRY("vSee"_Str);
  
  s48K sWorkspace;
  
  nnpTask->m_pTokeniser->vNextToken();
  
  Word* pWord = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Str,nnpTask->m_pTokeniser->sToken().s());
    return;
  }
  
  vTrace("See: %s\n"_Str,pWord->sName().s());
  
  Cell* pDoesCell = pWord->pEntryCell();
  
  Word* pDoesWord = (Word*)( ((u8)pDoesCell & (u8)~0xFFFF)
                            + sizeof(HeapBuffer));
  
  while (pDoesCell < pDoesWord->pLastCell()) {
    
    Word* pCallsWord = nnpTask->m_xPathStack.pFindStep(*pDoesCell->m_ppEntryStep);
    
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
void Task::vSemicolon(NNP_Task nnpTask)
{
  ENTRY("vSemicolon"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  if (nnpTask->m_nVariables) {
    
    // reset the number of local variables
    nnpTask->m_nVariables = 0;
    
    // pop top dictionary
    nnpTask->m_xPathStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Str);
  }
  
  pCurrentWord->vEncode("exit"_Str);
  
  nnpTask->m_bCompiling = false;
  
  if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed() ||
      DT_ONEPARTICIPANT  == pCurrentWord->eDistributed()) {
    
    s32 sTildeName;
    sTildeName = "~"_Str;
    sTildeName.vConcat(pCurrentWord->sName());
    
    // create new word for tilde of current word
    Dic* pDic;
    if (nnpTask->m_xPathStack.iCount())
      pDic = nnpTask->m_xPathStack.pTop();
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
  for ( ; nnpTask->m_nSubDic; nnpTask->m_nSubDic--)
    nnpTask->m_xPathStack.vDiscard();

  nnpTask->m_pCurrentWord = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subDic -- push sub dictionary                                              */
/*----------------------------------------------------------------------------*/
void Task::vSubDic(NNP_Task nnpTask)
{
  ENTRY("vSubDic"_Str);
  
  nnpTask->m_pTokeniser->vNextToken();
  
  s32 sNameSubDic;
  sNameSubDic = nnpTask->m_pTokeniser->sToken();
  
  Word* pWordSubDic = nnpTask->m_xPathStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    THROW("No such subDic: %s"_Str,sNameSubDic.s());
  
  Dic* pDic = _pDic(pWordSubDic->aBodyArea());
  
  nnpTask->m_xPathStack.vPush(pDic);

  nnpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(NNP_Task nnpTask)
{
  ENTRY("vTick"_Str);
  
  nnpTask->m_pTokeniser->vNextToken();
  
  //  if ( ! bExtracted ) {
  //    nnpTask->m_sException.vSprint("No name: ' ");
  //    nnpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  Word* pWord = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No word: %s "_Str,nnpTask->m_pTokeniser->sToken().s());
  
  if (nnpTask->m_bCompiling) {
    Word* pCurrentWord = nnpTask->m_pCurrentWord;
    pCurrentWord->vEncode("pushValue"_Str,pWord);
  }
  else {
    nnpTask->m_pDataStack->vPushObject(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Task::vWord(NNP_Task nnpTask)
{
  ENTRY("vWord"_Str);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(NNP_Task nnpTask)
{
ENTRY("vWordForget"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget"_Str);
  Word* pWord = _pWord(pWordObject);
  
  bool bResult = nnpTask->m_xPathStack.bForget(pWord->sName());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(NNP_Task nnpTask)
{
ENTRY("vWordDump"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump"_Str);
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateGet(NNP_Task nnpTask)
{
  ENTRY("vWordImmediateGet"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  nnpTask->m_pDataStack->vPushInt(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Task::vWordImmediateSet(NNP_Task nnpTask)
{
  ENTRY("vWordImmediateSet"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! nnpTask->m_pDataStack->bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!"_Str);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vWordIsImmediate(NNP_Task nnpTask)
{
  ENTRY("vWordIsImmediate"_Str);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPushInt(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isPrimitive -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Task::vWordIsPrimitive(NNP_Task nnpTask)
{
  ENTRY("vWordIsPrimitive"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isPrimitive"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPushInt(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(NNP_Task nnpTask)
{
ENTRY("vWordKindGet"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet"_Str);
  Word* pWord = _pWord(pWordObject);
  
  Word* pWordKind = pWord->pKind();
  
  nnpTask->m_pDataStack->vPushObject(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(NNP_Task nnpTask)
{
ENTRY("vWordKindSet"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Str);
  Word* pWord = _pWord(pWordObject);
  
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Str);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(NNP_Task nnpTask)
{
ENTRY("vWordName"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name"_Str);
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName());
  
  nnpTask->m_pDataStack->vPushS32(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordPrev(NNP_Task nnpTask)
{
  ENTRY("vWordPrev"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev"_Str);
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  nnpTask->m_pDataStack->vPushObject(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(NNP_Task nnpTask)
{
ENTRY("vWordThis"_Str);
  
  Word* pWord = nnpTask->m_pCurrentWord;
  
  nnpTask->m_pDataStack->vPushObject(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeGet(NNP_Task nnpTask)
{
  ENTRY("vWordTypeGet"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet"_Str);
  Word* pWord = _pWord(pWordObject);
  
  eWT eWordType = pWord->eWordType();
  
  nnpTask->m_pDataStack->vPushInt(eWordType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type! -- word type set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordTypeSet(NNP_Task nnpTask)
{
  ENTRY("vWordTypeSet"_Str);
  
  Object* pWordObject;
  if ( ! nnpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Str);
  Word* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! nnpTask->m_pDataStack->bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Str);
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
