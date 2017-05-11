/*============================================================================*/
/* ValWsp.cpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ValWsp* Values::_pValWsp(void* aAddr)
{
  ValWsp* pValWsp = reinterpret_cast<ValWsp*>(aAddr);
  
  RASSERT( !pValWsp || CN_VALWSP == pValWsp->eClass() );
  
  return pValWsp;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValWsp::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: ValWsp class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValWsp::ValWsp(void)
: Object(CN_VALWSP)
{
  _precondition_(_MAGIC_,CN_VALWSP);                    //! need to add _FOOTER_
  
  _postcondition_(_MAGIC_,CN_VALWSP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign/copy a date into workspace                                          */
/*----------------------------------------------------------------------------*/
void ValWsp::operator =(Dte xDte)
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
void ValWsp::operator =(d8 dFlt)
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
void ValWsp::operator =(i8 iInt)
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
void ValWsp::operator =(Ivl iIvl)
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
void ValWsp::operator =(Object* pPtr)
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
void ValWsp::operator =(Str* pStr)
{
  StrVal* pNestedStrVal = (StrVal*)pNestedValue();
  
  // set nested class
  pNestedStrVal->vClass(CN_STRVAL);
  
  pNestedStrVal->vAssign(pStr);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an timestamp into workspace                                    */
/*----------------------------------------------------------------------------*/
void ValWsp::operator =(Tsp xTsp)
{
  TspVal* pNestedTspVal = (TspVal*)pNestedValue();
  
  // set nested class
  pNestedTspVal->vClass(CN_TSPVAL);
  
  // assign int into the nested intval
  *pNestedTspVal = xTsp;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void ValWsp::vAssignBinary(Object* pObject)
{
  vClass(CN_BINVAL);
  _precondition_(_MAGIC_,CN_BINVAL);
  
  *_pBinVal(this) = pObject;
  
  ASSERT(CN_BINVAL == _pBinVal(this)->eClass());
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* ValWsp::aWspArea(void)
{
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
Value* ValWsp::pNestedValue(void)
{
  _precondition_(_MAGIC_);
  
  return (Value*)m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
Value* ValWsp::pNestedValueSelf(void)
{
  _precondition_(_MAGIC_);
  
  return (Value*)m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 ValWsp::iNestedAsInt(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  i8 i = pNestedIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
IntVal* ValWsp::pNestedIntVal(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  return pNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
PtrVal* ValWsp::pNestedPtrVal(void)
{
  _precondition_(_MAGIC_);
  
  PtrVal* pNestedPtrVal = reinterpret_cast<PtrVal*>(pNestedValue());
  
  return pNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
StrVal* ValWsp::pNestedStrVal(void)
{
  _precondition_(_MAGIC_);
  
  StrVal* pNestedStrVal = reinterpret_cast<StrVal*>(pNestedValue());
  
  return pNestedStrVal;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ValWsp::pFooter(void)
{
  _precondition_(_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
