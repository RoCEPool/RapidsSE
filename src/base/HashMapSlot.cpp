/*============================================================================*/
/* HashMapSlot.cpp -- HashMapSlot                                             */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashMapSlot* BASE::_pHashMapSlot(void* aAddr)
{
  HashMapSlot* pHashMapSlot = reinterpret_cast<HashMapSlot*>(aAddr);

  RASSERT_TYPE(pHashMapSlot,CN_HASHMAPSLOT);

  return pHashMapSlot;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashMapSlot::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_HASHMAPSLOT);

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void HashMapSlot::operator delete(void* pHashMapSlotAddress)
{
  HashMapSlot* pHashMapSlot = reinterpret_cast<HashMapSlot*>(pHashMapSlotAddress);

  Heaps::vFree(pHashMapSlot);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashMapSlot::HashMapSlot(Object* pKey, Object* pValue)
    : Object(CN_HASHMAPSLOT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);

  m_pKeyObject = pKey;
  m_pValObject = pValue;
  m_pNext = nullptr;// Check if I should use C_DATUM_NULL

  _checkInitialised_(CN_HASHMAPSLOT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMapSlot::~HashMapSlot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* HashMapSlot::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT);

  return &m_xFooter;
}