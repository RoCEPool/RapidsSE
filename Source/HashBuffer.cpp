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
HashBuffer::HashBuffer(HashMap* pHashMap, HashBuffer* pParent, i8 iArc,
                       bool bDuplicates)
: Object(CN_HASHBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  m_pHashMap    = pHashMap;
  m_pParent     = pParent;
  m_iDepth      = m_pParent ? m_pParent->m_iDepth + 1 : 0;
  m_iArc        = iArc;
  m_bDuplicates = bDuplicates;
  
  if (! m_pParent)
    m_sDotNotation = m_bDuplicates ? "d"_Stz : "r"_Stz;
  else {
    m_sDotNotation = m_pParent->m_sDotNotation;
    m_sDotNotation.vCatSprint(".%lld"_Stz,m_bDuplicates ? m_iDepth : m_iArc);
  }
  
  //m_xHashEntry[]
  for (i8 i_Collide = 0; i_Collide < C_HASH_COLLIDES; i_Collide++)
    m_paCollides[i_Collide] = nullptr;

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
bool HashBuffer::bAppendDuplicate(HashEntry* aHeadEntry, i8 iAltValue)
{
  // check this buffer is used to hold duplicates
  RASSERT(m_bDuplicates);
  
  // is the buffer full?
  if (m_i_Duplicate + 1 > C_HASH_ENTRIES)
    return false;
  
  // put the duplicate into next free entry but with alt value
  m_xHashEntry[m_i_Duplicate].vSet(aHeadEntry,iAltValue);
  
  // this new duplicate enty points to the current dupe head
  m_xHashEntry[m_i_Duplicate].m_aDupeChain = aHeadEntry->m_aDupeChain;
  
  // the dupe head for the key point to this entry
  aHeadEntry->m_aDupeChain = &m_xHashEntry[m_i_Duplicate];
  
  vDumpEntry(m_i_Duplicate,'D');

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
  
  m_xHashEntry[i_Entry].vSet(yaKey,nBytes,iValue,iHash);
  
  vDumpEntry(i_Entry,m_iDepth ? 'C' : 'I');
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump given entry                                                           */
/*----------------------------------------------------------------------------*/
void HashBuffer::vDumpEntry(i8 i_Entry, char cIndicator)
{
  HashEntry* aHashEntry = &m_xHashEntry[i_Entry];
  
  s256 sLead;
  
  sLead.vSprint("%c %s[%lld]"_Stz,
                cIndicator,
                m_sDotNotation.aChar(),
                i_Entry);
  
  vTrace("%-12s h:%08x k:%s/%lld/=v:%lld\n"_Stz,
         sLead.aChar(),
         
         aHashEntry->m_iHash,
         
         aHashEntry->m_yaKey,
         aHashEntry->m_nBytes,
         aHashEntry->m_iValue);
}

/*----------------------------------------------------------------------------*/
/* handle an occupation                                                       */
/*----------------------------------------------------------------------------*/
void HashBuffer::vHandleOccupied(i8 i_Entry, byte* yaKey, i8 nBytes, i8 iValue)
{
  vDumpEntry(i_Entry,'o');

  if (m_xHashEntry[i_Entry].bEqualTo(yaKey,nBytes)) {
    // its a key duplicate
    m_pHashMap->vAppendDuplicate(&m_xHashEntry[i_Entry],iValue);
  }
  else {
    // its a hash collision
    vInsertCollision(yaKey,nBytes,iValue);
  }
}

/*----------------------------------------------------------------------------*/
/* insert a colliding key,value into this hash buffer                         */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertCollision(byte* yaKey, i8 nBytes, i8 iValue)
{
  i8 iHash = HashEntry::iHash(yaKey,nBytes,m_iDepth+1);

  i8 i_Arc = iHash % C_HASH_COLLIDES;
  
  if (! m_paCollides[i_Arc])
    m_paCollides[i_Arc] = new HashBuffer(m_pHashMap,this,i_Arc,false);
  
  m_paCollides[i_Arc]->vInsertEntry(iHash,yaKey,nBytes,iValue);
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
