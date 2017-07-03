/*============================================================================*/
/* Steps_Common.cpp - common step group                                       */
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// common: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Common(void)
{
  vAddPrimitive("common.fetch"_Str,    &vCommonFetch,      false);
  vAddPrimitive("common.store"_Str,    &vCommonStore,      false);
  vAddPrimitive("common/"_Str,         &vCommonSlash,      true);
  vAddPrimitive("toCommon"_Str,        &vCommonToCommon,   true);

  vAddPrimitive("opFrame.drop"_Str,    &vOpFrameDrop,      false);
  vAddPrimitive("opFrame.push"_Str,    &vOpFramePush,      false);
}

/*----------------------------------------------------------------------------*/
/* common.fetch -- push commons[n] onto dstack                                */
/*----------------------------------------------------------------------------*/
void Task::vCommonFetch(MP_Task mpTask)
{
  ENTRY("vCommonFetch"_Str);
  
  if ( ! mpTask->m_pOpFrame )
    THROW_INVALID_DSTACK("common.fetch"_Str);
  
  i8 iCommon;
  if ( ! mpTask->m_pDataStack->bPop(iCommon) )
    THROW_INVALID_DSTACK("common.fetch"_Str);
  
  ValReg* pValReg = mpTask->m_pOpFrame->pValReg(iCommon);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.store -- pop dstack into common[n]                                  */
/*----------------------------------------------------------------------------*/
void Task::vCommonStore(MP_Task mpTask)
{
  ENTRY("vCommonStore"_Str);
  
  if ( ! mpTask->m_pOpFrame )
    THROW_INVALID_DSTACK("commonStore"_Str);
  
  i8 iCommon;
  if ( ! mpTask->m_pDataStack->bPop(iCommon) )
    THROW_INVALID_DSTACK("commonStore"_Str);
  
  ValReg* pValReg = mpTask->m_pOpFrame->pValReg(iCommon);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common/ -- declare set of common variables                      immediate  */
/*----------------------------------------------------------------------------*/
void Task::vCommonSlash(MP_Task mpTask)
{
ENTRY("vCommonSlash"_Str);

  Dic* pSubDic;
  
  s32 sDicName;
  s32 sVarName;
  
  //Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  ValWrk xValWrk;
  
//  for (;;) {
    
    // get name of common dictionary
    mpTask->m_pTokeniser->vNextToken();
    sDicName = mpTask->m_pTokeniser->sToken();
    
    // test for that dictionary
    Word* pWordDic = mpTask->m_xPathStack.pFindWord(sDicName);
    if (pWordDic) {
      // use that dictionary
      pSubDic = _pDic(pWordDic->aBodyArea());
      mpTask->m_nVariables = pSubDic->nWords();
    }
    else {
      // create a common dictionary      
      pWordDic = new(gpMainDic) Word(sDicName);
      pWordDic->vWordType(WT_OBJECT);
      pWordDic->pAllot(sizeof(Dic));
      
      pSubDic = new((Dic*)pWordDic->aBodyArea()) Dic;
      mpTask->m_nVariables = 0;
    }
    
    // get name of common variable
    mpTask->m_pTokeniser->vNextToken();
    sVarName = mpTask->m_pTokeniser->sToken();
    
    // create new word for common in current dictionary
    Word* pWordCommon = new(pSubDic) Word(sVarName);
    // ** do not set as current word **
    
    // set word as variable
    pWordCommon->vWordType(WT_VARIABLE);
    pWordCommon->vVariable(mpTask->m_nVariables);
    
    // encode the word
    xValWrk = mpTask->m_nVariables;
    pWordCommon->vEncode("pushValue"_Str,&xValWrk);
    pWordCommon->vEncode("common.fetch"_Str);
    pWordCommon->vEncode("exit"_Str);
    
    // accept /
    mpTask->m_pTokeniser->vNextToken();
  
    mpTask->m_nVariables = 0;

//  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toCommon -- compile store value into common variable             immediate */
/*----------------------------------------------------------------------------*/
void Task::vCommonToCommon(MP_Task mpTask)
{
ENTRY("vCommonToCommon"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // get name of common
  mpTask->m_pTokeniser->vNextToken();

  Word* pWordCommon = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  ASSERT(pWordCommon);
 
  ValWrk xValWrk;
  xValWrk = pWordCommon->iVariable();
  
  pCurrentWord->vEncode("pushValue"_Str,&xValWrk);
  pCurrentWord->vEncode("common.store"_Str);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opFrame.drop -- discard op frame, discard op dictionary                    */
/*----------------------------------------------------------------------------*/
void Task::vOpFrameDrop(MP_Task mpTask)
{
  ENTRY("vOpFrameDrop"_Str);
  
  RegFrame* pOpFrame = mpTask->m_pOpFrame;
  
  if ( ! pOpFrame )
    THROW_INVALID_DSTACK("opFrame.drop"_Str);
  
  // discard op frame
  mpTask->m_pOpFrame = nullptr; // mpTask->m_pOpFrame->pNext();
  
  // discard op dic
  mpTask->m_xPathStack.vDiscard();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opFrame.push -- push op frame, push op dictionary                          */
/*----------------------------------------------------------------------------*/
void Task::vOpFramePush(MP_Task mpTask)
{
  ENTRY("vOpFramePush"_Str);
  
  Object* pObjectOpFrame;
  if ( ! mpTask->m_pDataStack->bPop(pObjectOpFrame) )
    THROW_INVALID_DSTACK("opFrame.push"_Str);
  
  // push op frame
  RegFrame* pOpFrame = _pRegFrame(pObjectOpFrame);
  //pOpFrame->vNext(mpTask->m_pOpFrame);
  mpTask->m_pOpFrame = pOpFrame;

  // push op dictionary
  Dic* pOpDic = pOpFrame->pOpDic();
  mpTask->m_xPathStack.vPush(pOpDic);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
