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
/* delete a given key                                                         */
/*----------------------------------------------------------------------------*/
void HashMap::vDeleteKey(u8 uKey)
{
  u8 uIndex = uHashFunction(uKey);
  HashMapSlot* uIter1 = m_paMap[uIndex];
  if(uIter1 != nullptr)
  {
    if(uIter1->m_uKey == uKey)
    {
      m_paMap[uIndex] = uIter1->m_pNext;
      delete uIter1;
    }
    else
    {
      HashMapSlot* uPrevIter;
      do {
        uPrevIter = uIter1;
        uIter1 = uIter1->m_pNext;
      } while(uIter1 != nullptr && uIter1->m_uKey != uKey);

      if(uIter1 != nullptr)
      {
        uPrevIter->m_pNext = uIter1->m_pNext;
        delete uIter1;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* does given key exist in the hashmap                                        */
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
/* Insert a key,value at the front of the chain if it does not exist. Else update*/
/*----------------------------------------------------------------------------*/
void HashMap::vInsertAtFront(u8 uKey, i8 iValue)
{
  u8 uIndex = uHashFunction(uKey);

  HashMapSlot* uIter = m_paMap[uIndex];
  while(uIter != nullptr)
  {
    if(uIter->m_uKey == uKey)
    {
      uIter->m_iValue = iValue;
      return;
    }
    uIter = uIter->m_pNext;
  }

  HashMapSlot* tmpSlot = new HashMapSlot(uKey, iValue);

  tmpSlot->m_pNext = m_paMap[uIndex];
  m_paMap[uIndex] = tmpSlot;
}

/*----------------------------------------------------------------------------*/
/* Get value stored at key if it exists                                       */
/*----------------------------------------------------------------------------*/
i8 HashMap::iValueAt(u8 uKey)
{
  u8 uIndex = uHashFunction(uKey);
  HashMapSlot* uIter = m_paMap[uIndex];
  while(uIter != nullptr)
  {
    if(uIter->m_uKey == uKey)
      return uIter->m_iValue;
    uIter = uIter->m_pNext;
  }

  return C_DATUM_NULL;
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
