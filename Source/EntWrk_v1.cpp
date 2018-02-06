/*============================================================================*/
/* EntWrk.cpp -- Working Entry                                                */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;
extern i8 gnIndexOffsets;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk_v1::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk_v1::operator new(size_t iSize, EntWrk* pEntWrk)
{
  RASSERT(iSize);
  return pEntWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk_v1::EntWrk_v1(void)
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  m_bHasData = false;
  
  // generate logarithmic level for this entry
  m_iOffsets = Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1, C_FLAT_FREQ);
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gpAttachMap->m_nIndexOffsets);
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < gpAttachMap->m_nIndexOffsets; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xDataWrk
  m_xDataWrk.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk_v1::EntWrk_v1(i8 nOffsets)
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  // set no of offsets
  m_iOffsets = nOffsets;
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gpAttachMap->m_nIndexOffsets);
  
  m_bHasData = false;
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < gpAttachMap->m_nIndexOffsets; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xDataWrk
  m_xDataWrk.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk_v1));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk_v1::EntWrk_v1(Entry* pEntry)
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
  
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gpAttachMap->m_nIndexOffsets);
  
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
EntWrk_v1::EntWrk(Id& xId, i8 iTxId, KeyWrk& xKeyWrk)
: EntWrk()
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  vForm(xId,xKeyWrk);
  
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk_v1::~EntWrk_v1(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Entry Proper ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
bool EntWrk_v1::bHasData(void)
{
  return m_bHasData;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntWrk_v1::xId(void)
{
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk_v1::vId(Id& xId)
{
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 EntWrk_v1::iTxId(void)
{
  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk_v1::vTxId(i8 iTxId)
{
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk_v1::vShow(ColumnVec* pColVec)
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
    Row* aThisRow = aRow();
    aThisRow->vShow(pColVec);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Offsets -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk_v1::nOffsets(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gpAttachMap->m_nIndexOffsets);
  
  return m_iOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset at level                                                  */
/*----------------------------------------------------------------------------*/
i8 EntWrk_v1::iRightOffset(on nOffsetNo)
{
  ASSERT(nOffsetNo >= 0_on && nOffsetNo <= gpAttachMap->m_nIndexOffsets);
  
  return m_iOffset[*nOffsetNo];
}

/*----------------------------------------------------------------------------*/
/* set right offset at level                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk_v1::vRightOffset(on nOffsetNo, u8 uRightOffset)
{
  ASSERT(nOffsetNo >= 0_on && nOffsetNo <= gpAttachMap->m_nIndexOffsets);
  
  m_iOffset[*nOffsetNo] = uRightOffset;
}

/*----------------------------------------------------------------------------*/
/* get size of RightVec                                                       */
/*----------------------------------------------------------------------------*/
i8 EntWrk_v1::nRightSize(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gpAttachMap->m_nIndexOffsets);
  
  i8 nBytes = RightVec::nBytesRequired(m_iOffsets);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get RightVec                                                               */
/*----------------------------------------------------------------------------*/
RightVec* EntWrk_v1::aRightVec(void)
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
char* EntWrk_v1::aData(void)
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
i8 EntWrk_v1::nDataSize(void)
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
void EntWrk_v1::vData(Stz* aDataBytes, i8 iSize)
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
void EntWrk_v1::vAddKey(KeyWrk& xKeyWrk)
{
  Stz* aKeySrc = xKeyWrk.aWspStr();
  i8 nKeyBytes = xKeyWrk.nWspUsed();
  
  m_xDataWrk.vAppend(aKeySrc,nKeyBytes,CN_ENTWRK);
}

/*----------------------------------------------------------------------------*/
/* form a simple entry using given id and key                                 */
/*----------------------------------------------------------------------------*/
void EntWrk_v1::vForm(Id& xId, KeyWrk& xKeyWrk)
{
  m_xId = xId;
  vAddKey(xKeyWrk);
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* EntWrk_v1::aKey(void)
{
  //Key* aKey = &m_xKey;
  Key* aKey = reinterpret_cast<Key*>(&m_xDataWrk);
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWrk_v1::nKeySize(void)
{
  if (m_bHasData)
    return sizeof(i8);
  else
    return m_xDataWrk.nUsed();
}

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* EntWrk_v1::aRow(void)
{
  ASSERT(m_bHasData);
  
  Row* aThisRow = (Row*)aData();
  
  return aThisRow;
}

/*----------------------------------------------------------------------------*/
/*-- Datums ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_aFooter_(EntWrk_v1)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
