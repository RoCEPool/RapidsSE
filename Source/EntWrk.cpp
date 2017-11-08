/*============================================================================*/
/* EntWrk.cpp -- Working Entry                                                */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

extern i8 gnIndexOffsets;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t iSize, EntWrk* pEntWrk)
{
  RASSERT(iSize);
  return pEntWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(void)
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  m_bHasData = false;
  
  // generate logarithmic level for this entry
  m_iOffsets = Random::iLogLevel(gnIndexOffsets+1, C_FLAT_FREQ);
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < gnIndexOffsets; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xDataWrk
  m_xDataWrk.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(i8 nOffsets)
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  // set no of offsets
  m_iOffsets = nOffsets;
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  m_bHasData = false;
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < gnIndexOffsets; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xDataWrk
  m_xDataWrk.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(Entry* pEntry)
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  m_bHasData = false;
  
  // set no of offsets
  m_iOffsets = pEntry->nOffsets();
  
  // set the idx
  vId(pEntry->xId());
  
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  // set all right offsets to nil
  for (on nOffsetNo; nOffsetNo < m_iOffsets; nOffsetNo++) {
    vRightOffset(nOffsetNo, pEntry->iRightOffset(nOffsetNo));
  }
  
  // m_xDataWrk
  m_xDataWrk.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(Id& xId, i8 iTxId, KeyWrk& xKeyWrk)
: EntWrk()
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  vForm(xId,xKeyWrk);
  
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk::~EntWrk(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Entry Proper ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
bool EntWrk::bHasData(void)
{
  return m_bHasData;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntWrk::xId(void)
{
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vId(Id& xId)
{
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iTxId(void)
{
  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk::vTxId(i8 iTxId)
{
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(ColumnVec* pColVec)
{
  i8 iKey;
  
  Int::vCopy(&iKey,aKey());
  
  vTrace("EntWrk %p: Id %lli.%lli Offsets: %llu "_Stz,
         this,
         m_xId.iBuffer(),
         m_xId.iRow(),
         m_iOffsets
         );
  
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < m_iOffsets - 1; nOffsetNo++) {
    vTrace("R%lld: %lld "_Stz,
           *nOffsetNo,
           iRightOffset(nOffsetNo)
           );
  }
  vTrace("key (size:%lld value:%lld) "_Stz,
         nKeySize(),
         iKey
         );
  
  if (m_bHasData) {
    RowData::vShow((Stz*)aData(), pColVec);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Offsets -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nOffsets(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  return m_iOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset at level                                                  */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iRightOffset(on nOffsetNo)
{
  ASSERT(nOffsetNo >= 0_on && nOffsetNo <= gnIndexOffsets);
  
  return m_iOffset[*nOffsetNo];
}

/*----------------------------------------------------------------------------*/
/* set right offset at level                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk::vRightOffset(on nOffsetNo, u8 uRightOffset)
{
  ASSERT(nOffsetNo >= 0_on && nOffsetNo <= gnIndexOffsets);
  
  m_iOffset[*nOffsetNo] = uRightOffset;
}

/*----------------------------------------------------------------------------*/
/* get size of RightVec                                                       */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nRightSize(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  i8 nBytes = RightVec::nBytesRequired(m_iOffsets);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get RightVec                                                               */
/*----------------------------------------------------------------------------*/
RightVec* EntWrk::aRightVec(void)
{
  RightVec* aRightVec = &m_xRight;
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/*-- Data --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get data pointer                                                           */
/*----------------------------------------------------------------------------*/
char* EntWrk::aData(void)
{
  ASSERT(m_bHasData);
  
  char* aKey = reinterpret_cast<char*>(&m_xDataWrk);
  // assuming key of type CT_INTEGER in hidden index
  char* aData = pAdd<char*>(aKey, sizeof(i8));                       //ALKA-TODO

  return aData;
}

/*----------------------------------------------------------------------------*/
/* get data size                                                              */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nDataSize(void)
{
  if (m_bHasData)
    // assuming key of type CT_INTEGER in hidden index
    return m_xDataWrk.nUsed() - sizeof(i8);                          //ALKA-TODO
  else
    return 0;
  
}

/*----------------------------------------------------------------------------*/
/* copy the row data                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vData(Stz* aDataBytes, i8 iSize)
{
  char* aDataArea = m_xDataWrk.aLatter();
  MemLib::vCopy(aDataArea,aDataBytes,iSize,CN_ENTWRK);
  
  m_xDataWrk.vTake(iSize);
  
  m_bHasData = true;
}

/*----------------------------------------------------------------------------*/
/*-- Key ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddKey(KeyWrk& xKeyWrk)
{
  Stz* aKeySrc = xKeyWrk.aWspStr();
  i8 nKeyBytes = xKeyWrk.nWspUsed();
  
  m_xDataWrk.vAppend(aKeySrc,nKeyBytes,CN_ENTWRK);
}

/*----------------------------------------------------------------------------*/
/* form a simple entry using given id and key                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vForm(Id& xId, KeyWrk& xKeyWrk)
{
  m_xId = xId;
  vAddKey(xKeyWrk);
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* EntWrk::aKey(void)
{
  Key* aKey = &m_xKey;
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nKeySize(void)
{
  if (m_bHasData)
    return sizeof(i8);
  else
    return m_xDataWrk.nUsed();
}

/*----------------------------------------------------------------------------*/
/*-- Datums ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_aFooter_(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
