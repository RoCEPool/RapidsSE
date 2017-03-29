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
#ifdef OLD_ENTRY_MODEL
void Entry::vContentDump(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  i8 iKey;
  
  
  vTrace("Entry %p: Levels: %llu "_Str,
         this,
         m_nLevels
         );
  
  ValLib::nCopyDatum(CN_INTVAL, &iKey,m_xWsp.aArea()  + sizeof(i8)*(m_nLevels-1));
  vTrace("Row %llu.%llu Key (size:%lld value:%lld) "_Str,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         m_xWsp.nUsed() - sizeof(i8)*(m_nLevels-1),
         iKey
         );
  
  for (i8 iIndex=0; iIndex< m_nLevels - 1; iIndex++) {
    vTrace("L%lld: %lld "_Str,
           iIndex+1,
           uOffset(iIndex));
  }
  
  vTrace("\n"_Str);
  
  //m_xRowId.pRow()->vContentDump();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#else
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
           uOffset(iIndex));
  }
  
  vTrace("\n"_Str);
  
  //m_xRowId.pRow()->vContentDump();
}
#endif

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
#ifdef OLD_ENTRY_MODEL
Key* Entry::aKey(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* pKey;
  
  pKey = reinterpret_cast<Key*>(m_xWsp.aArea() + (sizeof(i8)*(m_nLevels-1)));
  
  return pKey;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#else
Key* Entry::aKey(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);

  EntryData* pEntryData = this->aEntryData();

  Key* aKey = pEntryData->aKey(m_nLevels);

  return aKey;
}
#endif

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
#ifdef OLD_ENTRY_MODEL
u8 Entry::uOffset(u8 nLevel)
{
  i8* nLevels = reinterpret_cast<i8*>(m_xWsp.aArea());
  return nLevels[nLevel];
}
#else
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
u8 Entry::uOffset(u8 iLevel)
{
  EntryData* aEntryData = this->aEntryData();
  Levels* aLevels = aEntryData->aLevels(m_nLevels);

  u8 uOffset = aLevels->uOffset(iLevel);

  return uOffset;
}
#endif

/*----------------------------------------------------------------------------*/
/* set next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
#ifdef OLD_ENTRY_MODEL
void Entry::vOffset(u8 nLevel, u8 uOffset)
{
  i8* nLevels = reinterpret_cast<i8*>(m_xWsp.aArea());
  nLevels[nLevel] = uOffset;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#else
void Entry::vOffset(u8 iLevel, u8 uOffset)
{

  EntryData* aEntryData = this->aEntryData();
  Levels* aLevels = aEntryData->aLevels(m_nLevels);

  aLevels->vRight(iLevel,uOffset);
}
#endif

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
#ifdef OLD_ENTRY_MODEL
void Entry::vSetEntry(EntWsp& xEntWsp)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  i8 iLevels = xEntWsp.nLevels();
  
  m_xRowId  = xEntWsp.xRowId();
  m_nLevels = iLevels;
  
  Str* aDst = m_xWsp.aStr();
  Str* aSrc = xEntWsp.aWspStr();
  i8 nBytes = xEntWsp.nWspUsed();
  
  MemLib::vCopy(aDst,aSrc,nBytes);
  
  m_xWsp.vUsed(nBytes);
  
  m_nBytes = sizeof(Entry) + m_xWsp.nUsed();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#else
void Entry::vSetEntry(EntWsp& xEntWsp)
{
  //_precondition_(_MAGIC_,CN_ENTRY);

  // set Bytes
  i8 nBytes = xEntWsp.nWspUsed();
  m_nBytes = sizeof(Entry) + nBytes;

  // set RowId
  m_xRowId  = xEntWsp.xRowId();

  // set Levels
  m_nLevels = xEntWsp.nLevels();

  // copy entry data
  Str* aSrc = xEntWsp.aWspStr();
  EntryData* aEntryData = this->aEntryData();
  aEntryData->vCopyBytes(aSrc,nBytes);
}
#endif

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
