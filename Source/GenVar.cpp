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
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  StrVal* aStrVal = (StrVal*)aNestedValue();
  
  // set nested class
  aStrVal->vSetClass(CN_STRVAL);
  
  // copy the stz into the nested ptrval
  aStrVal->vAssign(szStz);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 GenVar::iInt(void)
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
/* get stz in the wsp area                                                    */
/*----------------------------------------------------------------------------*/
Stz* GenVar::szStz(void)
{
  _precondition_(_MAGIC_);
  
  StrVal* aStrVal = reinterpret_cast<StrVal*>(aNestedValue());
  
  Stz* sz = aStrVal->aStz();
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* get address of the value in the wsp area                                   */
/*----------------------------------------------------------------------------*/
Value* GenVar::aNestedValue(void)
{
  _precondition_(_MAGIC_);
 
  Value* aNestedValue = reinterpret_cast<Value*>(&xValue);

  return aNestedValue;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


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
