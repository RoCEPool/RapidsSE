/*============================================================================*/
/* HashBuffer.cpp -- HashMap Buffer                                           */
/*============================================================================*/

#include "Structures.h"

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
  RASSERT(iArc < pHashMap->m_nArcsPerBuffer);
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHBUFFER,_FOOTER_);
  
  m_pHashMap    = pHashMap;
  m_pParent     = pParent;
  m_iDepth      = m_pParent ? m_pParent->m_iDepth + 1 : 0;
  m_iArc        = iArc;
  m_nStrata     = m_pParent
                    ? m_pParent->m_nStrata * pHashMap->m_nArcsPerBuffer + iArc
                    : 0 ;
  m_bDuplicates = bDuplicates;
  
  if (! m_pParent)
    m_sDotNotation = m_bDuplicates ? "d"_Stz : "r"_Stz;
  else {
    m_sDotNotation = m_pParent->m_sDotNotation;
    m_sDotNotation.vCatSprint(".%lld"_Stz,m_bDuplicates ? m_iDepth : m_iArc);
  }
  
  //m_xHashEntry[]
  for (i8 i_Arc = 0; i_Arc < C_HASH_ARCS; i_Arc++)
    m_paArcs[i_Arc] = nullptr;

  m_i_Duplicate = 0;
  m_pLinkDuplicates = nullptr;
  
  m_nEntries = 0;
  
  // increment number of buffers
  m_pHashMap->m_nBuffers++;
  if (m_bDuplicates)
    m_pHashMap->m_nDupBuffers++;
  
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
  
  for (i8 i_Arc = 0; i_Arc < C_HASH_ARCS; i_Arc++)
    if (m_paArcs[i_Arc])
      delete m_paArcs[i_Arc];
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print analysis of this hash buffer                                         */
/*----------------------------------------------------------------------------*/
void HashBuffer::vAnalysis(void)
{
  vTrace("    %s Entries %lld Full %lld%\n"_Stz,
         m_sDotNotation.azChar(),
         m_nEntries,
         IntLib::iPercent(m_nEntries, m_pHashMap->m_nEntriesPerBuffer));
  
  for (i8 i_Arc = 0; i_Arc < C_HASH_ARCS; i_Arc++)
    if (m_paArcs[i_Arc])
      m_paArcs[i_Arc]->vAnalysis();
}

