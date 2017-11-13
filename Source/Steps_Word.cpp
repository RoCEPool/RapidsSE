/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Word(void)
{
  vAddPrimitive("allParticipants"_Stz, &vAllParticipants,  true );
  vAddPrimitive("allot"_Stz,           &vAllot,            false);
  vAddPrimitive("cell"_Stz,            &vCell,             false);
  vAddPrimitive(":"_Stz,               &vColon,            false);
  vAddPrimitive(","_Stz,               &vComma,            false);
  vAddPrimitive("compile,"_Stz,        &vCompileComma,     false);
  vAddPrimitive("create"_Stz,          &vCreate,           false);
  vAddPrimitive(">create"_Stz,         &vCreateFrom,       false);
  vAddPrimitive(">createInMain"_Stz,   &vCreateFromInMain, false);
  vAddPrimitive("dic?"_Stz,            &vDicWhich,         false);
  vAddPrimitive("does>"_Stz,           &vDoes,             true );
  vAddPrimitive("does_rt"_Stz,         &vDoes_RT,          false);
  vAddPrimitive("find"_Stz,            &vFind,             false);
  vAddPrimitive("findInCurrent"_Stz,   &vFindInCurrent,    false);
  vAddPrimitive("forget"_Stz,          &vForget,           false);
  vAddPrimitive("##:"_Stz,             &vHashHashColon,    true );
  vAddPrimitive("indirect"_Stz,        &vIndirect,         true );
  vAddPrimitive("immediate"_Stz,       &vImmediate,        true );
  vAddPrimitive("localCalls"_Stz,      &vLocalCalls,       true );
  vAddPrimitive("locate"_Stz,          &vLocate,           false);
  vAddPrimitive("oneParticipant"_Stz,  &vOneParticipant,   true );
  vAddPrimitive("opDic"_Stz,           &vOpDic,            true );
  vAddPrimitive("planDic"_Stz,         &vPlanDicTask,      true );
  vAddPrimitive("postpone"_Stz,        &vPostpone,         true );
  vAddPrimitive("see"_Stz,             &vSee,              false);
  vAddPrimitive(";"_Stz,               &vSemicolon,        true );
  vAddPrimitive("subDic"_Stz,          &vSubDic,           true );
  vAddPrimitive("tick"_Stz,            &vTick,             true );
  vAddPrimitive("word.dump"_Stz,       &vWordDump,         false);
  vAddPrimitive("word.forget"_Stz,     &vWordForget,       false);
  vAddPrimitive("word.immediate@"_Stz, &vWordImmediateGet, false);
  vAddPrimitive("word.immediate!"_Stz, &vWordImmediateSet, false);
  vAddPrimitive("word.isImmediate"_Stz,&vWordIsImmediate,  false);
  vAddPrimitive("word.isPrimitive"_Stz,&vWordIsPrimitive,  false);
  vAddPrimitive("word.kind@"_Stz,      &vWordKindGet,      false);
  vAddPrimitive("word.kind!"_Stz,      &vWordKindSet,      false);
  vAddPrimitive("word.name"_Stz,       &vWordName,         false);
  vAddPrimitive("word.prev"_Stz,       &vWordPrev,         false);
  vAddPrimitive("word.this"_Stz,       &vWordThis,         false);
  vAddPrimitive("word.type@"_Stz,      &vWordTypeGet,      false);
  vAddPrimitive("word.type!"_Stz,      &vWordTypeSet,      false);
}

