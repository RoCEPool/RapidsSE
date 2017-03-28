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
i8 Entry::nBytesRequired(i8 nDataBytes)
{
  i8 iBytes = sizeof(Entry)
                + nDataBytes
                + sizeof(Footer);
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry                                         */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(i8 nKeyBytes, i8 nLevels)
{
  i8 iBytes = sizeof(Entry)
                + nKeyBytes
                + Levels::nBytesRequired(nLevels)
                + sizeof(Footer);
  
  return iBytes;
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
void* Entry::operator new(size_t iSize, Entry* pEntry)
{
  RASSERT(iSize);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(i8 nDataBytes, i8 nLevels)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  m_nLevels = nLevels;
  m_xWsp.vUsed(nDataBytes);
  
  //_setFooter_();
  
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
           nOffset(iIndex));
  }
  
  vTrace("\n"_Str);
  
  //m_xRowId.pRow()->vContentDump();
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
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void Entry::vRowId(RowId& xRowId)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Entry::xRowId() = xRowId;
}

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vEntry(EntWsp& xEntWsp)
{
  //_precondition_(_MAGIC_,CN_ENTRY);

  i8 iLevels = xEntWsp.nLevels();
  
  vRowId(xEntWsp.xRowId());
  vLevels(iLevels);
  
  Str* aDst = m_xWsp.aStr();
  Str* aSrc = xEntWsp.aWspStr();
  i8 nBytes = xEntWsp.nWspUsed();

  MemLib::vCopy(aDst,aSrc,nBytes);
  
  m_xWsp.vUsed(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* Entry::aKey(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* pKey = reinterpret_cast<Key*>(m_xWsp.aArea() + (sizeof(i8)*(m_nLevels-1)));
  
  return pKey;
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
/* set nLevels                                                                */
/*----------------------------------------------------------------------------*/
void Entry::vLevels(u8 nLevels)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  m_nLevels = nLevels;
}

/*----------------------------------------------------------------------------*/
/* get next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
u8 Entry::nOffset(u8 nLevel)
{
  i8* nLevels = reinterpret_cast<i8*>(m_xWsp.aArea());
  return nLevels[nLevel];
}

/*----------------------------------------------------------------------------*/
/* set next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
void Entry::vOffset(u8 nLevel, u8 nOffset)
{
  i8* nLevels = reinterpret_cast<i8*>(m_xWsp.aArea());
  nLevels[nLevel] = nOffset;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence                                  */
/*----------------------------------------------------------------------------*/
i8 Entry::nSize(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);

  i8 nBytes = sizeof(Entry) + m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the right vector of this entry occurence                               */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return nullptr;
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
/* get entry data                                                             */
/*----------------------------------------------------------------------------*/
EntryData* Entry::aEntryData(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  EntryData* aEntryData = pAdd<EntryData*>(this,sizeof(Entry));
  
  return aEntryData;
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
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(Entry) + m_xWsp.nUsed());
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
