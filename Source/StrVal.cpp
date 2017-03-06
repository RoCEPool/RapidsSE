/*============================================================================*/
/* StrVal.cpp -- abstract string value                                        */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVal* Values::_pStrVal(void* aAddr)
{
  StrVal* pStrVal = reinterpret_cast<StrVal*>(aAddr);
  
  RASSERT( ! pStrVal
          || CN_VALWSP == pStrVal->eClass()
          || CN_STRVAL == pStrVal->eClass()
          || CN_STRFXD == pStrVal->eClass()
          );
  
  return pStrVal;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(eCN eClassNo)
: Value(eClassNo)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(eCN eClassNo, i8 nBytes)
: Value(eClassNo)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StrVal::~StrVal(void)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
StrVal::operator Str*(void)
{
  _precondition_(_MAGIC_);

  // return address to char byte area
  return reinterpret_cast<Str*>(pAdd<char*>(this,sizeof(StrVal)));
}


/*----------------------------------------------------------------------------*/
/* get reference to Str area                                                  */
/*----------------------------------------------------------------------------*/
StrVal::operator Str&(void)
{
  _precondition_(_MAGIC_);
  
  // return reference to char byte area
  return reinterpret_cast<Str&>(*(pAdd<char*>(this,sizeof(StrVal))));
}

/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
//StrVal::operator Str*(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  return reinterpret_cast<Str*>(pAdd<char*>(this,sizeof(StrVal)));
//}

/*----------------------------------------------------------------------------*/
/* get adddress of Str area                                                   */
/*----------------------------------------------------------------------------*/
Str* StrVal::aStrArea(void)
{
  char* aAddr = pAdd<char*>(this,sizeof(StrVal));
  
  return reinterpret_cast<Str*>(aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
