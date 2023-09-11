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
HashMapSlot::HashMapSlot(void)
: Object(CN_HASHMAPSLOT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);

  m_uNoOfSlots = C_INIT_HASHMAP_SIZE;

  for(u8 i =0; i < m_uNoOfSlots; i++)
  {
    HashMapEntry* pDummyEntry = new HashMapEntry(nullptr, nullptr);
    pDummyEntry->m_pNext = nullptr;
    m_pEntry[i] = pDummyEntry;
    m_okSlotLock[i].vInit(m_pEntry[i]);
  }

  _checkInitialised_(CN_HASHMAPSLOT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMapSlot::~HashMapSlot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);
  for(u8 i = 0; i < m_uNoOfSlots; i++)
  {
    if(m_pEntry[i] != nullptr)
    {
      HashMapEntry* uIter1 = m_pEntry[i];
      HashMapEntry* uIter2;
      while(uIter1 != nullptr)
      {
        uIter2 = uIter1->m_pNext;
        delete uIter1;
        uIter1 = uIter2;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* Get number of slots                                                        */
/*----------------------------------------------------------------------------*/
u8 HashMapSlot::uGetNoSlots(void)
{
  return m_uNoOfSlots;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/