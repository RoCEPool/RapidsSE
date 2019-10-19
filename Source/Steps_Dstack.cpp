/*============================================================================*/
/* Steps_Dstack.cpp - data stack step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dstack(void)
{
  vAddStep("stack.class"_Stz,     &Step::vDstackClass);
  vAddStep("depth"_Stz,           &Step::vDstackDepth);
  vAddStep("drop"_Stz,            &Step::vDstackDrop);
  vAddStep("2drop"_Stz,           &Step::vDstackDropTwo);
  vAddStep("dup"_Stz,             &Step::vDstackDup);
  vAddStep("?dup"_Stz,            &Step::vDstackDupNonZero);
  vAddStep("nip"_Stz,             &Step::vDstackNip);
  vAddStep("over"_Stz,            &Step::vDstackOver);
  vAddStep("pick"_Stz,            &Step::vDstackPick);
  vAddStep("reverse"_Stz,         &Step::vDstackReverse);
  vAddStep("roll"_Stz,            &Step::vDstackRoll);
  vAddStep("-roll"_Stz,           &Step::vDstackRollInv);
  vAddStep("rot"_Stz,             &Step::vDstackRot);
  vAddStep("-rot"_Stz,            &Step::vDstackRotInv);
  vAddStep("stk"_Stz,             &Step::vDstackStk);
  vAddStep("swap"_Stz,            &Step::vDstackSwap);
  vAddStep(">DStack"_Stz,         &Step::vDstackTransfer);
  vAddStep("tuck"_Stz,            &Step::vDstackTuck);
  
}

/*----------------------------------------------------------------------------*/
/* >DStack -- pop stack of this task and push onto aux stack of that task     */
/*----------------------------------------------------------------------------*/
void Step::vDstackTransfer(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTask) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Task* pThatTask = _pTask(pPtrTask);
  
  i8 iVal;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iVal) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pThatTask->m_okLock.vGrabWrite();
  pThatTask->m_pForth->m_pDataStack->vPushI8(iVal);
  pThatTask->m_okLock.vDropWrite();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDstackClass(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackClass"_Stz);
  
  i8 nTh;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(nTh) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pDataStack->vPeekValue(nTh,xVariable);
  
  eCN eClass = xVariable.aNestedValue()->eClass();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Step::vDstackDepth(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackDepth"_Stz);
  
  i8 iDepth = mpTask->m_pForth->m_pDataStack->nValues();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vDstackDrop(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackDropTwo(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  GenVar xValB;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA,xValB) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vDstackDup(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackDup"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }

  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackDupNonZero(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackDupNonZero"_Stz);
  
  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  if (xValA.iInt()) {
    mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
    mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  }
  else
    mpTask->m_pForth->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Step::vDstackNip(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackNip"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA,xValB) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Step::vDstackOver(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackOver"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pDataStack->vPeekValue(1,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Step::vDstackPick(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackPick"_Stz);
  
  i8 iPick;
  if ( ! mpTask->m_pForth->m_pDataStack->bPopAsInt(iPick) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pDataStack->vPeekValue(iPick,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackReverse(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackReverse"_Stz);
  
  GenVar xVal0;
  GenVar xVal1;
  GenVar xVal2;
  GenVar xVal3;
  GenVar xVal4;
  GenVar xVal5;
  GenVar xVal6;
  GenVar xVal7;
  
  GenVar* apValWrk[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(nValues) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(apValWrk[i]) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  }

  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pForth->m_pDataStack->vPushValue(apValWrk[i]->aNestedValue());
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Step::vDstackRoll(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackRoll"_Stz);
  
  GenVar xValA;
  GenVar xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(nValues) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValX) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
    
    mpTask->m_pForth->m_pReturnStack->vPushGenVar(xValX);
  }
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pForth->m_pReturnStack->bPop(xValX) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
    
    mpTask->m_pForth->m_pDataStack->vPushGenVar(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vDstackRollInv(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackRollInv"_Stz);
  
  GenVar xValZ;
  GenVar xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(nValues) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValX) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
    
    mpTask->m_pForth->m_pReturnStack->vPushGenVar(xValX);
  }
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValZ) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pForth->m_pReturnStack->bPop(xValX) )
      vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
    
    mpTask->m_pForth->m_pDataStack->vPushGenVar(xValX);
  }
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Step::vDstackRot(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackRot"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  GenVar xValC;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA,xValB,xValC) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValB);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Step::vDstackRotInv(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackRotInv"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  GenVar xValC;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA,xValB,xValC) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValC);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackStk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackStk"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vContentDump(
    (Stz*)mpTask->szJobName(),
    "DataStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackSwap(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA,xValB) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackTuck(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDstackTuck"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValA,xValB) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValB);
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
