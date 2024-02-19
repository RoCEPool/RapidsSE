/*============================================================================*/
/* HashMapSlot.cpp -- HashMapSlot                                             */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two nodes                                                          */
/*----------------------------------------------------------------------------*/
i8 BASE::iHashMapEntryKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  HashMapEntry* pKey_l = (HashMapEntry*) aItem_l;

  // do the node keys match?
  if (pKey_l->m_pKeyObject == aItem_r)
    return 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pKey_l->m_pKeyObject >= aItem_r)
    return 1;
  else
    return -1;
}

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
, m_okSlotLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPSLOT,_FOOTER_);

  m_pPdsOfEntries = new Pds_t<HashMapEntry>(iHashMapEntryKeyCompare, false);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/