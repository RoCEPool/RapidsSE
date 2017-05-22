/*============================================================================*/
/* EntrySeq.cpp -- index entry (sequential variant)                           */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an EntrySeq based on given EntWsp                   */
/*----------------------------------------------------------------------------*/
i8 EntrySeq::nBytesRequired(EntWsp& xEntWsp)
{
  i8 nLevels = xEntWsp.nOffsets();
  i8 nKeyBytes = xEntWsp.nKeySize();
  i8 nBytes = EntrySeq::nBytesRequired(nLevels,nKeyBytes);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry                                         */
/*----------------------------------------------------------------------------*/
i8 EntrySeq::nBytesRequired(i8 nLevels, i8 nKeyBytes)
{
  i8 nEntryDataBytes = EntryData::nBytesRequired(nLevels,nKeyBytes);
  
  i8 nBytes = sizeof(EntrySeq)
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
EntrySeq* DBobjects::_aEntrySeq(void* aAddr)
{
  EntrySeq* aEntrySeq = reinterpret_cast<EntrySeq*>(aAddr);
  
  return aEntrySeq;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntrySeq::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: EntrySeq class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntrySeq::operator new(size_t iSize, EntrySeq* aEntrySeq)
{
  RASSERT(iSize);
  
  return aEntrySeq;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntrySeq::EntrySeq(i8 nBytes, i8 nLevels)
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
EntrySeq::~EntrySeq(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void EntrySeq::vContentDump(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  Key* aKey = this->aKey();
  
  i8 iKey;
  
  vTrace("EntrySeq %p: Levels: %llu "_Str,
         this,
         m_nLevels
         );
  
  ValLib::nCopyDatum(CN_INTVAL,&iKey,aKey);
  
  vTrace("Row %llu.%llu Key %lld"_Str,
         m_xId.uBuffer(),
         m_xId.uRow(),
         iKey
         );
  
  for (u8 iIndex=0; iIndex< m_nLevels - 1; iIndex++) {
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
EntryData* EntrySeq::aEntryData(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = pAdd<EntryData*>(this,sizeof(EntrySeq));
  
  return aEntryData;
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* EntrySeq::aKey(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);

  EntryData* pEntryData = this->aEntryData();

  Key* aKey = pEntryData->aKey(m_nLevels);

  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get nLevels                                                                */
/*----------------------------------------------------------------------------*/
u8 EntrySeq::nLevels(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_nLevels;
}

/*----------------------------------------------------------------------------*/
/* get next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
u8 EntrySeq::uRight(u8 iLevel)
{
  EntryData* aEntryData = this->aEntryData();
  RightVec* aRightVec = aEntryData->aRightVec(m_nLevels);

  u8 uRight = 0;
  //if (iLevel)
  //  uRight = aRightVec->uOffset(iLevel);

  return uRight;
}

/*----------------------------------------------------------------------------*/
/* set next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
void EntrySeq::vRight(u8 iLevel, u8 uRight)
{

  EntryData* aEntryData = this->aEntryData();
  RightVec* aRightVec = aEntryData->aRightVec(m_nLevels);

  aRightVec->vOffset(iLevel,uRight);
}

/*----------------------------------------------------------------------------*/
/* get a right entry of this entry occurence                                  */
/*----------------------------------------------------------------------------*/
EntrySeq* EntrySeq::aRightEntry(i8 iLevel)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get RightVec                                                               */
/*----------------------------------------------------------------------------*/
RightVec* EntrySeq::aRightVec(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  RightVec* aRightVec = pAdd<RightVec*>(this,sizeof(EntrySeq));
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntrySeq::xId(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  // return reference to the RowId inherited class
  //return dynamic_cast<RowId&>(*this);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void EntrySeq::vSetEntry(EntWspSeq& xEntWspSeq)
{
  //_precondition_(_MAGIC_,CN_ENTRY);

  // set Bytes
  m_nBytes = xEntWspSeq.nEntryBytesRequired();
  
  // set RowId
  m_xId  = xEntWspSeq.xId();

  // set Levels
  m_nLevels = xEntWspSeq.nLevels();

  // copy levels
  RightVec* aEntryRightVec = this->aRightVec();
  RightVec* aEntWspRightVec = xEntWspSeq.aRightVec();
  i8 nRightBytes = xEntWspSeq.nRightSize();
  
  MemLib::vCopy(aEntryRightVec,aEntWspRightVec,nRightBytes);
  
  // copy key
  Key* aEntryKey = this->aKey();
  Key* aEntWspKey = xEntWspSeq.aKey();
  i8 nKeyBytes = xEntWspSeq.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWspKey,nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence                                  */
/*----------------------------------------------------------------------------*/
i8 EntrySeq::nSize(void)
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
Footer* EntrySeq::pFooter(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY);
  
  i8 iFooterOffset = m_nBytes - sizeof(Footer);
  
  Footer* pFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
