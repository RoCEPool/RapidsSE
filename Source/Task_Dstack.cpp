/*============================================================================*/
/* Task_Dstack.cpp - task data stack group                                    */
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
  vAddPrimitive("stack.class",     &vDstackClass,      false);
  vAddPrimitive("depth",           &vDstackDepth,      false);
  vAddPrimitive("drop",            &vDstackDrop,       false);
  vAddPrimitive("2drop",           &vDstackDropTwo,    false);
  vAddPrimitive("dump",            &vDstackDump,       false);
  vAddPrimitive("dup",             &vDstackDup,        false);
  vAddPrimitive("?dup",            &vDstackDupNonZero, false);
  vAddPrimitive("nip",             &vDstackNip,        false);
  vAddPrimitive("over",            &vDstackOver,       false);
  vAddPrimitive("pick",            &vDstackPick,       false);
  vAddPrimitive("reverse",         &vDstackReverse,    false);
  vAddPrimitive("roll",            &vDstackRoll,       false);
  vAddPrimitive("-roll",           &vDstackRollInv,    false);
  vAddPrimitive("rot",             &vDstackRot,        false);
  vAddPrimitive("-rot",            &vDstackRotInv,     false);
  vAddPrimitive("swap",            &vDstackSwap,       false);
  vAddPrimitive("valset>",         &vDstackToValues,   false);
  vAddPrimitive(">valset",         &vDstackToValSet,   false);
  vAddPrimitive("tuck",            &vDstackTuck,       false);
}

/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDstackClass(NNP_Task nnpTask)
{
  ENTRY("vDstackClass");
  
  i8 nTh;
  if ( ! nnpTask->m_xDataStack.bPop(nTh) )
    THROW_INVALID_DSTACK("stack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_xDataStack.vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_xDataStack.vPush(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDstackDepth(NNP_Task nnpTask)
{
  ENTRY("vDstackDepth");
  
  i8 iDepth = nnpTask->m_xDataStack.iValues();
  
  nnpTask->m_xDataStack.vPush(iDepth);
  
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
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
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
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("2drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDstackDup(NNP_Task nnpTask)
{
  ENTRY("vDstackDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("dup");
  
  nnpTask->m_xDataStack.vPush(xValA);
  nnpTask->m_xDataStack.vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of param stack if non zero                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackDupNonZero(NNP_Task nnpTask)
{
  ENTRY("vDstackDupNonZero");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("?dup");
  
  if (xValA.iAsInt()) {
    nnpTask->m_xDataStack.vPush(xValA);
    nnpTask->m_xDataStack.vPush(xValA);
  }
  else
    nnpTask->m_xDataStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of param stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vDstackDump(NNP_Task nnpTask)
{
  ENTRY("vDstackDump");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xDataStack.bPop(xValWsp) )
    THROW_INVALID_DSTACK("dump");
  
  xValWsp.vToString(sWorkspace.a());
  char* aClass = xValWsp.aClass();
  
  eCN eClass = xValWsp.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* pPtrVal = xValWsp.pAsPtrVal();
    Object* pObject = pPtrVal->pObject();
    if (pObject) {
      eCN ePtdClass   = pObject->eClass();
      vTrace("[%s] %s (%s %s)\n",
             nnpTask->m_sName.a(),sWorkspace.a(),aClass,pCN[ePtdClass]);
    }
    else {
      vTrace("[%s] nullptr\n",nnpTask->m_sName.a());
    }
  }
  else {
    vTrace("[%s] %s (%s)\n",
           nnpTask->m_sName.a(),sWorkspace.a(),aClass);
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
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("nip");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackOver(NNP_Task nnpTask)
{
  ENTRY("vDstackOver");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xDataStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- place copy of nth value in stack onto stack                        */
/*----------------------------------------------------------------------------*/
void Task::vDstackPick(NNP_Task nnpTask)
{
  ENTRY("vDstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("pick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xDataStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
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
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("reverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xDataStack.bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("reverse");
  }

  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xDataStack.vPush(apValWsp[i]);
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
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("roll");
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA) )
    THROW_INVALID_DSTACK("roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("roll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  nnpTask->m_xDataStack.vPush(&xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_DSTACK("roll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
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
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("-roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("-roll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  if ( ! nnpTask->m_xDataStack.bPop(xValZ) )
    THROW_INVALID_DSTACK("-roll");
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_DSTACK("-roll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  nnpTask->m_xDataStack.vPush(&xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values (rotate C to top) ( C B A -- B A C )            */
/*----------------------------------------------------------------------------*/
void Task::vDstackRot(NNP_Task nnpTask)
{
  ENTRY("vDstackRot");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot");
  
  nnpTask->m_xDataStack.vPush(&xValB);
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- inv rotate top 3 values (rotate A to bottom) ( C B A -- A C B )    */
/*----------------------------------------------------------------------------*/
void Task::vDstackRotInv(NNP_Task nnpTask)
{
  ENTRY("vDstackRotInv");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("-rot");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValC);
  nnpTask->m_xDataStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vDstackSwap(NNP_Task nnpTask)
{
  ENTRY("vDstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on dstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vDstackToValSet");
  
  i8 nVal;
  
  if ( ! nnpTask->m_xDataStack.bPop(nVal) )
  THROW_INVALID_DSTACK(">valset");
  
  if ( ! nnpTask->m_xDataStack.bGuard(nVal) )
  THROW_INVALID_DSTACK(">valset");
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_xDataStack.bPop(xVal) )
    THROW_INVALID_DSTACK(">valset");
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n",xValSet.iSize());
  
  nnpTask->m_xDataStack.vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on dstack                               */
/*----------------------------------------------------------------------------*/
void Task::vDstackToValues(NNP_Task nnpTask)
{
  ENTRY("vDstackToValues");
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValSet) )
    THROW_INVALID_DSTACK(">values");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
    nnpTask->m_xDataStack.vPush(pValWsp[iVal]);
  
  nnpTask->m_xDataStack.vPush(nVal);
  
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
  
  if ( ! nnpTask->m_xDataStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("tuck");
  
  nnpTask->m_xDataStack.vPush(&xValA);
  nnpTask->m_xDataStack.vPush(&xValB);
  nnpTask->m_xDataStack.vPush(&xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
