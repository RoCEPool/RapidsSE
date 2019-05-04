/*============================================================================*/
/* HashCursor.cpp -- HashCursor                                               */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashCursor* Structures::_pHashCursor(void* aAddr)
{
  HashCursor* pHashCursor = reinterpret_cast<HashCursor*>(aAddr);
  
  return pHashCursor;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashCursor::HashCursor(HashMap* pHashMap)
{
  m_pHashMap    = pHashMap;
  
  m_pHashBuffer = pHashMap->m_pRootBuffer;
  m_i_HashEntry = 0;

  m_i_Stack = 0;
  for (i8 i_Stack = 0; i_Stack < C_HASH_DEPTH; i_Stack++) {
    m_paHashBuffer[i_Stack] = nullptr;
    m_ia_HashEntry[i_Stack] = 0;
  }
  
  m_pDupeBuffer = nullptr;
  m_pDupeEntry  = nullptr;
  
  m_yaKey       = nullptr;
  m_nBytes      = 0;
  m_iValue      = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashCursor::~HashCursor(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* iterate each entry in the hash map                                         */
/*----------------------------------------------------------------------------*/
bool HashCursor::bIterate(void)
{
  if (m_pDupeEntry) {
    
    m_yaKey  = m_pDupeEntry->m_yaKey;
    m_nBytes = m_pDupeEntry->m_nBytes;
    m_iValue = m_pDupeEntry->m_iValue;

    m_pDupeEntry = m_pDupeEntry->m_aDupeChain;
    return true;
 }
  
  for ( ;
       m_i_HashEntry < m_pHashMap->m_nEntriesPerBuffer;
       m_i_HashEntry++) {
    
    if (C_UNKNOWN == m_pHashBuffer->m_xHashEntry[m_i_HashEntry].m_iHash)
      continue;
    
    m_yaKey  = m_pHashBuffer->m_xHashEntry[m_i_HashEntry].m_yaKey;
    m_nBytes = m_pHashBuffer->m_xHashEntry[m_i_HashEntry].m_nBytes;
    m_iValue = m_pHashBuffer->m_xHashEntry[m_i_HashEntry].m_iValue;

    m_pDupeEntry = m_pHashBuffer->m_xHashEntry[m_i_HashEntry].m_aDupeChain;

    m_i_HashEntry++;
    return true;
  }
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
