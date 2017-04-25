/*============================================================================*/
/* ValWsp2.cpp - workspace for moving values to/from thread and task stacks   */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ValWsp2* Values::_pValWsp2(void* aAddr)
{
  ValWsp2* pValWsp2 = reinterpret_cast<ValWsp2*>(aAddr);
  
  RASSERT( !pValWsp2 || CN_VALWSP == pValWsp2->eClass() );
  
  return pValWsp2;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValWsp2::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: ValWsp2 class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValWsp2::ValWsp2(void)
: Object(CN_VALWSP)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign an int into nested intval                                           */
/*----------------------------------------------------------------------------*/
void ValWsp2::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
  
  IntVal* pNestedIntVal = (IntVal*)pNestedValue();
  
  // set nested class
  pNestedIntVal->vClass(CN_INTVAL);
  
  // assign int into the nested intval
  *pNestedIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign a pointer into nested ptrval                                        */
/*----------------------------------------------------------------------------*/
void ValWsp2::operator =(Object* pPtr)
{
  //vClass(CN_PTRVAL);
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);

  PtrVal* pNestedPtrVal = (PtrVal*)pNestedValue();
  
  // set nested class
  pNestedPtrVal->vClass(CN_PTRVAL);
  
  // assign ptr into the nested ptrval
  *pNestedPtrVal = pPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into nested strval                                    */
/*----------------------------------------------------------------------------*/
void ValWsp2::operator =(Str* pStr)
{
  //vClass(CN_STRVAL);
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);

  StrVal* pNestedStrVal = (StrVal*)pNestedValue();
  
  // set nested class
  pNestedStrVal->vClass(CN_STRVAL);
  
  pNestedStrVal->vAssign(pStr);
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* ValWsp2::aWspArea(void)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
  
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
Value* ValWsp2::pNestedValue(void)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
  
  return (Value*)m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 ValWsp2::iNestedAsInt(void)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
  
  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());
  
  i8 i = pNestedIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
IntVal* ValWsp2::pNestedIntVal(void)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);

  IntVal* pNestedIntVal = reinterpret_cast<IntVal*>(pNestedValue());

  return pNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
PtrVal* ValWsp2::pNestedPtrVal(void)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);

  PtrVal* pNestedPtrVal = reinterpret_cast<PtrVal*>(pNestedValue());
  
  return pNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get value in the wsp area                                                  */
/*----------------------------------------------------------------------------*/
StrVal* ValWsp2::pNestedStrVal(void)
{
  _precondition_(_MAGIC_,CN_VALWSP,_FOOTER_);
  
  StrVal* pNestedStrVal = reinterpret_cast<StrVal*>(pNestedValue());
  
  return pNestedStrVal;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ValWsp2::pFooter(void)
{
  _precondition_(_MAGIC_,CN_VALWSP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
