/*============================================================================*/
/* GenVar.cpp -- Generic Variable                                             */
/*============================================================================*/

#include "Forth.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenVar* Forth::_pGenVar(void* aAddr)
{
  GenVar* pValWrk = reinterpret_cast<GenVar*>(aAddr);
  
  RASSERT_TYPE(pValWrk,CN_VALWRK);
  
  return pValWrk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* GenVar::operator new(size_t)
{
  vFatal("fatal: GenVar class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenVar::GenVar(void)
: Object(CN_VALWRK)
{
  _precondition_(_MAGIC_,CN_VALWRK);                    //! need to add _FOOTER_
  
  _postcondition_(_MAGIC_,CN_VALWRK);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GenVar::~GenVar(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign/copy a bcd into workspace                                           */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Bcd xBcd)
{
  BcdVal* aNestedBcdVal = (BcdVal*)aNestedValue();
  
  // set nested class
  aNestedBcdVal->vSetClass(CN_BCDVAL);
  
  // assign int into the nested intval
  *aNestedBcdVal = xBcd;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a boolean into workspace                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Bol xBol)
{
  BolVal* aNestedBolVal = (BolVal*)aNestedValue();
  
  // set nested class
  aNestedBolVal->vSetClass(CN_BOLVAL);
  
  // assign int into the nested intval
  *aNestedBolVal = xBol;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a date into workspace                                          */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Dte xDte)
{
  DteVal* aNestedDteVal = (DteVal*)aNestedValue();
  
  // set nested class
  aNestedDteVal->vSetClass(CN_DTEVAL);
  
  // assign int into the nested intval
  *aNestedDteVal = xDte;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a float into workspace                                         */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(d8 dFlt)
{
  FltVal* aNestedFltVal = (FltVal*)aNestedValue();
  
  // set nested class
  aNestedFltVal->vSetClass(CN_FLTVAL);
  
  // assign flt into the nested fltval
  *aNestedFltVal = dFlt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  IntVal* aNestedIntVal = (IntVal*)aNestedValue();
  
  // set nested class
  aNestedIntVal->vSetClass(CN_INTVAL);
  
  // assign int into the nested intval
  *aNestedIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an interval into workspace                                     */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Ivl iIvl)
{
  IvlVal* aNestedIvlVal = (IvlVal*)aNestedValue();
  
  // set nested class
  aNestedIvlVal->vSetClass(CN_IVLVAL);
  
  // assign int into the nested intval
  *aNestedIvlVal = iIvl;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pPtr)
{
  PtrVal* aNestedPtrVal = (PtrVal*)aNestedValue();
  
  // set nested class
  aNestedPtrVal->vSetClass(CN_PTRVAL);
  
  // assign ptr into the nested ptrval
  *aNestedPtrVal = pPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Value* aPtr)
{
  PtrVal* aNestedPtrVal = (PtrVal*)aNestedValue();
  
  // set nested class
  aNestedPtrVal->vSetClass(CN_PTRVAL);
  
  // assign ptr into the nested ptrval
  *aNestedPtrVal = aPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* pStr)
{
  StrVal* aNestedStrVal = (StrVal*)aNestedValue();
  
  // set nested class
  aNestedStrVal->vSetMagic();
  aNestedStrVal->vSetClass(CN_STRVAL);
  
  aNestedStrVal->vAssign(pStr);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an time into workspace                                         */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Tme xTme)
{
  TmeVal* aNestedTmeVal = (TmeVal*)aNestedValue();
  
  // set nested class
  aNestedTmeVal->vSetClass(CN_TMEVAL);
  
  // assign tme into the nested intval
  *aNestedTmeVal = xTme;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an timestamp into workspace                                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Tsp xTsp)
{
  TspVal* aNestedTspVal = (TspVal*)aNestedValue();
  
  // set nested class
  aNestedTspVal->vSetClass(CN_TSPVAL);
  
  // assign tsp into the nested intval
  *aNestedTspVal = xTsp;
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* GenVar::aWspArea(void)
{
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
Value* GenVar::aNestedValue(void)
{
  _precondition_(_MAGIC_);
  
  return (Value*)m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 GenVar::iNestedAsInt(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* aNestedIntVal = reinterpret_cast<IntVal*>(aNestedValue());
  
  i8 i = aNestedIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
IntVal* GenVar::aNestedIntVal(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* aNestedIntVal = reinterpret_cast<IntVal*>(aNestedValue());
  
  return aNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
PtrVal* GenVar::aNestedPtrVal(void)
{
  _precondition_(_MAGIC_);
  
  PtrVal* aNestedPtrVal = reinterpret_cast<PtrVal*>(aNestedValue());
  
  return aNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
StrVal* GenVar::aNestedStrVal(void)
{
  _precondition_(_MAGIC_);
  
  StrVal* aNestedStrVal = reinterpret_cast<StrVal*>(aNestedValue());
  
  return aNestedStrVal;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void GenVar::vToString(Stz* aStz)
{
  *aStz = reinterpret_cast<void*>(aNestedValue());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* GenVar::aFooter(void)
{
  _precondition_(_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
