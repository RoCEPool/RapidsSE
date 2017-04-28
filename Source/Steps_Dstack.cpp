/*============================================================================*/
/* Steps_Dstack.cpp - data stack step group                                   */
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

// dstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Dstack(void)
{
  vAddPrimitive("stack.class"_Str,     &vDstackClass,      false);
  vAddPrimitive("depth"_Str,           &vDstackDepth,      false);
  vAddPrimitive("drop"_Str,            &vDstackDrop,       false);
  vAddPrimitive("2drop"_Str,           &vDstackDropTwo,    false);
  vAddPrimitive("dump"_Str,            &vDstackDump,       false);
  vAddPrimitive("dup"_Str,             &vDstackDup,        false);
  vAddPrimitive("?dup"_Str,            &vDstackDupNonZero, false);
  vAddPrimitive("nip"_Str,             &vDstackNip,        false);
  vAddPrimitive("over"_Str,            &vDstackOver,       false);
  vAddPrimitive("pick"_Str,            &vDstackPick,       false);
  vAddPrimitive("reverse"_Str,         &vDstackReverse,    false);
  vAddPrimitive("roll"_Str,            &vDstackRoll,       false);
  vAddPrimitive("-roll"_Str,           &vDstackRollInv,    false);
  vAddPrimitive("rot"_Str,             &vDstackRot,        false);
  vAddPrimitive("-rot"_Str,            &vDstackRotInv,     false);
  vAddPrimitive("stk"_Str,             &vDstackStk,        false);
  vAddPrimitive("swap"_Str,            &vDstackSwap,       false);
  vAddPrimitive("valset>"_Str,         &vDstackToValues,   false);
  vAddPrimitive(">valset"_Str,         &vDstackToValSet,   false);
  vAddPrimitive(">DStack"_Str,         &vDstackTransfer,   false);
  vAddPrimitive("tuck"_Str,            &vDstackTuck,       false);
  
}