/*----------------------------------------------------------------------------*/
/* allParticipants -- create an allParticipant word                 immediate */
/*----------------------------------------------------------------------------*/
void Task::vAllParticipants(MP_Task mpTask)
{
  ENTRY("vAllParticipants"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Stz,pCurrentWord->sName().aChar());
  
  pCurrentWord->vDistributed(DT_ALLPARTICIPANTS);
  
  // get number of parameters
  mpTask->m_pTokeniser->vNextToken();
  
  // convert string to integer
  Stz* aToken = mpTask->m_pTokeniser->sToken();
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
  ENTRY("vAllot"_Stz);
  
  i8 iBytes;
  
  if ( !  mpTask->m_pDataStack->bPop(iBytes) )
    THROW_INVALID_DSTACK("allot"_Stz);
  
  RASSERT(iBytes);
  
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
  
  mpTask->m_bCompiling = true;
  
  mpTask->m_pTokeniser->vNextToken();
  sName = mpTask->m_pTokeniser->sToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCurrentWord = pNewWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(MP_Task mpTask)
{
  ENTRY("vComma"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("comma"_Stz);
  
  pCurrentWord->vEncode("pushValue"_Stz,&xValWrk);
  pCurrentWord->vEncode("exit"_Stz);
  
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
ENTRY("vCreate"_Stz);
  
  mpTask->m_pTokeniser->vNextToken();
  
  Dic* pDic;
  if (mpTask->m_xPathStack.iCount())
    pDic = mpTask->m_xPathStack.pTop();
  else
    pDic = gpMainDic;
  
  //vTrace(": %s\n"_Stz,mpTask->m_pTokeniser->sToken().aChar());

  Word* pNewWord = new(pDic) Word(mpTask->m_pTokeniser->sToken());
  
  mpTask->m_pCurrentWord = pNewWord;
  
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
    pDic = gpMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCurrentWord = pNewWord;
  
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
  
  Dic* pDic = gpMainDic;
  
  //vTrace(": %s\n"_Stz,sName.aChar());
  Word* pNewWord = new(pDic) Word(sName);
  
  mpTask->m_pCurrentWord = pNewWord;
  
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
  mpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(MP_Task mpTask)
{
ENTRY("vDoes"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
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
  
  mpTask->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No such word: %s"_Stz,mpTask->m_pTokeniser->sToken().aChar());
  
  bool bResult = mpTask->m_xPathStack.bForget(mpTask->m_pTokeniser->sToken());
  ASSERT(bResult);
  
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
  Word* pW = mpTask->m_pCurrentWord;
  
  // get the name of the symbolic argument
  s32 sSymbolicArg;
  mpTask->m_pTokeniser->vNextToken();
  sSymbolicArg = mpTask->m_pTokeniser->sToken();
  
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
/* indirect -- create an indirect word                              immediate */
/*----------------------------------------------------------------------------*/
void Task::vIndirect(MP_Task mpTask)
{
  ENTRY("vIndirect"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Stz,pCurrentWord->sName().aChar());
  
  pCurrentWord->vIndirect(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(MP_Task mpTask)
{
  ENTRY("vImmediate"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Stz,pCurrentWord->sName().aChar());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localCalls -- set current word as a locals calls only word       immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalCalls(MP_Task mpTask)
{
  ENTRY("vLocalCalls"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  pCurrentWord->vDistributed(DT_LOCALCALLS);
  
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
/* oneParticipant -- create a oneParticipant word                   immediate */
/*----------------------------------------------------------------------------*/
void Task::vOneParticipant(MP_Task mpTask)
{
  ENTRY("vAllParticipants"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  //vTrace("Task::vImmediate: %s\n"_Stz,pCurrentWord->sName().aChar());
  
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
  ENTRY("vOpDic"_Stz);
  
  Dic* pOpDic = nullptr;

  s32 sNameOpFrame;
  
  mpTask->m_pTokeniser->vNextToken();
  sNameOpFrame = mpTask->m_pTokeniser->sToken();
  
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
  
  mpTask->m_nSubDic++;
  
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
  
  mpTask->m_nSubDic++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* postpone -- append call to next word from input to current word  immediate */
/*----------------------------------------------------------------------------*/
void Task::vPostpone(MP_Task mpTask)
{
  ENTRY("vPostpone"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // get name of word to be called
  mpTask->m_pTokeniser->vNextToken();
  
  // find the word in dic stack
  Word* pCalledWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  if ( ! pCalledWord )
    THROW("No word: %s "_Stz,mpTask->m_pTokeniser->sToken().aChar());
  
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
  
  mpTask->m_pTokeniser->vNextToken();
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  if ( ! pWord ) {
    vTrace("See: %s unknown\n"_Stz,mpTask->m_pTokeniser->sToken().aChar());
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
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  if (mpTask->m_nVariables) {
    
    // reset the number of local variables
    mpTask->m_nVariables = 0;
    
    // pop top dictionary
    mpTask->m_xPathStack.vDiscard();
    
    // enocde locals frame pop
    pCurrentWord->vEncode("locals.pop"_Stz);
  }
  
  pCurrentWord->vEncode("exit"_Stz);
  
  mpTask->m_bCompiling = false;
  
  if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed() ||
      DT_ONEPARTICIPANT  == pCurrentWord->eDistributed()) {
    
    s32 sTildeName;
    sTildeName = "~"_Stz;
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
    pWordTilde->vEncode("pushValue"_Stz,&xValWrk);
    pWordTilde->vEncode("tracePoint"_Stz);
    
    xValWrk = pCurrentWord->iParams();
    pWordTilde->vEncode("pushValue"_Stz,&xValWrk);
    pWordTilde->vEncode("put$$"_Stz);
    
    xValWrk = pCurrentWord->sName();
    pWordTilde->vEncode("pushValue"_Stz,&xValWrk);
    
    xValWrk = "theIndirectCall"_Stz;
    pWordTilde->vEncode("pushValue"_Stz,&xValWrk);
    pWordTilde->vEncode("tracePoint"_Stz);
    
    pWordTilde->vEncode(">>"_Stz);
    
    if (DT_ALLPARTICIPANTS == pCurrentWord->eDistributed())
      pWordTilde->vEncode(">>allParticipants>"_Stz);
    else
      pWordTilde->vEncode(">>oneParticipant>"_Stz);
    
    xValWrk = sTildeName;
    pWordTilde->vEncode("pushValue"_Stz,&xValWrk);
    pWordTilde->vEncode("tracePoint"_Stz);
    
    pWordTilde->vEncode((Stz*)pCurrentWord->pFinalRoutine());
    
    pWordTilde->vEncode("exit"_Stz);
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
  ENTRY("vSubDic"_Stz);
  
  mpTask->m_pTokeniser->vNextToken();
  
  s32 sNameSubDic;
  sNameSubDic = mpTask->m_pTokeniser->sToken();
  
  Word* pWordSubDic = mpTask->m_xPathStack.pFindWord(sNameSubDic);
  if ( ! pWordSubDic )
    THROW("No such subDic: %s"_Stz,sNameSubDic.aChar());
  
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
  ENTRY("vTick"_Stz);
  
  mpTask->m_pTokeniser->vNextToken();
  
  //  if ( ! bExtracted ) {
  //    mpTask->m_sException.vSprint("No name: ' ");
  //    mpTask->m_sFile = __FILE__;
  //    throw (i8)__LINE__;
  //  }
  
  Word* pWord = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  
  if ( ! pWord )
    THROW("No word: %s "_Stz,mpTask->m_pTokeniser->sToken().aChar());
  
  if (mpTask->m_bCompiling) {
    Word* pCurrentWord = mpTask->m_pCurrentWord;
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
  
  bool bResult = mpTask->m_xPathStack.bForget(pWord->sName());
  ASSERT(bResult);
  
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
  
  Word* pWord = mpTask->m_pCurrentWord;
  
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