/*----------------------------------------------------------------------------*/
/* append a key,value duplicate onto this hash buffer                         */
/*----------------------------------------------------------------------------*/
bool HashBuffer::bAppendDuplicate(HashInsert* pHi, HashEntry* aOriginalEntry)
{
  // check this buffer is used to hold duplicates
  RASSERT(m_bDuplicates);
  
  // is the buffer full?
  if (m_i_Duplicate + 1 > m_pHashMap->m_nEntriesPerBuffer)
    return false;
  
  // put the duplicate into next free entry but with alt value
  //m_xHashEntry[m_i_Duplicate].vSet(aHeadEntry,iAltValue);
  m_xHashEntry[m_i_Duplicate].vSet(aOriginalEntry,pHi->m_iValue);
  
  // this new duplicate enty points to the current dupe head
  m_xHashEntry[m_i_Duplicate].m_aDupeChain = aOriginalEntry->m_aDupeChain;
  
  // the dupe head for the key point to this entry
  aOriginalEntry->m_aDupeChain = &m_xHashEntry[m_i_Duplicate];
  
  // increment number of entries
  m_nEntries++;
  m_pHashMap->m_nEntries++;
  m_pHashMap->m_nDupEntries++;
  
  if (m_pHashMap->m_bTrace)
    vDumpEntry(m_i_Duplicate,'D');
  
  m_i_Duplicate++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compute i_Arc from hash                                                    */
/*----------------------------------------------------------------------------*/
i8 HashBuffer::iCalcArc(HashInsert* pHi)
{
  pHi->vArcEntry();
  
  i8 i_Arc = pHi->m_i_Arc;
  
  return i_Arc;
}

/*----------------------------------------------------------------------------*/
/* compute i_Entry from hash                                                  */
/*----------------------------------------------------------------------------*/
i8 HashBuffer::iCalcEntry(HashInsert* pHi)
{
  i8 i_Entry = pHi->m_i_item;
  
  return i_Entry;
}

/*----------------------------------------------------------------------------*/
/* dump given entry                                                           */
/*----------------------------------------------------------------------------*/
void HashBuffer::vDumpEntry(i8 i_Entry, char cIndicator)
{
  HashEntry* aHashEntry = &m_xHashEntry[i_Entry];
  
  s256 sLead;
  
  sLead.vSprint("%c %10s[%lld:%lld,%lld]"_Stz,
                cIndicator,
                m_sDotNotation.azChar(),
                
                m_iDepth,
                m_nStrata,
                i_Entry);
  
  vTrace("%-30s s:%016llx k:%6s/b:%lld/=v:%lld "
         "(%lld entries (%lld total) %lld%% full)\n"_Stz,
         
         sLead.azChar(),
         
         aHashEntry->m_uSignature,

         aHashEntry->m_yaKey,
         aHashEntry->m_nBytes,
         aHashEntry->m_iValue,
         
         m_nEntries,
         m_pHashMap->m_nEntries,
         IntLib::iPercent(m_nEntries, m_pHashMap->m_nEntriesPerBuffer)
         );
}

/*----------------------------------------------------------------------------*/
/* handle an occupation                                                       */
/*----------------------------------------------------------------------------*/
void HashBuffer::vHandleOccupied(HashInsert* pHi, i8 i_Entry)
{
  if (m_pHashMap->m_bTrace)
    vDumpEntry(i_Entry,'o');

  if (m_xHashEntry[i_Entry].bEqualTo(pHi->m_yaKey,pHi->m_nBytes)) {
    // its a key duplicate
    m_pHashMap->vAppendDuplicate(pHi, &m_xHashEntry[i_Entry]);
  }
  else {
    // its a hash collision
    vInsertCollision(pHi);
  }
}

/*----------------------------------------------------------------------------*/
/* insert a colliding key,value into this hash buffer                         */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertCollision(HashInsert* pHi)
{
  i8 i_Arc = iCalcArc(pHi);

  if (! m_paArcs[i_Arc])
    m_paArcs[i_Arc] = new HashBuffer(m_pHashMap,this,i_Arc,false);
  
  m_paArcs[i_Arc]->vInsertEntry(pHi);
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hash buffer                                   */
/*----------------------------------------------------------------------------*/
void HashBuffer::vInsertEntry(HashInsert* pHi)
{
   i8 i_Entry = iCalcEntry(pHi);

  // test if the entry is already in use
  if (m_xHashEntry[i_Entry].m_uSignature) {
    if (pHi->m_bDebug)
      vDebugPoint();
    vHandleOccupied(pHi,i_Entry);
    if (pHi->m_bArcTrc)
      vDumpEntry(i_Entry,'A');
    else if (pHi->m_bDebug)
      vDumpEntry(i_Entry,'C');
    return;
  }
  
  m_xHashEntry[i_Entry].vSet(pHi);
  
  if (pHi->m_bDebug || m_pHashMap->m_bTrace) {
    vDumpEntry(i_Entry,'I');
    vDebugPoint();
  }
  
  if (m_iDepth > m_pHashMap->m_iMaxDepth) {
    m_pHashMap->m_iMaxDepth = m_iDepth;
    
    pHi->vDumpHistory();
    
    //    if (pHi->m_bDebug) {
    pHi->m_bArcTrc = true;
    vDumpEntry(i_Entry,'A');
    //vDebugPoint();
    //    }
  }
  
  // increment number of entries
  m_nEntries++;
  m_pHashMap->m_nEntries++;
  
  if (m_iDepth)
    m_pHashMap->m_nArcEntries++;
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

/*----------------------------------------------------------------------------*/
/* compute i_Arc from hash                                                    */
/*----------------------------------------------------------------------------*/
/*
i8 HashBuffer::iArc(byte* yaKey, i8 nBytes, u8 uSignature)
{
  if (m_iDepth > 6)
    vDebugPoint();

  u8 uArcHash = uSignature;//HashEntry::iHashArc(yaKey,nBytes,m_iDepth+1);

  i8 nNextStrataBuffers  = (i8)pow(m_pHashMap->m_nArcsPerBuffer,m_iDepth + 1);
  i8 nNextStrataElements = nNextStrataBuffers * m_pHashMap->m_nEntriesPerBuffer;

  i8 i_StrataEntry  = uArcHash % nNextStrataElements;
  i8 i_StrataBuffer = i_StrataEntry / m_pHashMap->m_nEntriesPerBuffer;
//  i8 i_StrataArc    = i_StrataBuffer / m_pHashMap->m_nArcsPerBuffer;

//  i8 i_Arc = i_StrataArc >= m_pHashMap->m_nArcsPerBuffer
//               ? i_StrataArc - ((i_StrataArc -1) * m_pHashMap->m_nArcsPerBuffer)
//               : i_StrataArc;

  i8 i_Arc = i_StrataBuffer % m_pHashMap->m_nArcsPerBuffer;

  RASSERT(i_Arc >= 0 and i_Arc < m_pHashMap->m_nArcsPerBuffer);

  //i8 i_Arc = iHash % m_pHashMap->m_nArcsPerBuffer;

  return i_Arc;
}
*/

/*----------------------------------------------------------------------------*/
/* compute i_Entry from hash                                                  */
/*----------------------------------------------------------------------------*/
/*
i8 HashBuffer::iEntry(byte* yaKey, i8 nBytes, u8 uSignature)
{
  //i8 iEntryHash = HashEntry::iHashEntry(yaKey,nBytes,m_iDepth);
  u8 uEntryHash = uSignature;//HashEntry::iHashArc(yaKey,nBytes,m_iDepth);

  i8 nThisStrataBuffers  = (i8)pow(m_pHashMap->m_nArcsPerBuffer,m_iDepth);
  i8 nThisStrataElements = nThisStrataBuffers * m_pHashMap->m_nEntriesPerBuffer;

  i8 i_StrataEntry  = uEntryHash % nThisStrataElements;
  i8 i_StrataBuffer = i_StrataEntry / m_pHashMap->m_nEntriesPerBuffer;

  i8 i_Entry = i_StrataEntry - (i_StrataBuffer * m_pHashMap->m_nEntriesPerBuffer);

  //i8 i_Entry = iHash % m_pHashMap->m_nEntriesPerBuffer;

  RASSERT(i_Entry >= 0 and i_Entry < m_pHashMap->m_nEntriesPerBuffer);

  return i_Entry;
}
*/
