/*============================================================================*/
/* Steps_Dstack.cpp - data stack step group                                   */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dstack(void)
{
  vAddStep("stack.class"_Stz,     &Step::vDstackClass);
  vAddStep("depth"_Stz,           &Step::vDstackDepth);
  vAddStep("drop"_Stz,            &Step::vDstackDrop);
  vAddStep("2drop"_Stz,           &Step::vDstackDropTwo);
  vAddStep("dump"_Stz,            &Step::vDstackDump);
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
void Step::vDstackTransfer(MP_Task mpTask)
{
  ENTRY("vDstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
  THROW_INVALID_DSTACK(">DStack"_Stz);
  Task* pThatTask = _pTask(pPtrTask);
  
  i8 iVal;
  if ( ! mpTask->m_pDataStack->bPop(iVal) )
  THROW_INVALID_DSTACK(">DStack"_Stz);
  
  pThatTask->m_oLock.vGrabWrite();
  pThatTask->m_pDataStack->vPushI8(iVal);
  pThatTask->m_oLock.vDropWrite();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDstackClass(MP_Task mpTask)
{
  ENTRY("vDstackClass"_Stz);
  
  i8 nTh;
  if ( ! mpTask->m_pDataStack->bPop(nTh) )
    THROW_INVALID_DSTACK("stack.class"_Stz);
  
  GenVar xVariable;
  
  mpTask->m_pDataStack->vPeekValue(nTh,xVariable);
  
  eCN eClass = xVariable.aNestedValue()->eClass();
  
  mpTask->m_pDataStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Step::vDstackDepth(MP_Task mpTask)
{
  ENTRY("vDstackDepth"_Stz);
  
  i8 iDepth = mpTask->m_pDataStack->nValues();
  
  mpTask->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vDstackDrop(MP_Task mpTask)
{
  ENTRY("vDstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! mpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("drop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackDropTwo(MP_Task mpTask)
{
  ENTRY("vDstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  GenVar xValB;
  if ( ! mpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("2drop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vDstackDup(MP_Task mpTask)
{
  ENTRY("vDstackDup"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }

  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! mpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("dup"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackDupNonZero(MP_Task mpTask)
{
  ENTRY("vDstackDupNonZero"_Stz);
  
  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! mpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("?dup"_Stz);
  
  if (xValA.iNestedAsInt()) {
    mpTask->m_pDataStack->vPushGenVar(xValA);
    mpTask->m_pDataStack->vPushGenVar(xValA);
  }
  else
    mpTask->m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of data stack                                             */
/*----------------------------------------------------------------------------*/
void Step::vDstackDump(MP_Task mpTask)
{
  ENTRY("vDstackDump"_Stz);
  
  GenVar xVariable;
  if ( ! mpTask->m_pDataStack->bPop(xVariable) )
    THROW_INVALID_DSTACK("dump"_Stz);
  
  s48K sString;
  xVariable.vToString(sString);
  Stz* aClass = xVariable.aClass();
  
  eCN eClass = xVariable.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* aPtrVal = _aPtrVal(&xVariable);
    Object* pObject = aPtrVal->pObj();
    eCN ePtdClass   = pObject->eClass();
    vTrace("[%s] %s (%s %s)\n"_Stz,
           mpTask->aJobName(),
           sString.aChar(),
           aClass,
           aCN[ePtdClass]);
  }
  else if (CN_FIPVAL == eClass) {                                         //RAY??
    IpVal* aIpVal = _aIpVal(&xVariable);
    Cell* aCell = aIpVal->aIpPtr();
    vTrace("[%s] nullptr\n"_Stz,
           mpTask->aJobName());    //RAY: should trace pIpVal
  }
  else {
    vTrace("[%s] %s (%s)\n"_Stz,
           mpTask->aJobName(),
           sString.aChar(),
           aClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Step::vDstackNip(MP_Task mpTask)
{
  ENTRY("vDstackNip"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("nip"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Step::vDstackOver(MP_Task mpTask)
{
  ENTRY("vDstackOver"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pDataStack->vPeekValue(1,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Step::vDstackPick(MP_Task mpTask)
{
  ENTRY("vDstackPick"_Stz);
  
  i8 iPick;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPick) )
    THROW_INVALID_DSTACK("pick"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pDataStack->vPeekValue(iPick,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackReverse(MP_Task mpTask)
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
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("reverse"_Stz);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pDataStack->bPop(apValWrk[i]) )
      THROW_INVALID_DSTACK("reverse"_Stz);
  }

  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pDataStack->vPushValue(apValWrk[i]->aNestedValue());
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Step::vDstackRoll(MP_Task mpTask)
{
  ENTRY("vDstackRoll"_Stz);
  
  GenVar xValA;
  GenVar xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("roll"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xValA) )
    THROW_INVALID_DSTACK("roll"_Stz);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll"_Stz);
    
    mpTask->m_pReturnStack->vPushGenVar(xValX);
  }
  
  mpTask->m_pDataStack->vPushGenVar(xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("roll"_Stz);
    
    mpTask->m_pDataStack->vPushGenVar(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vDstackRollInv(MP_Task mpTask)
{
  ENTRY("vDstackRollInv"_Stz);
  
  GenVar xValZ;
  GenVar xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-roll"_Stz);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll"_Stz);
    
    mpTask->m_pReturnStack->vPushGenVar(xValX);
  }
  
  if ( ! mpTask->m_pDataStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-roll"_Stz);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-roll"_Stz);
    
    mpTask->m_pDataStack->vPushGenVar(xValX);
  }
  
  mpTask->m_pDataStack->vPushGenVar(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Step::vDstackRot(MP_Task mpTask)
{
  ENTRY("vDstackRot"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  GenVar xValC;
  
  if ( ! mpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("rot"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xValB);
  mpTask->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pDataStack->vPushGenVar(xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Step::vDstackRotInv(MP_Task mpTask)
{
  ENTRY("vDstackRotInv"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  GenVar xValC;
  
  if ( ! mpTask->m_pDataStack->bPop(xValA,xValB,xValC) )
    THROW_INVALID_DSTACK("-rot"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pDataStack->vPushGenVar(xValC);
  mpTask->m_pDataStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackStk(MP_Task mpTask)
{
  ENTRY("vDstackStk"_Stz);
  
  mpTask->m_pDataStack->vContentDump(
    (Stz*)mpTask->aJobName(),
    "DataStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackSwap(MP_Task mpTask)
{
  ENTRY("vDstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pDataStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackTuck(MP_Task mpTask)
{
  ENTRY("vDstackTuck"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pDataStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("tuck"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xValA);
  mpTask->m_pDataStack->vPushGenVar(xValB);
  mpTask->m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
