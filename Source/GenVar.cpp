/*============================================================================*/
/* GenVar.cpp -- Generic Variable                                             */
/*============================================================================*/

#include "Forth.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenVar* Forth::_pGenVar(void* aAddr)
{
  GenVar* pValWrk = reinterpret_cast<GenVar*>(aAddr);
  
  RASSERT_TYPE(pValWrk,CN_GENVAR);
  
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
: Object(CN_GENVAR)
{
  _precondition_(_MAGIC_,CN_GENVAR);                    //! need to add _FOOTER_
  
  Value* aNestedValue = this->aNestedValue();
  
  aNestedValue->vSetMagic();
  aNestedValue->vSetClass(CN_NONE);
  
  _postcondition_(_MAGIC_,CN_GENVAR);
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
//void GenVar::operator =(Bcd wBcd)
//{
//  BcdVal* aNestedBcdVal = (BcdVal*)aNestedValue();
//
//  // set nested class
//  aNestedBcdVal->vSetClass(CN_BCDVAL);
//
//  // put the bcd word into the nested bcdval
//  *aNestedBcdVal = wBcd;
//}

/*----------------------------------------------------------------------------*/
/* assign/copy a boolean into workspace                                       */
/*----------------------------------------------------------------------------*/
//void GenVar::operator =(Bol wBol)
//{
//  BolVal* aNestedBolVal = (BolVal*)aNestedValue();
//
//  // set nested class
//  aNestedBolVal->vSetClass(CN_BOLVAL);
//
//  // put the bol word into the nested bolval
//  *aNestedBolVal = wBol;
//}

/*----------------------------------------------------------------------------*/
/* assign/copy a date into workspace                                          */
/*----------------------------------------------------------------------------*/
//void GenVar::operator =(Dte wDte)
//{
//  DteVal* aNestedDteVal = (DteVal*)aNestedValue();
//
//  // set nested class
//  aNestedDteVal->vSetClass(CN_DTEVAL);
//
//  // put the dte word into the nested dteval
//  *aNestedDteVal = wDte;
//}

/*----------------------------------------------------------------------------*/
/* assign/copy a float into workspace                                         */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(d8 dFlt)
{
  FltVal* aNestedFltVal = (FltVal*)aNestedValue();
  
  // set nested class
  aNestedFltVal->vSetClass(CN_FLTVAL);
  
  // put the flt word into the nested fltval
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
  
  // put the int word into the nested intval
  *aNestedIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an interval into workspace                                     */
/*----------------------------------------------------------------------------*/
//void GenVar::operator =(Ivl wIvl)
//{
//  IvlVal* aNestedIvlVal = (IvlVal*)aNestedValue();
//
//  // set nested class
//  aNestedIvlVal->vSetClass(CN_IVLVAL);
//
//  // put the ivl word into the nested ivlval
//  *aNestedIvlVal = wIvl;
//}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object into workspace                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObject)
{
  PtrVal* aNestedPtrVal = (PtrVal*)aNestedValue();
  
  // set nested class
  aNestedPtrVal->vSetClass(CN_PTRVAL);
  
  // put the pointer into the nested ptrval
  *aNestedPtrVal = pObject;
}

/*----------------------------------------------------------------------------*/
/* assign/copy the address of a non-proper object instance into workspace     */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Value* aValue)
{
  PtrVal* aNestedPtrVal = (PtrVal*)aNestedValue();
  
  // set nested class
  aNestedPtrVal->vSetClass(CN_PTRVAL);
  
  // put the address into the nested ptrval
  *aNestedPtrVal = aValue;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* aStz)
{
  StrVal* aNestedStrVal = (StrVal*)aNestedValue();
  
  // set nested class
  aNestedStrVal->vSetClass(CN_STRVAL);
  
  // copy the stz into the nested ptrval
  aNestedStrVal->vAssign(aStz);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a time into workspace                                          */
/*----------------------------------------------------------------------------*/
//void GenVar::operator =(Tme wTme)
//{
//  TmeVal* aNestedTmeVal = (TmeVal*)aNestedValue();
//
//  // set nested class
//  aNestedTmeVal->vSetClass(CN_TMEVAL);
//
//  // put the tme word into the nested tmeval
//  *aNestedTmeVal = wTme;
//}

/*----------------------------------------------------------------------------*/
/* assign/copy an timestamp into workspace                                    */
/*----------------------------------------------------------------------------*/
//void GenVar::operator =(Tsp wTsp)
//{
//  TspVal* aNestedTspVal = (TspVal*)aNestedValue();
//  
//  // set nested class
//  aNestedTspVal->vSetClass(CN_TSPVAL);
//  
//  // put the tsp word into the nested tmeval
//  *aNestedTspVal = wTsp;
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 GenVar::iNestedAsInt(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* aNestedIntVal = reinterpret_cast<IntVal*>(aNestedValue());
  
  i8 iInt = aNestedIntVal->iInt();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* get address of the intval in the wsp area                                  */
/*----------------------------------------------------------------------------*/
IntVal* GenVar::aNestedIntVal(void)
{
  _precondition_(_MAGIC_);
  
  IntVal* aNestedIntVal = reinterpret_cast<IntVal*>(aNestedValue());
  
  return aNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get address of the ptrval in the wsp area                                  */
/*----------------------------------------------------------------------------*/
PtrVal* GenVar::aNestedPtrVal(void)
{
  _precondition_(_MAGIC_);
  
  PtrVal* aNestedPtrVal = reinterpret_cast<PtrVal*>(aNestedValue());
  
  return aNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get address of the strval in the wsp area                                  */
/*----------------------------------------------------------------------------*/
StrVal* GenVar::aNestedStrVal(void)
{
  _precondition_(_MAGIC_);
  
  StrVal* aNestedStrVal = reinterpret_cast<StrVal*>(aNestedValue());
  
  return aNestedStrVal;
}

/*----------------------------------------------------------------------------*/
/* get address of the value in the wsp area                                   */
/*----------------------------------------------------------------------------*/
Value* GenVar::aNestedValue(void)
{
  _precondition_(_MAGIC_);
 
  Value* aNestedValue = reinterpret_cast<Value*>(m_xWsp.ayArea());

  return aNestedValue;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get string                                                                 */
/*----------------------------------------------------------------------------*/
void GenVar::vGetString(Stz* zxStz)
{
  *zxStz = reinterpret_cast<void*>(aNestedValue());
}

/*----------------------------------------------------------------------------*/
/* get address of wsp area                                                    */
/*----------------------------------------------------------------------------*/
char* GenVar::aWspArea(void)
{
  return (char*)m_xWsp.ayArea();
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
