/*============================================================================*/
/* Entry.cpp -- index entry (skip list variant)                               */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an Entry based on given EntWsp                      */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(EntWsp& xEntWsp)
{
  i8 nLevels = xEntWsp.nLevels();
  i8 nKeyBytes = xEntWsp.nKeySize();
  i8 nBytes = Entry::nBytesRequired(nLevels,nKeyBytes);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry                                         */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(i8 nLevels, i8 nKeyBytes)
{
  i8 nEntryDataBytes = EntryData::nBytesRequired(nLevels,nKeyBytes);
  
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
Entry::Entry(i8 nBytes, i8 nLevels)                   //! should we pass EntWsp?
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  // initialise bytes
  m_nBytes  = nBytes;
  
  // check initialisation of rowid
  ASSERT(0 == m_xRowId.uBuffer() && 0 == m_xRowId.uRow());
  
  // initialise levels
  m_nLevels = nLevels;
  
  // entry data will be initialised by vSetEntry       //! Is this correct form?
  
  _setFooter_();
  
  _checkInitialised_(CN_ENTRY);
  
  //_postcondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  Key* aKey = this->aKey();
  
  i8 iKey;
  
  vTrace("Entry %p: Levels: %llu "_Str,
         this,
         m_nLevels
         );
  
  ValLib::nCopyDatum(CN_INTVAL,&iKey,aKey);
  
  vTrace("Row %llu.%llu Key %lld"_Str,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         iKey
         );
  
  for (i8 iIndex=0; iIndex< m_nLevels - 1; iIndex++) {
    vTrace("L%lld: %lld "_Str,
           iIndex+1,
           uRight(iIndex));
  }
  
  vTrace("\n"_Str);
  
  //m_xRowId.pRow()->vContentDump();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get entry data                                                             */
/*----------------------------------------------------------------------------*/
EntryData* Entry::aEntryData(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = pAdd<EntryData*>(this,sizeof(Entry));
  
  return aEntryData;
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* Entry::aKey(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);

  EntryData* pEntryData = this->aEntryData();

  Key* aKey = pEntryData->aKey(m_nLevels);

  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get nLevels                                                                */
/*----------------------------------------------------------------------------*/
u8 Entry::nLevels(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_nLevels;
}

/*----------------------------------------------------------------------------*/
/* get right offset for given level                                           */
/*----------------------------------------------------------------------------*/
u8 Entry::uRight(u8 iLevel)
{
  EntryData* aEntryData = this->aEntryData();
  RightVec* aRightVec = aEntryData->aRightVec(m_nLevels);

  u8 uRight = 0;
  if (iLevel)
    uRight = aRightVec->uRight(iLevel);

  return uRight;
}

/*----------------------------------------------------------------------------*/
/* set right offset for given level                                           */
/*----------------------------------------------------------------------------*/
void Entry::vRight(u8 iLevel, u8 uRight)
{

  EntryData* aEntryData = this->aEntryData();
  RightVec* aRightVec = aEntryData->aRightVec(m_nLevels);

  aRightVec->vRight(iLevel,uRight);
}

/*----------------------------------------------------------------------------*/
/* get the entire RightVec for this Entry                                     */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  RightVec* aRightVec = pAdd<RightVec*>(this,sizeof(Entry));
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
RowId& Entry::xRowId(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  // return reference to the RowId inherited class
  //return dynamic_cast<RowId&>(*this);
  
  return m_xRowId;
}

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(EntWsp& xEntWsp)
{
  //_precondition_(_MAGIC_,CN_ENTRY);

  // set Bytes
  m_nBytes = nBytesRequired(xEntWsp);
  
  // set RowId
  m_xRowId  = xEntWsp.xRowId();

  // set Levels
  m_nLevels = xEntWsp.nLevels();

  // copy levels
  RightVec* aEntryRightVec = this->aRightVec();
  RightVec* aEntWspRightVec = xEntWsp.aRightVec();
  i8 nRightBytes = xEntWsp.nRightSize();
  
  MemLib::vCopy(aEntryRightVec,aEntWspRightVec,nRightBytes);
  
  // copy key
  Key* aEntryKey = this->aKey();
  Key* aEntWspKey = xEntWsp.aKey();
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWspKey,nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence                                  */
/*----------------------------------------------------------------------------*/
i8 Entry::nSize(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
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
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  i8 iFooterOffset = m_nBytes - sizeof(Footer);
  
  Footer* pFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
