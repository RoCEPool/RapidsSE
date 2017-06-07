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
  
  RASSERT( !aKeyVal || CN_KEYVAL == aKeyVal->eClass() );
 
  return aKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyVal::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  KeyVal* pKeyVal = reinterpret_cast<KeyVal*>(Heap::pAllocate(CN_KEYVAL));
  
  return pKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void KeyVal::operator delete(void* pKeyValAddress)
{
  KeyVal* pKeyVal = reinterpret_cast<KeyVal*>(pKeyValAddress);
  
  Heap::vFree(pKeyVal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyVal::KeyVal(KeyWsp& xKeyWsp)
: Object(CN_KEYVAL),
  m_xKey(xKeyWsp)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyVal::~KeyVal(void)
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
bool KeyVal::bLessThan(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bLessThan(&pKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key less than or equal to given key?                               */
/*----------------------------------------------------------------------------*/
bool KeyVal::bLessEqual(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bLessEqual(&pKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key equal to given key?                                            */
/*----------------------------------------------------------------------------*/
bool KeyVal::bEqualTo(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bEqualTo(&pKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key not equal to given key?                                        */
/*----------------------------------------------------------------------------*/
bool KeyVal::bNotEqual(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bNotEqual(&pKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key greater than or equal to given key?                            */
/*----------------------------------------------------------------------------*/
bool KeyVal::bGreaterEqual(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bGreaterEqual(&pKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
bool KeyVal::bGreaterThan(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.bGreaterThan(&pKeyValThat->m_xKey, pColVec);}

/*----------------------------------------------------------------------------*/
/* if this key greater than given key?                                        */
/*----------------------------------------------------------------------------*/
i8 KeyVal::iCmp(KeyVal* pKeyValThat, IdxColVec* pColVec)
{
  return m_xKey.iCmp(&pKeyValThat->m_xKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* does this keyval match the given arguments                                 */
/*----------------------------------------------------------------------------*/
bool KeyVal::bMatches(KeyVal* pKeyValThat, IdxColVec* pColVec, RelOpAry* pRelOpAry)
{
  return m_xKey.bMatches(&pKeyValThat->m_xKey, pColVec, pRelOpAry);
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
