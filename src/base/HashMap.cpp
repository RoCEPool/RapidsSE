/*============================================================================*/
/* HashMap.cpp -- HashMap                                                     */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashMap* BASE::_pHashMap(void* aAddr)
{
  HashMap* pHashMap = reinterpret_cast<HashMap*>(aAddr);
  
  RASSERT_TYPE(pHashMap,CN_HASHMAP);
  
  return pHashMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashMap::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_HASHMAP);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void HashMap::operator delete(void* pHashMapAddress)
{
  HashMap* pHashMap = reinterpret_cast<HashMap*>(pHashMapAddress);
  
  Heaps::vFree(pHashMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashMap::HashMap(bool bTrace, u8 uNoOfSlots)
: Object(CN_HASHMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  
  m_bTrace            = bTrace;
  m_uNoOfSlots = C_HASHMAP_SIZE;
  for(u8 i = 0; i < m_uNoOfSlots; i++)
    m_paMap[i] = nullptr;
  
  _checkInitialised_(CN_HASHMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMap::~HashMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  for(u8 i = 0; i < m_uNoOfSlots; i++)
  {
    if(m_paMap[i] != nullptr)
    {
      HashMapSlot* uIter1 = m_paMap[i];
      HashMapSlot* uIter2;
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* does given key exist in the hashmap                                  */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(u8 uKey)
{
  u8 uIndex = uHashFunction(uKey);
  HashMapSlot* uIter = m_paMap[uIndex];
  while(uIter != nullptr)
  {
    if(uIter->m_uKey == uKey)
      return true;
    uIter = uIter->m_pNext;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
u8 HashMap::uHashFunction(u8 uKey)
{
  return uKey % m_uNoOfSlots;
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hashmap                                       */
/*----------------------------------------------------------------------------*/
void HashMap::vInsertAtFront(u8 uKey, i8 iValue)
{
  u8 uIndex = uHashFunction(uKey);
  HashMapSlot* tmpSlot = new HashMapSlot(uKey, iValue);

  tmpSlot->m_pNext = m_paMap[uIndex];
  m_paMap[uIndex] = tmpSlot;
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
Footer* HashMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
