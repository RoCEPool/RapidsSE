/*============================================================================*/
/* Entry.cpp -- index entry                                                   */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
Entry::Entry(i8 nBytes, i8 nLevels)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  m_nBytes  = nBytes;
  m_nLevels = nLevels;
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
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
/* get next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
u8 Entry::uRight(u8 iLevel)
{
  EntryData* aEntryData = this->aEntryData();
  Levels* aLevels = aEntryData->aLevels(m_nLevels);

  u8 uRight = 0;
  if (iLevel)
    uRight = aLevels->uRight(iLevel);

  return uRight;
}

/*----------------------------------------------------------------------------*/
/* set next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
void Entry::vRight(u8 iLevel, u8 uRight)
{

  EntryData* aEntryData = this->aEntryData();
  Levels* aLevels = aEntryData->aLevels(m_nLevels);

  aLevels->vRight(iLevel,uRight);
}

/*----------------------------------------------------------------------------*/
/* get a right entry of this entry occurence                                  */
/*----------------------------------------------------------------------------*/
Entry* Entry::aRightEntry(i8 iLevel)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  /*
   
   we can't call with iLevel=0, because the caller should have used
   IndexBuffer::aEntryNext()
   
   */
  
  ASSERT(iLevel > 0 && iLevel < IndexBuffer::C_FLAT_LEVELS);
  
  // i8 iLevelSub = iLevel - 1;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
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
  i8 nBytes = xEntWsp.nWspUsed();
  m_nBytes = sizeof(Entry) + nBytes + sizeof(Footer);

  // set RowId
  m_xRowId  = xEntWsp.xRowId();

  // set Levels
  m_nLevels = xEntWsp.nLevels();

  // copy entry data
  Str* aSrc = xEntWsp.aWspStr();
  EntryData* aEntryData = this->aEntryData();
  aEntryData->vCopyBytes(aSrc,nBytes);
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
