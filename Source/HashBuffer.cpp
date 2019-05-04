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
HashBuffer::HashBuffer(HashMap* pHashMap, i8 iDepth)
: Object(CN_HASHBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  m_pHashMap    = pHashMap;
  m_iDepth      = iDepth;
  m_bDuplicates = false;
  
  //m_xHashEntry[]
  for (i8 i_Collide = 0; i_Collide < C_HASH_COLLIDES; i_Collide++)
    m_paCollides[i_Collide] = nullptr;

  m_i_Duplicate = 0;
  m_pLinkDuplicates = nullptr;

  _checkInitialised_(CN_HASHBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashBuffer::HashBuffer(HashMap* pHashMap, i8 iDepth, HashBuffer* pLinkDuplicates)
: Object(CN_HASHBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  m_pHashMap    = pHashMap;
  m_iDepth      = iDepth;
  m_bDuplicates = true;
  
  //m_xHashEntry[]
  for (i8 i_Collide = 0; i_Collide < C_HASH_COLLIDES; i_Collide++)
    m_paCollides[i_Collide] = nullptr;
  
  m_i_Duplicate = 0;
  m_pLinkDuplicates = pLinkDuplicates;
  
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
bool HashBuffer::bAppendDuplicate(HashEntry* aHeadEntry, i8 iValue)
{
  // check this buffer is used to hold duplicates
  RASSERT(m_bDuplicates);
  
  // is the buffer full?
  if (m_i_Duplicate + 1 > C_HASH_ENTRIES)
    return false;
  
  vTrace("D[%ld,%lld:%lld] %s/%lld %lld\n"_Stz,
         m_iDepth,
         m_i_Duplicate,
         aHeadEntry->m_iHash,
         
         aHeadEntry->m_yaKey,
         aHeadEntry->m_nBytes,
         aHeadEntry->m_iValue);
  
  // put the duplicate into next free entry but with new value
  m_xHashEntry[m_i_Duplicate].vSet(aHeadEntry->m_iHash,
                                   aHeadEntry->m_yaKey,
                                   aHeadEntry->m_nBytes,
                                   iValue);
  
  // this new duplicate enty points to the current dupe head
  m_xHashEntry[m_i_Duplicate].m_aDupeChain = aHeadEntry->m_aDupeChain;
  
  // the dupe head for the key point to this entry
  aHeadEntry->m_aDupeChain = &m_xHashEntry[m_i_Duplicate];
  
  m_i_Duplicate++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertEntry(i8 iHash, byte* yaKey, i8 nBytes, i8 iValue)
{
  i8 i_Entry = iHash % C_HASH_ENTRIES;
  
  // test if the entry is already in use
  if (C_UNKNOWN != m_xHashEntry[i_Entry].m_iHash) {
    vHandleOccupied(i_Entry,yaKey,nBytes,iValue);
    return;
  }
  
  vTrace("%c[%ld,%lld:%lld] %s/%lld %lld\n"_Stz,
         m_iDepth ? 'C' : 'I',
         
         m_iDepth,
         i_Entry,
         iHash,
         
         yaKey,
         nBytes,
         iValue);
  
  m_xHashEntry[i_Entry].vSet(iHash,yaKey,nBytes,m_iDepth);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* handle an occupation                                                       */
/*----------------------------------------------------------------------------*/
void HashBuffer::vHandleOccupied(i8 i_Entry, byte* yaKey, i8 nBytes, i8 iValue)
{
  // is it a key duplicate or a hashcollision?
  if (nBytes == m_xHashEntry[i_Entry].m_nBytes &&
      MemLib::bCompareBytes(m_xHashEntry[i_Entry].m_yaKey,yaKey,nBytes)) {
    // its a key duplicate
    m_pHashMap->vAppendDuplicate(&m_xHashEntry[i_Entry],iValue);
  }
  else {
    vTrace("o[%ld,%lld:%lld] %s/%lld %lld\n"_Stz,
           m_iDepth,
           i_Entry,
           m_xHashEntry[i_Entry].m_iHash,
           
           yaKey,
           nBytes,
           iValue);
    // its a hash collision
    
    vInsertCollision(yaKey,nBytes,iValue);
  }
}

/*----------------------------------------------------------------------------*/
/* insert a colliding key,value into this hash buffer                         */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertCollision(byte* yaKey, i8 nBytes, i8 iValue)
{
  i8 iHash = HashEntry::iHash(yaKey, nBytes,m_iDepth+1);

  i8 i_Collide = iHash % C_HASH_COLLIDES;
  
  if (! m_paCollides[i_Collide])
    m_paCollides[i_Collide] = new HashBuffer(m_pHashMap,m_iDepth+1);
  
  m_paCollides[i_Collide]->vInsertEntry(iHash,yaKey,nBytes,iValue);
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
