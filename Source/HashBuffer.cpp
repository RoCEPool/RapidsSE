/*============================================================================*/
/* HashBuffer.cpp -- HashMap Buffer                                           */
/*============================================================================*/

#include "Structures.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashBuffer* Structures::_pHashBuffer(void* aAddr)
{
  HashBuffer* pHashBuffer = reinterpret_cast<HashBuffer*>(aAddr);
  
  RASSERT_TYPE(pHashBuffer,CN_HASHBUFFER);
  
  return pHashBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashBuffer::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_HASHBUFFER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void HashBuffer::operator delete(void* aAddr)
{
  HashBuffer* pHashBuffer = reinterpret_cast<HashBuffer*>(aAddr);
  
  HeapMem::vFree(pHashBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashBuffer::HashBuffer(HashMap* pHashMap, bool bDuplicates, i8 iDepth)
: Object(CN_HASHBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  m_pHashMap    = pHashMap;
  m_bDuplicates = bDuplicates;
  m_iDepth      = iDepth;
  
  //m_xHashEntry[]
  for (i8 i_Colide = 0; i_Colide < C_HASH_COLLIDES; i_Colide++)
    m_paCollides[i_Colide] = nullptr;

  m_i_Duplicate = 0;
  m_pLinkDuplicates = nullptr;

  _checkInitialised_(CN_HASHBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashBuffer::~HashBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  delete m_pLinkDuplicates;
  
  for (i8 i_Colide = 0; i_Colide < C_HASH_COLLIDES; i_Colide++)
    if (m_paCollides[i_Colide])
      delete m_paCollides[i_Colide];
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append a key,value duplicate onto this hash buffer                         */
/*----------------------------------------------------------------------------*/
void HashBuffer::vAppendDuplicate(HashEntry* aHeadEntry, i8 iValue)
{
  RASSERT(m_bDuplicates);
  
  if (m_i_Duplicate + 1 > C_HASH_ENTRIES) {
    HashBuffer* pDuplicates = new HashBuffer(m_pHashMap,true,m_iDepth+1);
    pDuplicates->m_pLinkDuplicates = this;
    m_pHashMap->m_pRootDuplicates = pDuplicates;
    m_pHashMap->m_pRootDuplicates->vAppendDuplicate(aHeadEntry,iValue);
    return;
  }

  m_xHashEntry[m_i_Duplicate].m_iHash = aHeadEntry->m_iHash;
  MemLib::vCopy(m_xHashEntry[m_i_Duplicate].m_yaKey,
                aHeadEntry->m_yaKey,aHeadEntry->m_nBytes,CN_HASHBUFFER);
  m_xHashEntry[m_i_Duplicate].m_nBytes = aHeadEntry->m_nBytes;
  m_xHashEntry[m_i_Duplicate].m_iValue = aHeadEntry->m_iValue;
  
  m_xHashEntry[m_i_Duplicate].m_aDupeChain = aHeadEntry->m_aDupeChain;
  aHeadEntry->m_aDupeChain = &m_xHashEntry[m_i_Duplicate];

  m_i_Duplicate++;
}

/*----------------------------------------------------------------------------*/
/* handle an occupation                                                       */
/*----------------------------------------------------------------------------*/
void HashBuffer::vHandleOccupied(i8 i_Entry, i8 iHash, byte* yaKey, i8 nBytes, i8 iValue)
{
  // is it a key duplicate?
  if (nBytes == m_xHashEntry[i_Entry].m_nBytes &&
      MemLib::bCompareBytes(m_xHashEntry[i_Entry].m_yaKey,yaKey,nBytes)) {
    
    if (! m_pHashMap->m_pRootDuplicates)
      m_pHashMap->m_pRootDuplicates = new HashBuffer(m_pHashMap,true,0);
    
    m_pHashMap->m_pRootDuplicates->vAppendDuplicate(&m_xHashEntry[i_Entry],iValue);
    return;
  }
  
  // its a hash collision
  vInsertCollide(yaKey,nBytes,iValue);
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsert(i8 iHash, byte* yaKey, i8 nBytes, i8 iValue)
{
  i8 i_Entry = iHash % C_HASH_ENTRIES;
  
  if (C_UNKNOWN != m_xHashEntry[i_Entry].m_iHash) {
    vHandleOccupied(i_Entry,iHash,yaKey,nBytes,iValue);
    return;
  }
  
  m_xHashEntry[i_Entry].m_iHash = iHash;
  MemLib::vCopy(m_xHashEntry[i_Entry].m_yaKey,yaKey,nBytes,CN_HASHBUFFER);
  m_xHashEntry[i_Entry].m_nBytes = nBytes;
  m_xHashEntry[i_Entry].m_iValue = iValue;
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertCollide(byte* yaKey, i8 nBytes, i8 iValue)
{
  byte ayKeyReverse[32];
  
  i8 i_Src = nBytes - 1;
  for (i8 i_Dst = 0; i_Dst < nBytes; i_Dst++, i_Src--)
    ayKeyReverse[i_Dst] = yaKey[i_Src];
  
  i8 iHashRev = HshLib::iHash(ayKeyReverse,nBytes);
  
  i8 i_Colide = iHashRev % C_HASH_COLLIDES;
  if (! m_paCollides[i_Colide])
    m_paCollides[i_Colide] = new HashBuffer(m_pHashMap,false,m_iDepth+1);
  m_paCollides[i_Colide]->vInsert(iHashRev,yaKey,nBytes,iValue);
}

/*----------------------------------------------------------------------------*/
/* iterate                                                                    */
/*----------------------------------------------------------------------------*/
bool HashBuffer::bIterate(HashCursor* pCursor, byte*& yaKey, i8& nBytes, i8& iValue)
{
  for ( ;
       pCursor->m_ia_HashEntry[pCursor->m_i_Stack] < C_HASH_ENTRIES;
       pCursor->m_ia_HashEntry[pCursor->m_i_Stack]++)
    
    if (C_UNKNOWN != m_xHashEntry[pCursor->m_ia_HashEntry[pCursor->m_i_Stack]].m_iHash) {
      
      yaKey  = &m_xHashEntry[pCursor->m_ia_HashEntry[pCursor->m_i_Stack]].m_yaKey[0];
      nBytes = m_xHashEntry[pCursor->m_ia_HashEntry[pCursor->m_i_Stack]].m_nBytes;
      iValue = m_xHashEntry[pCursor->m_ia_HashEntry[pCursor->m_i_Stack]].m_iValue;
      
      pCursor->m_ia_HashEntry[pCursor->m_i_Stack]++;
      
      return true;
    }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* HashBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
