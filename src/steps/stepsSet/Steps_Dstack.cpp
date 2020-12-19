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
  vAddStep("discard"_Stz,         &Step::vDstackDiscard);
  vAddStep("2discard"_Stz,        &Step::vDstackDiscardTwo);
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
  vAddStep("tuck"_Stz,            &Step::vDstackTuck);
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDstackClass(void)
{
  ENTRY();
  
  i8 i_Val = m_pDataStack->iPopInt(__func__);
  
  Datum xVar;
  
  m_pDataStack->vPeekVal(i_Val,xVar);
  
  eCN eValClass = xVar.eAnyClass();
  
  m_pDataStack->vPushInt(eValClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Step::vDstackDepth(void)
{
  ENTRY();
  
  i8 iDepth = m_pDataStack->nValues();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vDstackDiscard(void)
{
  ENTRY();
  
  m_pDataStack->vDiscardTop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackDiscardTwo(void)
{
  ENTRY();
  
  //! should just discard on stack
  
  m_pDataStack->vDiscardTop();

  m_pDataStack->vDiscardTop();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vDstackDup(void)
{
  ENTRY();
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }

  // could just copy from stack back into stack
  
  //Datum xVarA;
  
  CellSingle xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  
  m_pDataStack->vPushVal(xVarA);
  m_pDataStack->vPushVal(xVarA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackDupNonZero(void)
{
  ENTRY();
  
  // could just copy from stack back into stack
  
  Datum xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  
  if (xVarA.iInt()) {
    m_pDataStack->vPushVal(xVarA);
    m_pDataStack->vPushVal(xVarA);
  }
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Step::vDstackNip(void)
{
  ENTRY();
  
  Datum xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  
  Datum xVarB;
  m_pDataStack->vPopVal(xVarB,__func__);
  
  m_pDataStack->vPushVal(xVarA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Step::vDstackOver(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pDataStack->vPeekVal(1,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Step::vDstackPick(void)
{
  ENTRY();
  
  i8 iPick = m_pDataStack->iPopAsInt(__func__);
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pDataStack->vPeekVal(iPick,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackReverse(void)
{
  ENTRY();
  
  Datum xVal1;
  Datum xVal2;
  Datum xVal3;
  Datum xVal4;
  Datum xVal5;
  Datum xVal6;
  Datum xVal7;
  Datum xVal8;

  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  switch (nValues) {
    case 8: m_pDataStack->vPopVal(xVal8,__func__);
    case 7: m_pDataStack->vPopVal(xVal7,__func__);
    case 6: m_pDataStack->vPopVal(xVal6,__func__);
    case 5: m_pDataStack->vPopVal(xVal5,__func__);
    case 4: m_pDataStack->vPopVal(xVal4,__func__);
    case 3: m_pDataStack->vPopVal(xVal3,__func__);
    case 2: m_pDataStack->vPopVal(xVal2,__func__);
    case 1: m_pDataStack->vPopVal(xVal1,__func__);
  }
  
  switch (nValues) {
    case 8: m_pDataStack->vPushVal(xVal8);
    case 7: m_pDataStack->vPushVal(xVal7);
    case 6: m_pDataStack->vPushVal(xVal6);
    case 5: m_pDataStack->vPushVal(xVal5);
    case 4: m_pDataStack->vPushVal(xVal4);
    case 3: m_pDataStack->vPushVal(xVal3);
    case 2: m_pDataStack->vPushVal(xVal2);
    case 1: m_pDataStack->vPushVal(xVal1);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Step::vDstackRoll(void)
{
  ENTRY();
  
  Datum xVarA;
  Datum xVarX;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPopVal(xVarA,__func__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pDataStack->vPopVal(xVarX,__func__);
    
    m_pReturnStack->vPushVal(xVarX);
  }
  
  m_pDataStack->vPushVal(xVarA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pReturnStack->vPopVal(xVarX,__func__);
    
    m_pDataStack->vPushVal(xVarX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vDstackRollInv(void)
{
  ENTRY();
  
  Datum xVarZ;
  Datum xVarX;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pDataStack->vPopVal(xVarX,__func__);
    
    m_pReturnStack->vPushVal(xVarX);
  }
  
  m_pDataStack->vPopVal(xVarZ,__func__);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pReturnStack->vPopVal(xVarX,__func__);
    
    m_pDataStack->vPushVal(xVarX);
  }
  
  m_pDataStack->vPushVal(xVarZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Step::vDstackRot(void)
{
  ENTRY();
  
  Datum xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  Datum xVarB;
  m_pDataStack->vPopVal(xVarB,__func__);
  Datum xVarC;
  m_pDataStack->vPopVal(xVarC,__func__);

  
  m_pDataStack->vPushVal(xVarB);
  m_pDataStack->vPushVal(xVarA);
  m_pDataStack->vPushVal(xVarC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Step::vDstackRotInv(void)
{
  ENTRY();
  
  Datum xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  Datum xVarB;
  m_pDataStack->vPopVal(xVarB,__func__);
  Datum xVarC;
  m_pDataStack->vPopVal(xVarC,__func__);

  m_pDataStack->vPushVal(xVarA);
  m_pDataStack->vPushVal(xVarC);
  m_pDataStack->vPushVal(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackStk(void)
{
  ENTRY();
  
  m_pDataStack->vDump(&m_oDicStack,"DataStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackSwap(void)
{
  ENTRY();
  
  Datum xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  
  Datum xVarB;
  m_pDataStack->vPopVal(xVarB,__func__);
  
  m_pDataStack->vPushVal(xVarA);
  m_pDataStack->vPushVal(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackTuck(void)
{
  ENTRY();
  
  Datum xVarA;
  m_pDataStack->vPopVal(xVarA,__func__);
  
  Datum xVarB;
  m_pDataStack->vPopVal(xVarB,__func__);

  m_pDataStack->vPushVal(xVarA);
  m_pDataStack->vPushVal(xVarB);
  m_pDataStack->vPushVal(xVarA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
