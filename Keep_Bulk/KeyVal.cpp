/*============================================================================*/
/* KeyVal.cpp -- key value                                                    */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
KeyVal* DBobjects::_aKeyVal(void* aAddr)
{
  KeyVal* aKeyVal = reinterpret_cast<KeyVal*>(aAddr);
  
  RASSERT_TYPE(aKeyVal,CN_KEYVAL);
 
  return aKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyVal::operator new(size_t)
{
  KeyVal* aKeyVal = reinterpret_cast<KeyVal*>(HeapMem::pAllocate(CN_KEYVAL));
  
  return aKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void KeyVal::operator delete(void* aKeyValAddress)
{
  KeyVal* aKeyVal = reinterpret_cast<KeyVal*>(aKeyValAddress);
  
  HeapMem::vFree(aKeyVal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyVal::KeyVal(KeyWrk& xKeyWrk)
: Object(CN_KEYVAL),
  m_xKey(xKeyWrk)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyVal::~KeyVal(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
char* KeyVal::aEachDatum(Column* pColumn, i8& nBytes, char** aaCursor)
{
  char* aDatum = m_xKey.aEachDatum(pColumn,nBytes,aaCursor);
  
  return aDatum;
}

/*----------------------------------------------------------------------------*/
/* if this key less than given key?                                           */
/*----------------------------------------------------------------------------*/
bool KeyVal::bLessThan(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bLessThan(&aKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key less than or equal to given key?                               */
/*----------------------------------------------------------------------------*/
bool KeyVal::bLessEqual(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bLessEqual(&aKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key equal to given key?                                            */
/*----------------------------------------------------------------------------*/
bool KeyVal::bEqualTo(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bEqualTo(&aKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key not equal to given key?                                        */
/*----------------------------------------------------------------------------*/
bool KeyVal::bNotEqual(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bNotEqual(&aKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key greater than or equal to given key?                            */
/*----------------------------------------------------------------------------*/
bool KeyVal::bGreaterEqual(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bGreaterEqual(&aKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
bool KeyVal::bGreaterThan(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bGreaterThan(&aKeyValThat->m_xKey, pColVec);}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
i8 KeyVal::iCmp(KeyVal* aKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.iCmp(&aKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* does this keyval match the given arguments                                 */
/*----------------------------------------------------------------------------*/
bool KeyVal::bMatches(KeyVal* aKeyValThat, IdxColVec* pColVec, RelOpAry* pRelOpAry)
{
  return m_xKey.bMatches(&aKeyValThat->m_xKey, pColVec, pRelOpAry);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyVal::vShow(IdxColVec* pColVec)
{
  m_xKey.vShow(pColVec);
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key& KeyVal::xKey(void)
{
  return m_xKey;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
