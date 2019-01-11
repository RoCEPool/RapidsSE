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
  BcdVal* pNestedBcdVal = (BcdVal*)pNestedValue();
  
  // set nested class
  pNestedBcdVal->vSetClass(CN_BCDVAL);
  
  // assign int into the nested intval
  *pNestedBcdVal = xBcd;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a boolean into workspace                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Bol xBol)
{
  BolVal* pNestedBolVal = (BolVal*)pNestedValue();
  
  // set nested class
  pNestedBolVal->vSetClass(CN_BOLVAL);
  
  // assign int into the nested intval
  *pNestedBolVal = xBol;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a date into workspace                                          */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Dte xDte)
{
  DteVal* pNestedDteVal = (DteVal*)pNestedValue();
  
  // set nested class
  pNestedDteVal->vSetClass(CN_DTEVAL);
  
  // assign int into the nested intval
  *pNestedDteVal = xDte;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a float into workspace                                         */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(d8 dFlt)
{
  FltVal* pNestedFltVal = (FltVal*)pNestedValue();
  
  // set nested class
  pNestedFltVal->vSetClass(CN_FLTVAL);
  
  // assign flt into the nested fltval
  *pNestedFltVal = dFlt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  IntVal* pNestedIntVal = (IntVal*)pNestedValue();
  
  // set nested class
  pNestedIntVal->vSetClass(CN_INTVAL);
  
  // assign int into the nested intval
  *pNestedIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an interval into workspace                                     */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Ivl iIvl)
{
  IvlVal* pNestedIvlVal = (IvlVal*)pNestedValue();
  
  // set nested class
  pNestedIvlVal->vSetClass(CN_IVLVAL);
  
  // assign int into the nested intval
  *pNestedIvlVal = iIvl;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pPtr)
{
  PtrVal* pNestedPtrVal = (PtrVal*)pNestedValue();
  
  // set nested class
  pNestedPtrVal->vSetClass(CN_PTRVAL);
  
  // assign ptr into the nested ptrval
  *pNestedPtrVal = pPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Value* pPtr)
{
  PtrVal* pNestedPtrVal = (PtrVal*)pNestedValue();
  
  // set nested class
  pNestedPtrVal->vSetClass(CN_PTRVAL);
  
  // assign ptr into the nested ptrval
  *pNestedPtrVal = pPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* pStr)
{
  StrVal* pNestedStrVal = (StrVal*)pNestedValue();
  
  // set nested class
  pNestedStrVal->vSetClass(CN_STRVAL);
  
  pNestedStrVal->vAssign(pStr);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an time into workspace                                         */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Tme xTme)
{
  TmeVal* pNestedTmeVal = (TmeVal*)pNestedValue();
  
  // set nested class
  pNestedTmeVal->vSetClass(CN_TMEVAL);
  
  // assign tme into the nested intval
  *pNestedTmeVal = xTme;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an timestamp into workspace                                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Tsp xTsp)
{
  TspVal* pNestedTspVal = (TspVal*)pNestedValue();
  
  // set nested class
  pNestedTspVal->vSetClass(CN_TSPVAL);
  
  // assign tsp into the nested intval
  *pNestedTspVal = xTsp;
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
Value* GenVar::pNestedValue(void)
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
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  i8 i = pNestedIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
IntVal* GenVar::pNestedIntVal(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  return pNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
PtrVal* GenVar::pNestedPtrVal(void)
{
  _precondition_(_MAGIC_);
  
  PtrVal* pNestedPtrVal = reinterpret_cast<PtrVal*>(pNestedValue());
  
  return pNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
StrVal* GenVar::pNestedStrVal(void)
{
  _precondition_(_MAGIC_);
  
  StrVal* pNestedStrVal = reinterpret_cast<StrVal*>(pNestedValue());
  
  return pNestedStrVal;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void GenVar::vToString(Stz* aWorkspace)
{
  *aWorkspace = reinterpret_cast<void*>(pNestedValue());
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
