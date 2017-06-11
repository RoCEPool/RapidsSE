/*============================================================================*/
/* EntWrk.cpp -- Entry Working Object (skip list variant)                     */
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
  
  // generate logarithmic level for this entry
  m_iOffsets = Random::iLogLevel(gnIndexOffsets+1, C_FLAT_FREQ);
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < gnIndexOffsets; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xKeyWsp
  m_xKeyWsp.vInit();
  
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
  
  // m_xKeyWsp
  m_xKeyWsp.vInit();
  
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
  
  // m_xKeyWsp
  m_xKeyWsp.vInit();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(Id& xId, KeyWsp& xKeyWsp)
: EntWrk()
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  vForm(xId,xKeyWsp);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk::~EntWrk(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddKey(KeyWsp& xKeyWsp)
{
  Str* aKeySrc = xKeyWsp.aWspStr();
  i8 nKeyBytes = xKeyWsp.nWspUsed();
  
  m_xKeyWsp.vAppend(aKeySrc,nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* form a simple entry using given id and key                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vForm(Id& xId, KeyWsp& xKeyWsp)
{
  m_xId = xId;
  vAddKey(xKeyWsp);
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
  return m_xKeyWsp.nUsed();
}

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
  
  vTrace("key (size:%lld value:%lld) "_Str,
         nKeySize(),
         iKey
         );
  
  vTrace("\n"_Str);
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
/* get size of RightVec                                                       */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nRightSize(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
 
  i8 nBytes = RightVec::nBytesRequired(m_iOffsets);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
