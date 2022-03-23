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
  m_uNoOfEntries = 0;
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
/* print analysis of this hash map                                            */
/*----------------------------------------------------------------------------*/
void HashMap::vAnalysis(void)
{
  vTrace("HM: (with %llu buckets/slots and %llu total entries)\n"
         "    Load Factor: %f\n"_Stz,

         m_uNoOfSlots,
         m_uNoOfEntries,
         (d8)m_uNoOfEntries/m_uNoOfSlots);
}

/*----------------------------------------------------------------------------*/
/* delete a given key                                                         */
/*----------------------------------------------------------------------------*/
void HashMap::vDeleteKey(Object* pKey)
{
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter1 = m_paMap[uIndex];
  if(pIter1 != nullptr)
  {
    if(pIter1->m_pKeyObject == pKey)
    {
      m_paMap[uIndex] = pIter1->m_pNext;
      delete pIter1;
    }
    else
    {
      HashMapSlot* pPrevIter;
      do {
        pPrevIter = pIter1;
        pIter1 = pIter1->m_pNext;
      } while(pIter1 != nullptr && pIter1->m_pKeyObject != pKey);

      if(pIter1 != nullptr)
      {
        pPrevIter->m_pNext = pIter1->m_pNext;
        delete pIter1;
        ASSERT(1 <= m_uNoOfEntries); //Assert before decrement since unsigned underflow could occur
        m_uNoOfEntries--;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* does given key exist in the hashmap                                        */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(Object* pKey)
{
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter = m_paMap[uIndex];
  while(pIter != nullptr)
  {
    if(pIter->m_pKeyObject == pKey)
      return true;
    pIter = pIter->m_pNext;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
u8 HashMap::uHashFunction(Object* pKey)
{
  return ((u8)pKey % m_uNoOfSlots);//Check if this is safe
}

/*----------------------------------------------------------------------------*/
/* Insert a key,value at the front of the chain if it does not exist. Else update*/
/*----------------------------------------------------------------------------*/
void HashMap::vInsertAtFront(Object* pKey, Object* pValue)
{
  u8 uIndex = uHashFunction(pKey);

  HashMapSlot* pIter = m_paMap[uIndex];
  while(pIter != nullptr)
  {
    if(pIter->m_pKeyObject == pKey)
    {
      pIter->m_pValObject = pValue;
      return;
    }
    pIter = pIter->m_pNext;
  }

  HashMapSlot* pTmpSlot = new HashMapSlot(pKey, pValue);

  pTmpSlot->m_pNext = m_paMap[uIndex];
  m_paMap[uIndex] = pTmpSlot;
  m_uNoOfEntries++;
}

/*----------------------------------------------------------------------------*/
/* Get value stored at key if it exists                                       */
/*----------------------------------------------------------------------------*/
Object* HashMap::pValueAt(Object* pKey)
{
  u8 uIndex = uHashFunction(pKey);
  HashMapSlot* pIter = m_paMap[uIndex];
  while(pIter != nullptr)
  {
    if(pIter->m_pKeyObject == pKey)
      return pIter->m_pValObject;
    pIter = pIter->m_pNext;
  }

  return nullptr;
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
