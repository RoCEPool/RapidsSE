/*============================================================================*/
/* Entry.cpp -- index entry (skip list variant)                               */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

extern i8 gnIndexOffsets;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Entry* DBobjects::_pEntry(void* aAddr)
{
  Entry* pEntry = reinterpret_cast<Entry*>(aAddr);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Entry class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize, Entry* pEntry)
{
  RASSERT(iSize);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(EntWsp& xEntWsp)
: BufferItem(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  _setFooter_();

  vSetEntry(xEntWsp);
  
  _checkInitialised_(CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(i8 nBytes, i8 iOffsets)                   //! should we pass EntWsp?
: BufferItem(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  // initialise bytes
  m_iBytes = nBytes;
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  // initialise offsets
  m_iOffsets = iOffsets;
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  // entry data will be initialised by vSetEntry       //! Is this correct form?
  
  _setFooter_();
  
  _checkInitialised_(CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iOffset, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* aKey = this->aKey();
  i8 iKey;
  
  if (m_xId.iBuffer() == 0) {
    ASSERT(m_xId.iRow() == 0);
    iKey = -1;
  }
  else {
    ValLib::nCopyDatum(CN_INTVAL,&iKey,aKey);
  }
  
  vTrace("Entry %p: Size: %llu Offsets: %llu "_Str,
         this,
         nSize(),
         m_iOffsets
         );
  
  vTrace("Id %3lli.%3lli "_Str,
         m_xId.iBuffer(),
         m_xId.iRow()
         );
  aKey->vShow(pColVec);
  
  on nRight;
  for (nRight = 0_on; nRight < m_iOffsets; nRight++) {
    
    vTrace(" Offset(%lld): %lld "_Str,
           *nRight,
           iRightOffset(nRight) == C_NIL_OFFSET ?
              C_NIL_OFFSET :
              iRightOffset(nRight) + iOffset + nSize());
  }
  
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get EntryData                                                              */
/*----------------------------------------------------------------------------*/
EntryData* Entry::aEntryData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = pAdd<EntryData*>(this,sizeof(Entry));
  
  return aEntryData;
}

/*----------------------------------------------------------------------------*/
/* get bufferno from id of key                                                */
/*----------------------------------------------------------------------------*/
bn Entry::nBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  bn nBufferNo = m_xId.iBuffer();
  
  return nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get highest offset                                                         */
/*----------------------------------------------------------------------------*/
on Entry::nHighestOffset(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  on nHighest(m_iOffsets - 1);
  
  return nHighest;
}

/*----------------------------------------------------------------------------*/
/* get indexbuffer of this entry                                              */
/*----------------------------------------------------------------------------*/
IndexBuffer* Entry::pIndexBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  bn nBufferNo = m_xId.iBuffer();
  IndexBuffer* pIndexBuffer =
    reinterpret_cast<IndexBuffer*>(gpBufferPool->pToBuffer(nBufferNo));
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* get Key                                                                    */
/*----------------------------------------------------------------------------*/
Key* Entry::aKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);

  EntryData* pEntryData = this->aEntryData();

  Key* aKey = pEntryData->aKey(m_iOffsets);

  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get nOffsets                                                               */
/*----------------------------------------------------------------------------*/
i8 Entry::nOffsets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_iOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset for given level                                           */
/*----------------------------------------------------------------------------*/
i8 Entry::iRightOffset(on nOffsetNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gnIndexOffsets);
  
  if (nOffsetNo >= m_iOffsets)
    return C_NIL_OFFSET;
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_iOffsets);

  i8 iRight = aRightVec->iOffset(nOffsetNo);
  
  return iRight;
}

/*----------------------------------------------------------------------------*/
/* set right offset for given level                                           */
/*----------------------------------------------------------------------------*/
void Entry::vRightOffset(on nOffsetNo, i8 iRightOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gnIndexOffsets);
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_iOffsets);
  
  aRightVec->vOffset(nOffsetNo,iRightOffset);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the RightVec for this Entry                                            */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_iOffsets);
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
Id& Entry::xId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vId(Id xId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* set key given entry wsp                                                    */
/*----------------------------------------------------------------------------*/
void Entry::vKey(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(m_iBytes);
  
  // copy Key
  Key* aEntryKey = this->aKey();
  
  Key* aEntWspKey = xEntWsp.aKey();
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWspKey,nKeyBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set entry given entry wsp                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);

  ASSERT(m_iBytes);
  
  // set RowId
  m_xId  = xEntWsp.xId();

  // set Offsets
  m_iOffsets = xEntWsp.nOffsets();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // copy RightVec
  RightVec* aEntryRightVec = this->aRightVec();
  RightVec* aEntWspRightVec = xEntWsp.aRightVec();
  i8 nRightBytes = xEntWsp.nRightSize();
  
  MemLib::vCopy(aEntryRightVec,aEntWspRightVec,nRightBytes);
  
  // copy Key
  Key* aEntryKey = this->aKey();
  Key* aEntWspKey = xEntWsp.aKey();
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWspKey,nKeyBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence incl Object & Footer             */
/*----------------------------------------------------------------------------*/
i8 Entry::nSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_iBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Entry::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  i8 iFooterOffset = m_iBytes - sizeof(Footer);
  
  Footer* pFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
