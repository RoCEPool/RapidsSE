/*============================================================================*/
/* KeyVal.hpp -- Key based on Value Semantics                                 */
/*============================================================================*/

#include "Structures.h"

extern Heap* gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
KeyVal* Structures::_pKeyVal(void* pAddr)
{
  KeyVal* pKeyVal = reinterpret_cast<KeyVal*>(pAddr);
  
  return pKeyVal;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* KeyVal::operator new(size_t iSize, i4 iLevel)
{
  ASSERT(iSize);
  
  KeyVal* pKeyVal = (KeyVal*)gpMediumHeap->pAllocObject(CN_KEYVAL);
  
  return pKeyVal;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void KeyVal::operator delete(void* pKeyValAddress)
{
  KeyVal* pKeyVal = reinterpret_cast<KeyVal*>(pKeyValAddress);
  
  gpMediumHeap->vFreeObject(pKeyVal);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyVal::KeyVal(void)
: Key(CN_KEYVAL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KEYVAL);
  
  m_nElements = 0;
  
  for (i8 iElement = 0; iElement < C_KEY_ELEMENTS; iElement++) {
    m_eRelnOp[iElement] = RO_NONE;
    m_xRelnVal[iElement] = (i8)0;
  }
  
  pFooter()->vSetFooter();
  
  _checkInitialised_(sizeof(SkipNode));

  _postcondition_(_ADDRESS_,_MAGIC_,CN_KEYVAL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* denstructor                                                                */
/*----------------------------------------------------------------------------*/
KeyVal::~KeyVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KEYVAL);
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void KeyVal::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_KEYVAL);
  ASSERT(pObject->eClass() == CN_KEYVAL);
  
  KeyVal* pKeyVal = _pKeyVal(pObject);
  
  vTrace("%sKeyVal %p:\n", pTab, pKeyVal);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* KeyVal::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KEYVAL);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/