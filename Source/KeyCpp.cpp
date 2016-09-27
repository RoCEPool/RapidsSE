/*============================================================================*/
/* KeyCpp.cpp -- Key based on C++ Semantics                                   */
/*============================================================================*/

#include "Structures.h"

extern Heap* gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
KeyCpp* Structures::_pKeyCpp(void* pAddr)
{
  KeyCpp* pKeyCpp = reinterpret_cast<KeyCpp*>(pAddr);
  
  return pKeyCpp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* KeyCpp::operator new(size_t iSize, i4 iLevel)
{
  ASSERT(iSize);
  
  KeyCpp* pKeyCpp = (KeyCpp*)gpMediumHeap->pAllocObject(CN_KEYCPP);
  
  return pKeyCpp;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void KeyCpp::operator delete(void* pKeyCppAddress)
{
  KeyCpp* pKeyCpp = reinterpret_cast<KeyCpp*>(pKeyCppAddress);
  
  gpMediumHeap->vFreeObject(pKeyCpp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyCpp::KeyCpp(void)
: Key(CN_KEYCPP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KEYCPP);
  
  m_nElements = 0;
  
  for (i8 iElement = 0; iElement < C_KEY_ELEMENTS; iElement++) {
    m_eRelnOp[iElement] = RO_NONE;
    m_xRelnVal[iElement] = (i8)0;
  }
  
  pFooter()->vSetFooter();
  
  _checkInitialised_(sizeof(SkipNode));

  _postcondition_(_ADDRESS_,_MAGIC_,CN_KEYCPP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* denstructor                                                                */
/*----------------------------------------------------------------------------*/
KeyCpp::~KeyCpp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KEYCPP);
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void KeyCpp::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_KEYCPP);
  ASSERT(pObject->eClass() == CN_KEYCPP);
  
  KeyCpp* pKeyCpp = _pKeyCpp(pObject);
  
  vTrace("%sKeyCpp %p:\n", pTab, pKeyCpp);
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
Footer* KeyCpp::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KEYCPP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/