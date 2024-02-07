/*============================================================================*/
/* HashMapEntry.cpp -- HashMapEntry                                           */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashMapEntry* BASE::_pHashMapEntry(void* aAddr)
{
  HashMapEntry* pHashMapEntry = reinterpret_cast<HashMapEntry*>(aAddr);

  RASSERT_TYPE(pHashMapEntry,CN_HASHMAPENTRY);

  return pHashMapEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashMapEntry::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_HASHMAPENTRY);

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void HashMapEntry::operator delete(void* pHashMapEntryAddress)
{
  HashMapEntry* pHashMapEntry = reinterpret_cast<HashMapEntry*>(pHashMapEntryAddress);

  Heaps::vFree(pHashMapEntry);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashMapEntry::HashMapEntry(void* pKey, Object* pValue, eBI eBufferIdType)
: Object(CN_HASHMAPENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY,_FOOTER_);

  m_eBufferIdType = eBufferIdType;
  m_pKeyObject = pKey;
  m_pValObject = pValue;
  m_pNext = nullptr;

  m_ixInUse.vCheckInit();

  _checkInitialised_(CN_HASHMAPENTRY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMapEntry::~HashMapEntry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

bool HashMapEntry::bInUse(void)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* HashMapEntry::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY);

  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/