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
  vFatal("fatal: EntWrk class cannot be new'd\n"_Str);
  
  RASSERT(0);
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
  
  // m_xKeyWrk
  m_xKeyWrk.vInit();
  
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
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < gnIndexOffsets; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xKeyWrk
  m_xKeyWrk.vInit();
  
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
  
  // set no of offsets
  m_iOffsets = pEntry->nOffsets();
  
  // set the idx
  vId(pEntry->xId());
  
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  // set all right offsets to nil
  for (on nOffsetNo; nOffsetNo < m_iOffsets; nOffsetNo++) {
    vRightOffset(nOffsetNo, pEntry->iRightOffset(nOffsetNo));
  }
  
  // m_xKeyWrk
  m_xKeyWrk.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(Id& xId, KeyWrk& xKeyWrk)
: EntWrk()
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  vForm(xId,xKeyWrk);
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
void EntWrk::vShow(void)
{
  i8 iKey;
  
  ValLib::vCopyIntDatum(&iKey,aKey());
  
  vTrace("EntWrk %p: Id %lli.%lli Offsets: %llu "_Str,
         this,
         m_xId.iBuffer(),
         m_xId.iRow(),
         m_iOffsets
         );
  
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < m_iOffsets - 1; nOffsetNo++)
    vTrace("R%lld: %lld "_Str,
           *nOffsetNo,
           iRightOffset(nOffsetNo)
           );
  
  if (m_bHasData) {
    
    StrVal* pStrVal = reinterpret_cast<StrVal*>(aData());

    vTrace("data (size:%lld value:%s) "_Str,
           nDataSize(),
           pStrVal->aStr()
           );

  }
  else {
    vTrace("key (size:%lld value:%lld) "_Str,
           nKeySize(),
           iKey
           );
  }
  
  vTrace("\n"_Str);
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
  char* aData = reinterpret_cast<char*>(&m_xDataWrk);

  return aData;
}

/*----------------------------------------------------------------------------*/
/* get data size                                                              */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nDataSize(void)
{
  return m_xDataWrk.nUsed();
}

/*----------------------------------------------------------------------------*/
/* copy the row data                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vData(Str* aDataBytes, i8 iSize)
{
  i8 nBytes = StrVal::nBytesRequired(iSize);
 
  StrVal* pStrVal = new((StrVal*)aData()) StrVal((StrVal*)aData());
  pStrVal->vAssign(aDataBytes,iSize);
  m_xDataWrk.vTake(nBytes);
  
  //m_xDataWrk.vAppend(aDataBytes, iSize);
   
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
  Str* aKeySrc = xKeyWrk.aWspStr();
  i8 nKeyBytes = xKeyWrk.nWspUsed();
  
  m_xKeyWrk.vAppend(aKeySrc,nKeyBytes);
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
  return m_xKeyWrk.nUsed();
}

/*----------------------------------------------------------------------------*/
/*-- Datums ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
