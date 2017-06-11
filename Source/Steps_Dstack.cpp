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
  
  ValWrk xValWrk;
  
  nnpTask->m_pDataStack->vPeekValWrk(nTh,xValWrk);
  
  eCN eClass = xValWrk.pNestedValue()->eClass();
  
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
  
  ValWrk xValA;
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
  
  ValWrk xValA;
  ValWrk xValB;
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
  
  ValWrk xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("dup"_Str);
  
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackDupNonZero(NNP_Task nnpTask)
{
  ENTRY("vDstackDupNonZero"_Str);
  
  // could just copy from stack back into stack
  
  ValWrk xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("?dup"_Str);
  
  if (xValA.iNestedAsInt()) {
    nnpTask->m_pDataStack->vPushValWrk(xValA);
    nnpTask->m_pDataStack->vPushValWrk(xValA);
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
  
  
  ValWrk xValWrk;
  if ( ! nnpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("dump"_Str);
  
  s48K sWorkspace;
  ValLib::vToString(&xValWrk,sWorkspace);
  Str* aClass = xValWrk.aClass();
  
  eCN eClass = xValWrk.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* pPtrVal = _pPtrVal(&xValWrk);
    Object* pObject = pPtrVal->pPtr();
    eCN ePtdClass   = pObject->eClass();
    vTrace("[%s] %s (%s %s)\n"_Str,
          nnpTask->m_sName.s(),sWorkspace.s(),aClass,aCN[ePtdClass]);
  }
  else if (CN_IPVAL == eClass) {                                         //RAY??
    IpVal* pIpVal = _pIpVal(&xValWrk);
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
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("nip"_Str);
  
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Task::vDstackOver(NNP_Task nnpTask)
{
  ENTRY("vDstackOver"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  nnpTask->m_pDataStack->vPeekValWrk(1,xValWrk);
  
  nnpTask->m_pDataStack->vPushValWrk(xValWrk);
  
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
  
  ValWrk xValWrk;
  
  nnpTask->m_pDataStack->vPeekValWrk(iPick,xValWrk);
  
  nnpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackReverse(NNP_Task nnpTask)
{
  ENTRY("vDstackReverse"_Str);
  
  ValWrk xVal0;
  ValWrk xVal1;
  ValWrk xVal2;
  ValWrk xVal3;
  ValWrk xVal4;
  ValWrk xVal5;
  ValWrk xVal6;
  ValWrk xVal7;
  
  ValWrk* apValWrk[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("reverse"_Str);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_pDataStack->bPop(apValWrk[i]) )
      THROW_INVALID_DSTACK("reverse"_Str);
  }

  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_pDataStack->vPushValue(apValWrk[i]->pNestedValueSelf());
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Task::vDstackRoll(NNP_Task nnpTask)
{
  ENTRY("vDstackRoll"_Str);
  
  ValWrk xValA;
  ValWrk xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("roll"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("roll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWrk(xValX);
  }
  
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll"_Str);
    
    nnpTask->m_pDataStack->vPushValWrk(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vDstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRollInv"_Str);
  
  ValWrk xValZ;
  ValWrk xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-roll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWrk(xValX);
  }
  
  if ( ! nnpTask->m_pDataStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-roll"_Str);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll"_Str);
    
    nnpTask->m_pDataStack->vPushValWrk(xValX);
  }
  
  nnpTask->m_pDataStack->vPushValWrk(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Task::vDstackRot(NNP_Task nnpTask)
{
  ENTRY("vDstackRot"_Str);
  
  ValWrk xValA;
  ValWrk xValB;
  ValWrk xValC;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot"_Str);
  
  nnpTask->m_pDataStack->vPushValWrk(xValB);
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  nnpTask->m_pDataStack->vPushValWrk(xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Task::vDstackRotInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRotInv"_Str);
  
  ValWrk xValA;
  ValWrk xValB;
  ValWrk xValC;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("-rot"_Str);
  
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  nnpTask->m_pDataStack->vPushValWrk(xValC);
  nnpTask->m_pDataStack->vPushValWrk(xValB);
  
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
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap"_Str);
  
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  nnpTask->m_pDataStack->vPushValWrk(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on dstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vDstackToValSet"_Str);
  
  i8 nValues;
  
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK(">valset"_Str);
  
  if ( ! nnpTask->m_pDataStack->bGuard(nValues) )
    THROW_INVALID_DSTACK(">valset"_Str);
  
  Str* pCursor = nullptr;
  ValSet xValSet;
  
  ValWrk xValWrk;
  for (i8 iValue = 0; iValue < nValues; iValue++) {
    
    if ( ! nnpTask->m_pDataStack->bPop(xValWrk) )
      THROW_INVALID_DSTACK(">valset"_Str);
    
    if (xValWrk.pNestedValue()->eClass() == CN_PTRVAL) {
      PtrVal* pPtrVal = _pPtrVal(xValWrk.pNestedValue());
      Object* pObject = pPtrVal->pPtr();
      eCN eObjectClass = pObject->eClass();
      
      if (eObjectClass == CN_PLAN)
        xValWrk.vAssignBinary(pObject);
    }
    
    xValSet.vAppend(pCursor,xValWrk.pNestedValueSelf());
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
  Value* pValue[128];
  
  Str* pCursor = nullptr;
  while (xValSet.bExtractValue(pCursor,pValue[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--) {
//    vTrace("Task::vDstackToValues: %lld %s\n"_Str,
//           pValue[iVal]->eClass(),
//           aCN[pValue[iVal]->eClass()]);
    nnpTask->m_pDataStack->vPushValue(pValue[iVal]);
  }
  
  nnpTask->m_pDataStack->vPushInt(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackTuck(NNP_Task nnpTask)
{
  ENTRY("vDstackTuck"_Str);
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("tuck"_Str);
  
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  nnpTask->m_pDataStack->vPushValWrk(xValB);
  nnpTask->m_pDataStack->vPushValWrk(xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
