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
void Step::vDstackTransfer(void)
{
  ENTRY(); // ENTRY("vDstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! m_pDataStack->bPopObject(pPtrTask) )
  THROW_DSTACK();
  Task* pThatTask = _pTask(pPtrTask);
  
  i8 iVal;
  if ( ! m_pDataStack->bPopI8(iVal) )
  THROW_DSTACK();
  
  pThatTask->m_okLock.vGrabWrite();
  pThatTask->m_pForth->m_pDataStack->vPushI8(iVal);
  pThatTask->m_okLock.vDropWrite();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDstackClass(void)
{
  ENTRY(); // ENTRY("vDstackClass"_Stz);
  
  i8 nTh;
  if ( ! m_pDataStack->bPopI8(nTh) )
    THROW_DSTACK();
  
  GenVar xVariable;
  
  m_pDataStack->vPeekValue(nTh,xVariable);
  
  eCN eClass = xVariable.eEmbeddedClass();
  
  m_pDataStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Step::vDstackDepth(void)
{
  ENTRY(); // ENTRY("vDstackDepth"_Stz);
  
  i8 iDepth = m_pDataStack->nValues();
  
  m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vDstackDrop(void)
{
  ENTRY(); // ENTRY("vDstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! m_pDataStack->bPopGenVar(xValA) )
    THROW_DSTACK();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackDropTwo(void)
{
  ENTRY(); // ENTRY("vDstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  GenVar xValB;
  if ( ! m_pDataStack->bPopGenVar(xValA,xValB) )
    THROW_DSTACK();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vDstackDup(void)
{
  ENTRY(); // ENTRY("vDstackDup"_Stz);
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }

  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! m_pDataStack->bPopGenVar(xValA) )
    THROW_DSTACK();
  
  m_pDataStack->vPushGenVar(xValA);
  m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackDupNonZero(void)
{
  ENTRY(); // ENTRY("vDstackDupNonZero"_Stz);
  
  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! m_pDataStack->bPopGenVar(xValA) )
    THROW_DSTACK();
  
  if (xValA.iInt()) {
    m_pDataStack->vPushGenVar(xValA);
    m_pDataStack->vPushGenVar(xValA);
  }
  else
    m_pDataStack->vPushBool(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Step::vDstackNip(void)
{
  ENTRY(); // ENTRY("vDstackNip"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! m_pDataStack->bPopGenVar(xValA,xValB) )
    THROW_DSTACK();
  
  m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Step::vDstackOver(void)
{
  ENTRY(); // ENTRY("vDstackOver"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pDataStack->vPeekValue(1,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Step::vDstackPick(void)
{
  ENTRY(); // ENTRY("vDstackPick"_Stz);
  
  i8 iPick;
  if ( ! m_pDataStack->bPopAsInt(iPick) )
    THROW_DSTACK();
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pDataStack->vPeekValue(iPick,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackReverse(void)
{
  ENTRY(); // ENTRY("vDstackReverse"_Stz);
  
  GenVar xVal0;
  GenVar xVal1;
  GenVar xVal2;
  GenVar xVal3;
  GenVar xVal4;
  GenVar xVal5;
  GenVar xVal6;
  GenVar xVal7;
  
  GenVar* paVal[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! m_pDataStack->bPopI8(nValues) )
    THROW_DSTACK();
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! m_pDataStack->bPopGenVar(paVal[i]) )
      THROW_DSTACK();
  }

  for (i8 i = 0; i < nValues; i++) {
    m_pDataStack->vPushValue(paVal[i]->aEmbeddedStkVal());
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Step::vDstackRoll(void)
{
  ENTRY(); // ENTRY("vDstackRoll"_Stz);
  
  GenVar xValA;
  GenVar xValX;
  
  i8 nValues;
  if ( ! m_pDataStack->bPopI8(nValues) )
    THROW_DSTACK();
  
  if ( ! m_pDataStack->bPopGenVar(xValA) )
    THROW_DSTACK();
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! m_pDataStack->bPopGenVar(xValX) )
      THROW_DSTACK();
    
    m_pReturnStack->vPushGenVar(xValX);
  }
  
  m_pDataStack->vPushGenVar(xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! m_pReturnStack->bPopGenVar(xValX) )
      THROW_DSTACK();
    
    m_pDataStack->vPushGenVar(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vDstackRollInv(void)
{
  ENTRY(); // ENTRY("vDstackRollInv"_Stz);
  
  GenVar xValZ;
  GenVar xValX;
  
  i8 nValues;
  if ( ! m_pDataStack->bPopI8(nValues) )
    THROW_DSTACK();
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! m_pDataStack->bPopGenVar(xValX) )
      THROW_DSTACK();
    
    m_pReturnStack->vPushGenVar(xValX);
  }
  
  if ( ! m_pDataStack->bPopGenVar(xValZ) )
    THROW_DSTACK();
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! m_pReturnStack->bPopGenVar(xValX) )
      THROW_DSTACK();
    
    m_pDataStack->vPushGenVar(xValX);
  }
  
  m_pDataStack->vPushGenVar(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Step::vDstackRot(void)
{
  ENTRY(); // ENTRY("vDstackRot"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  GenVar xValC;
  
  if ( ! m_pDataStack->bPopGenVar(xValA,xValB,xValC) )
    THROW_DSTACK();
  
  m_pDataStack->vPushGenVar(xValB);
  m_pDataStack->vPushGenVar(xValA);
  m_pDataStack->vPushGenVar(xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Step::vDstackRotInv(void)
{
  ENTRY(); // ENTRY("vDstackRotInv"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  GenVar xValC;
  
  if ( ! m_pDataStack->bPopGenVar(xValA,xValB,xValC) )
    THROW_DSTACK();
  
  m_pDataStack->vPushGenVar(xValA);
  m_pDataStack->vPushGenVar(xValC);
  m_pDataStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackStk(void)
{
  ENTRY(); // ENTRY("vDstackStk"_Stz);
  
  m_pDataStack->vContentDump(&m_oDicStack,"DataStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackSwap(void)
{
  ENTRY(); // ENTRY("vDstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! m_pDataStack->bPopGenVar(xValA,xValB) )
    THROW_DSTACK();
  
  m_pDataStack->vPushGenVar(xValA);
  m_pDataStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackTuck(void)
{
  ENTRY(); // ENTRY("vDstackTuck"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! m_pDataStack->bPopGenVar(xValA,xValB) )
    THROW_DSTACK();
  
  m_pDataStack->vPushGenVar(xValA);
  m_pDataStack->vPushGenVar(xValB);
  m_pDataStack->vPushGenVar(xValA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
