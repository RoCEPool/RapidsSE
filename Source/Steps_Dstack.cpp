/*============================================================================*/
/* Steps_Dstack.cpp - data stack step group                                   */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  ENTRY("vDstackTransfer");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
  THROW_INVALID_DSTACK(">DStack");
  Task* pThatTask = _pTask(pPtrTask);
  
  i8 iVal;
  if ( ! nnpTask->m_pDataStack->bPop(iVal) )
  THROW_INVALID_DSTACK(">DStack");
  
  pThatTask->m_xLock.vGrabWrite();
  pThatTask->m_pDataStack->vPush(iVal);
  pThatTask->m_xLock.vDropWrite();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDstackClass(NNP_Task nnpTask)
{
  ENTRY("vDstackClass");
  
  i8 nTh;
  if ( ! nnpTask->m_pDataStack->bPop(nTh) )
    THROW_INVALID_DSTACK("stack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_pDataStack->vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_pDataStack->vPush(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDstackDepth(NNP_Task nnpTask)
{
  ENTRY("vDstackDepth");
  
  i8 iDepth = nnpTask->m_pDataStack->iValues();
  
  nnpTask->m_pDataStack->vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDstackDrop(NNP_Task nnpTask)
{
  ENTRY("vDstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vDstackDropTwo(NNP_Task nnpTask)
{
  ENTRY("vDstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  ValWsp xValB;
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("2drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackDup(NNP_Task nnpTask)
{
  ENTRY("vDstackDup");
  
  if (nnpTask->m_iTracing) {
    ASSERT(nnpTask->m_iTracing);
  }

  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("dup");
  
  nnpTask->m_pDataStack->vPush(xValA);
  nnpTask->m_pDataStack->vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackDupNonZero(NNP_Task nnpTask)
{
  ENTRY("vDstackDupNonZero");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("?dup");
  
  if (xValA.iAsInt()) {
    nnpTask->m_pDataStack->vPush(xValA);
    nnpTask->m_pDataStack->vPush(xValA);
  }
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of data stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vDstackDump(NNP_Task nnpTask)
{
  ENTRY("vDstackDump");
  
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("dump");
  
  s48K sWorkspace;
  ValLib::vToString(&xValWsp,sWorkspace.aStr());
  char* aClass = xValWsp.aClass();
  
  eCN eClass = xValWsp.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* pPtrVal = _pPtrVal(&xValWsp);
    Object* pObject = pPtrVal->pPtr();
    eCN ePtdClass   = pObject->eClass();
    vTrace("[%s] %s (%s %s)\n"_Str,
          nnpTask->m_sName.aStr(),sWorkspace.aStr(),aClass,aCN[ePtdClass]);
  }
  else if (CN_IPVAL == eClass) {                                         //RAY??
    IpVal* pIpVal = _pIpVal(&xValWsp);
    Cell* pCell = pIpVal->pIpPtr();
    vTrace("[%s] nullptr\n"_Str,nnpTask->m_sName.aStr());    //RAY: should trace pIpVal
//  vTrace("[%s] %p\n"_Str,nnpTask->m_sName.aStr(),pIpVal);
  }
  else {
    vTrace("[%s] %s (%s)\n"_Str,
           nnpTask->m_sName.aStr(),sWorkspace.aStr(),aClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Task::vDstackNip(NNP_Task nnpTask)
{
  ENTRY("vDstackNip");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("nip");
  
  nnpTask->m_pDataStack->vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Task::vDstackOver(NNP_Task nnpTask)
{
  ENTRY("vDstackOver");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pDataStack->vPeek(1,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Task::vDstackPick(NNP_Task nnpTask)
{
  ENTRY("vDstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("pick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pDataStack->vPeek(iPick,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackReverse(NNP_Task nnpTask)
{
  ENTRY("vDstackReverse");
  
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
    THROW_INVALID_DSTACK("reverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_pDataStack->bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("reverse");
  }

  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_pDataStack->vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Task::vDstackRoll(NNP_Task nnpTask)
{
  ENTRY("vDstackRoll");
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("roll");
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll");
    
    nnpTask->m_pReturnStack->vPush(&xValX);
  }
  
  nnpTask->m_pDataStack->vPush(&xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll");
    
    nnpTask->m_pDataStack->vPush(&xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vDstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRollInv");
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll");
    
    nnpTask->m_pReturnStack->vPush(&xValX);
  }
  
  if ( ! nnpTask->m_pDataStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-roll");
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll");
    
    nnpTask->m_pDataStack->vPush(&xValX);
  }
  
  nnpTask->m_pDataStack->vPush(&xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Task::vDstackRot(NNP_Task nnpTask)
{
  ENTRY("vDstackRot");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot");
  
  nnpTask->m_pDataStack->vPush(&xValB);
  nnpTask->m_pDataStack->vPush(&xValA);
  nnpTask->m_pDataStack->vPush(&xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Task::vDstackRotInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRotInv");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("-rot");
  
  nnpTask->m_pDataStack->vPush(&xValA);
  nnpTask->m_pDataStack->vPush(&xValC);
  nnpTask->m_pDataStack->vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackStk(NNP_Task nnpTask)
{
  ENTRY("vDstackStk");
  
  nnpTask->m_pDataStack->vContentDump(nnpTask->m_sName,"DataStack:"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vDstackSwap(NNP_Task nnpTask)
{
  ENTRY("vDstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap");
  
  nnpTask->m_pDataStack->vPush(&xValA);
  nnpTask->m_pDataStack->vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on dstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vDstackToValSet");
  
  i8 nVal;
  
  if ( ! nnpTask->m_pDataStack->bPop(nVal) )
    THROW_INVALID_DSTACK(">valset");
  
  if ( ! nnpTask->m_pDataStack->bGuard(nVal) )
    THROW_INVALID_DSTACK(">valset");
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_pDataStack->bPop(xVal) )
      THROW_INVALID_DSTACK(">valset");
    
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
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n"_Str,xValSet.nSize());
  
  nnpTask->m_pDataStack->vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on dstack                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValues(NNP_Task nnpTask)
{
  ENTRY("vDstackToValues");
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValSet) )
    THROW_INVALID_DSTACK(">values");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
    nnpTask->m_pDataStack->vPush(pValWsp[iVal]);
  
  nnpTask->m_pDataStack->vPush(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Task::vDstackTuck(NNP_Task nnpTask)
{
  ENTRY("vDstackTuck");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("tuck");
  
  nnpTask->m_pDataStack->vPush(&xValA);
  nnpTask->m_pDataStack->vPush(&xValB);
  nnpTask->m_pDataStack->vPush(&xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
