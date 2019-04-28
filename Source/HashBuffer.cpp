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
HashBuffer::HashBuffer(void)
: Object(CN_HASHBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  m_i_Duplicate = 0;
  //m_xHashEntry[]

  m_pDuplicates = nullptr;

  for (i8 i_Colide = 0; i_Colide < C_HASH_COLLIDES; i_Colide++)
    m_paCollides[i_Colide] = nullptr;
  
  _checkInitialised_(CN_HASHBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashBuffer::~HashBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  delete m_pDuplicates;
  
  for (i8 i_Colide = 0; i_Colide < C_HASH_COLLIDES; i_Colide++)
    if (m_paCollides[i_Colide])
      delete m_paCollides[i_Colide];
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vAppendDuplicate(i8 iDepth, i8 iHash, byte* yaKey, i8 nBytes, i8 iValue)
{
  if (m_i_Duplicate + 1 >= C_HASH_ENTRIES) {
    m_pDuplicates = new HashBuffer;
    m_pDuplicates->vAppendDuplicate(iDepth+1,iHash,yaKey,nBytes,iValue);
    return;
  }

  m_i_Duplicate++;
  
  m_xHashEntry[m_i_Duplicate].m_iHash = iHash;
  MemLib::vCopy(m_xHashEntry[m_i_Duplicate].m_yaKey,yaKey,nBytes,CN_HASHBUFFER);
  m_xHashEntry[m_i_Duplicate].m_nBytes = nBytes;
  m_xHashEntry[m_i_Duplicate].m_iValue = iValue;
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsert(i8 iDepth, i8 iHash, byte* yaKey, i8 nBytes, i8 iValue)
{
  i8 i_Entry = iHash % C_HASH_ENTRIES;
  
  if (C_UNKNOWN != m_xHashEntry[i_Entry].m_iHash) {
    vInsertCollide(iDepth,yaKey,nBytes,iValue);
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
void HashBuffer::vInsertCollide(i8 iDepth, byte* yaKey, i8 nBytes, i8 iValue)
{
  byte ayKeyReverse[32];
  
  i8 i_Src = nBytes - 1;
  for (i8 i_Dst = 0; i_Dst < nBytes; i_Dst++, i_Src--)
    ayKeyReverse[i_Dst] = yaKey[i_Src];
  
  i8 iHashRev = HshLib::iHash(ayKeyReverse,nBytes);
  
  i8 i_Colide = iHashRev % C_HASH_COLLIDES;
  if (! m_paCollides[i_Colide])
    m_paCollides[i_Colide] = new HashBuffer;
  m_paCollides[i_Colide]->vInsert(iDepth + 1,iHashRev,yaKey,nBytes,iValue);
  
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertDuplicate(i8 iDepth, byte* yaKey, i8 nBytes, i8 iValue)
{
}

/*----------------------------------------------------------------------------*/
/* iterate                                                                    */
/*----------------------------------------------------------------------------*/
bool HashBuffer::bIterate(i8& iCursor, byte*& yaKey, i8& nBytes, i8& iValue)
{
  for ( ; iCursor < C_HASH_ENTRIES; iCursor++)
    if (C_UNKNOWN != m_xHashEntry[iCursor].m_iHash) {
      yaKey   = &m_xHashEntry[iCursor].m_yaKey[0];
      nBytes = m_xHashEntry[iCursor].m_nBytes;
      iValue = m_xHashEntry[iCursor].m_iValue;
      iCursor++;
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
