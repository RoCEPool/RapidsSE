/*============================================================================*/
/* Entry.cpp -- index entry (skip list variant)                               */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an Entry based on given EntWsp incl Object & Footer */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(EntWsp& xEntWsp)
{
  i8 nOffsets = xEntWsp.nOffsets();
  
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  i8 nBytes = Entry::nBytesRequired(nOffsets,nKeyBytes);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry incl Object & Footer                    */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(i8 nOffsets, i8 nKeyBytes)
{
  i8 nEntryDataBytes = EntryData::nBytesRequired(nOffsets,nKeyBytes);
  
  i8 nBytes = sizeof(Entry)
                + nEntryDataBytes
                + sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Entry* DBobjects::_aEntry(void* aAddr)
{
  Entry* aEntry = reinterpret_cast<Entry*>(aAddr);
  
  return aEntry;
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
void* Entry::operator new(size_t iSize, Entry* aEntry)
{
  RASSERT(iSize);
  
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(EntWsp& xEntWsp)
: Object(CN_ENTRY)
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
Entry::Entry(i8 nBytes, i8 nOffsets)                   //! should we pass EntWsp?
: Object(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  // initialise bytes
  m_nBytes = nBytes;
  
  // check initialisation of rowid
  ASSERT(0 == m_xRowId.uBuffer() && 0 == m_xRowId.uRow());
  
  // initialise offsets
  m_nOffsets = nOffsets;
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= C_FLAT_OFFSETS);        
  
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
void Entry::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* aKey = this->aKey();
  i8 iKey;
  
  if (m_xRowId.uBuffer() == 0) {
    ASSERT(m_xRowId.uRow() == 0);
    iKey = -1;
  }
  else {
    ValLib::nCopyDatum(CN_INTVAL,&iKey,aKey);
  }
  
  vTrace("Entry %p: Offsets: %llu "_Str,
         this,
         m_nOffsets
         );
  
  vTrace("Row %llu.%llu Key %lld "_Str,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         iKey
         );
  
  for (i8 iRight=0; iRight< m_nOffsets; iRight++) {
    vTrace("L%lld: %lld "_Str,
           iRight,
           uRightOffset(iRight));
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
/* get Key                                                                    */
/*----------------------------------------------------------------------------*/
Key* Entry::aKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);

  EntryData* pEntryData = this->aEntryData();

  Key* aKey = pEntryData->aKey(m_nOffsets);

  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get nOffsets                                                                */
/*----------------------------------------------------------------------------*/
u8 Entry::nOffsets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_nOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset for given level                                           */
/*----------------------------------------------------------------------------*/
u8 Entry::uRightOffset(u8 iOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(iOffset >= 0 && iOffset < C_FLAT_OFFSETS);
  
  if (iOffset >= m_nOffsets)
    return C_NIL_OFFSET;
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_nOffsets);

  u8 uRight = aRightVec->uOffset(iOffset);
  
  return uRight;
}

/*----------------------------------------------------------------------------*/
/* set right offset for given level                                           */
/*----------------------------------------------------------------------------*/
void Entry::vRightOffset(u8 iOffset, u8 uRight)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(iOffset >= 0 && iOffset < C_FLAT_OFFSETS);
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_nOffsets);

  aRightVec->vOffset(iOffset,uRight);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the RightVec for this Entry                                            */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_nOffsets);
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
RowId& Entry::xRowId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_xRowId;
}

/*----------------------------------------------------------------------------*/
/* set entry given entry wsp                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);

  ASSERT(m_nBytes);
  
  // set RowId
  m_xRowId  = xEntWsp.xRowId();

  // set Offsets
  m_nOffsets = xEntWsp.nOffsets();
  
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
  
  return m_nBytes;
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
  
  i8 iFooterOffset = m_nBytes - sizeof(Footer);
  
  Footer* pFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