/*----------------------------------------------------------------------------*/
/* >DStack -- pop stack of this task and push onto aux stack of that task     */
/*----------------------------------------------------------------------------*/
void Task::vDstackTransfer(NNP_Task nnpTask)
{
  ENTRY("vDstackTransfer"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
  THROW_INVALID_DSTACK(">DStack"_Str);
  Task* pThatTask = _pTask(pPtrTask);
  
  i8 iVal;
  if ( ! nnpTask->m_pDataStack->bPop(iVal) )
  THROW_INVALID_DSTACK(">DStack"_Str);
  
  pThatTask->m_xLock.vGrabWrite();
  pThatTask->m_pDataStack->vPushInt(iVal);
  pThatTask->m_xLock.vDropWrite();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDstackClass(NNP_Task nnpTask)
{
  ENTRY("vDstackClass"_Str);
  
  i8 nTh;
  if ( ! nnpTask->m_pDataStack->bPop(nTh) )
    THROW_INVALID_DSTACK("stack.class"_Str);
  
  ValWsp xValWsp;
  
  nnpTask->m_pDataStack->vPeekValWsp(nTh,xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_pDataStack->vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDstackDepth(NNP_Task nnpTask)
{
  ENTRY("vDstackDepth"_Str);
  
  i8 iDepth = nnpTask->m_pDataStack->iValues();
  
  nnpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDstackDrop(NNP_Task nnpTask)
{
  ENTRY("vDstackDrop"_Str);
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("drop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vDstackDropTwo(NNP_Task nnpTask)
{
  ENTRY("vDstackDrop"_Str);
  
  // should just discard on stack
  
  ValWsp xValA;
  ValWsp xValB;
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("2drop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackDup(NNP_Task nnpTask)
{
  ENTRY("vDstackDup"_Str);
  
  if (nnpTask->m_iTracing) {
    ASSERT(nnpTask->m_iTracing);
  }

  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("dup"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackDupNonZero(NNP_Task nnpTask)
{
  ENTRY("vDstackDupNonZero"_Str);
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("?dup"_Str);
  
  if (xValA.iNestedAsInt()) {
    nnpTask->m_pDataStack->vPushValWsp(xValA);
    nnpTask->m_pDataStack->vPushValWsp(xValA);
  }
  else
    nnpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of data stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vDstackDump(NNP_Task nnpTask)
{
  ENTRY("vDstackDump"_Str);
  
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("dump"_Str);
  
  s48K sWorkspace;
  ValLib::vToString(&xValWsp,sWorkspace);
  Str* aClass = xValWsp.aClass();
  
  eCN eClass = xValWsp.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* pPtrVal = _pPtrVal(&xValWsp);
    Object* pObject = pPtrVal->pPtr();
    eCN ePtdClass   = pObject->eClass();
    vTrace("[%s] %s (%s %s)\n"_Str,
          nnpTask->m_sName.s(),sWorkspace.s(),aClass,aCN[ePtdClass]);
  }
  else if (CN_IPVAL == eClass) {                                         //RAY??
    IpVal* pIpVal = _pIpVal(&xValWsp);
    Cell* pCell = pIpVal->pIpPtr();
    vTrace("[%s] nullptr\n"_Str,nnpTask->m_sName.s());    //RAY: should trace pIpVal
//  vTrace("[%s] %p\n"_Str,nnpTask->m_sName.s(),pIpVal);
  }
  else {
    vTrace("[%s] %s (%s)\n"_Str,
           nnpTask->m_sName.s(),sWorkspace.s(),aClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Task::vDstackNip(NNP_Task nnpTask)
{
  ENTRY("vDstackNip"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("nip"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Task::vDstackOver(NNP_Task nnpTask)
{
  ENTRY("vDstackOver"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pDataStack->vPeekValWsp(1,xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Task::vDstackPick(NNP_Task nnpTask)
{
  ENTRY("vDstackPick"_Str);
  
  i8 iPick;
  if ( ! nnpTask->m_pDataStack->bPopAsInt(iPick) )
    THROW_INVALID_DSTACK("pick"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pDataStack->vPeekValWsp(iPick,xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackReverse(NNP_Task nnpTask)
{
  ENTRY("vDstackReverse"_Str);
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("reverse"_Str);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_pDataStack->bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("reverse"_Str);
  }

  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_pDataStack->vPushValue(apValWsp[i]->pNestedValueSelf());
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Task::vDstackRoll(NNP_Task nnpTask)
{
  ENTRY("vDstackRoll"_Str);
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("roll"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("roll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWsp(xValX);
  }
  
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll"_Str);
    
    nnpTask->m_pDataStack->vPushValWsp(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vDstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRollInv"_Str);
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-roll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWsp(xValX);
  }
  
  if ( ! nnpTask->m_pDataStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-roll"_Str);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll"_Str);
    
    nnpTask->m_pDataStack->vPushValWsp(xValX);
  }
  
  nnpTask->m_pDataStack->vPushValWsp(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Task::vDstackRot(NNP_Task nnpTask)
{
  ENTRY("vDstackRot"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xValB);
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  nnpTask->m_pDataStack->vPushValWsp(xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Task::vDstackRotInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRotInv"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("-rot"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  nnpTask->m_pDataStack->vPushValWsp(xValC);
  nnpTask->m_pDataStack->vPushValWsp(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackStk(NNP_Task nnpTask)
{
  ENTRY("vDstackStk"_Str);
  
  nnpTask->m_pDataStack->vContentDump(nnpTask->m_sName,"DataStack:"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vDstackSwap(NNP_Task nnpTask)
{
  ENTRY("vDstackSwap"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  nnpTask->m_pDataStack->vPushValWsp(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on dstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vDstackToValSet"_Str);
  
  i8 nVal;
  
  if ( ! nnpTask->m_pDataStack->bPop(nVal) )
    THROW_INVALID_DSTACK(">valset"_Str);
  
  if ( ! nnpTask->m_pDataStack->bGuard(nVal) )
    THROW_INVALID_DSTACK(">valset"_Str);
  
  Str* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_pDataStack->bPop(xVal) )
      THROW_INVALID_DSTACK(">valset"_Str);
    
    if (xVal.eClass() == CN_PTRVAL) {
      PtrVal* pPtrVal = _pPtrVal(&xVal);
      Object* pObject = pPtrVal->pPtr();
      eCN eObjectClass   = pObject->eClass();
      
      if (eObjectClass == CN_PLAN) {
        ValWsp xBinVal;
        xBinVal.vAssignBinary(pObject);
        xVal = xBinVal;
      }
    }
    
    xValSet.vAppend(pCursor,xVal.pNestedValueSelf());
  }
  
  //  vTrace("ValSet %lld bytes\n"_Str,xValSet.nSize());
  
  nnpTask->m_pDataStack->vPushValSet(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on dstack                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValues(NNP_Task nnpTask)
{
  ENTRY("vDstackToValues"_Str);
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValSet) )
    THROW_INVALID_DSTACK(">values"_Str);
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  Str* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
    nnpTask->m_pDataStack->vPushValue(pValWsp[iVal]->pNestedValueSelf());
  
  nnpTask->m_pDataStack->vPushInt(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackTuck(NNP_Task nnpTask)
{
  ENTRY("vDstackTuck"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("tuck"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  nnpTask->m_pDataStack->vPushValWsp(xValB);
  nnpTask->m_pDataStack->vPushValWsp(xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
