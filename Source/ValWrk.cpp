/*============================================================================*/
/* ValWrk.cpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ValWrk* Values::_pValWrk(void* aAddr)
{
  ValWrk* pValWrk = reinterpret_cast<ValWrk*>(aAddr);
  
  RASSERT( !pValWrk || CN_VALWRK == pValWrk->eClass() );
  
  return pValWrk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValWrk::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: ValWrk class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValWrk::ValWrk(void)
: Object(CN_VALWRK)
{
  _precondition_(_MAGIC_,CN_VALWRK);                    //! need to add _FOOTER_
  
  _postcondition_(_MAGIC_,CN_VALWRK);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign/copy a boolean into workspace                                       */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Bol xBol)
{
  BolVal* pNestedBolVal = (BolVal*)pNestedValue();
  
  // set nested class
  pNestedBolVal->vClass(CN_BOLVAL);
  
  // assign int into the nested intval
  *pNestedBolVal = xBol;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a date into workspace                                          */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Dte xDte)
{
  DteVal* pNestedDteVal = (DteVal*)pNestedValue();
  
  // set nested class
  pNestedDteVal->vClass(CN_DTEVAL);
  
  // assign int into the nested intval
  *pNestedDteVal = xDte;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a float into workspace                                         */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(d8 dFlt)
{
  FltVal* pNestedFltVal = (FltVal*)pNestedValue();
  
  // set nested class
  pNestedFltVal->vClass(CN_FLTVAL);
  
  // assign flt into the nested fltval
  *pNestedFltVal = dFlt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(i8 iInt)
{
  IntVal* pNestedIntVal = (IntVal*)pNestedValue();
  
  // set nested class
  pNestedIntVal->vClass(CN_INTVAL);
  
  // assign int into the nested intval
  *pNestedIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an interval into workspace                                     */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Ivl iIvl)
{
  IvlVal* pNestedIvlVal = (IvlVal*)pNestedValue();
  
  // set nested class
  pNestedIvlVal->vClass(CN_IVLVAL);
  
  // assign int into the nested intval
  *pNestedIvlVal = iIvl;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Object* pPtr)
{
  PtrVal* pNestedPtrVal = (PtrVal*)pNestedValue();
  
  // set nested class
  pNestedPtrVal->vClass(CN_PTRVAL);
  
  // assign ptr into the nested ptrval
  *pNestedPtrVal = pPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Str* pStr)
{
  StrVal* pNestedStrVal = (StrVal*)pNestedValue();
  
  // set nested class
  pNestedStrVal->vClass(CN_STRVAL);
  
  pNestedStrVal->vAssign(pStr);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an time into workspace                                         */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Tme xTme)
{
  TmeVal* pNestedTmeVal = (TmeVal*)pNestedValue();
  
  // set nested class
  pNestedTmeVal->vClass(CN_TMEVAL);
  
  // assign tme into the nested intval
  *pNestedTmeVal = xTme;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an timestamp into workspace                                    */
/*----------------------------------------------------------------------------*/
void ValWrk::operator =(Tsp xTsp)
{
  TspVal* pNestedTspVal = (TspVal*)pNestedValue();
  
  // set nested class
  pNestedTspVal->vClass(CN_TSPVAL);
  
  // assign tsp into the nested intval
  *pNestedTspVal = xTsp;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void ValWrk::vAssignBinary(Object* pObject)
{
  _precondition_(_MAGIC_,CN_VALWRK);
  
  eCN eClassNo = pObject->eClass();
  
  BinVal* pNestedBinVal = (BinVal*)pNestedValue();
  
  // set nested class
  pNestedBinVal->vClass(CN_BINVAL);
  
  *pNestedBinVal = pObject;
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* ValWrk::aWspArea(void)
{
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
Value* ValWrk::pNestedValue(void)
{
  _precondition_(_MAGIC_);
  
  return (Value*)m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
Value* ValWrk::pNestedValueSelf(void)
{
  _precondition_(_MAGIC_);
  
  return (Value*)m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
BinVal* ValWrk::pNestedBinVal(void)
{
  _precondition_(_MAGIC_);
  
  BinVal* pNestedBinVal = reinterpret_cast<BinVal*>(pNestedValue());
  
  return pNestedBinVal;
}

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 ValWrk::iNestedAsInt(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  i8 i = pNestedIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
IntVal* ValWrk::pNestedIntVal(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  return pNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
PtrVal* ValWrk::pNestedPtrVal(void)
{
  _precondition_(_MAGIC_);
  
  PtrVal* pNestedPtrVal = reinterpret_cast<PtrVal*>(pNestedValue());
  
  return pNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
StrVal* ValWrk::pNestedStrVal(void)
{
  _precondition_(_MAGIC_);
  
  StrVal* pNestedStrVal = reinterpret_cast<StrVal*>(pNestedValue());
  
  return pNestedStrVal;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ValWrk::vToString(Str* aWorkspace)
{
  *aWorkspace = reinterpret_cast<void*>(pNestedValue());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ValWrk::aFooter(void)
{
  _precondition_(_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
