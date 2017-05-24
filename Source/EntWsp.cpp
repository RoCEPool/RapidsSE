/*============================================================================*/
/* EntWsp.cpp -- Entry Workspace (skip list variant)                          */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWsp::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: EntWsp class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWsp::operator new(size_t iSize, EntWsp* pEntWsp)
{
  RASSERT(iSize);
  return pEntWsp;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWsp::EntWsp(void)
: Object(CN_ENTWSP)
{
  _precondition_(_MAGIC_,CN_ENTWSP);

  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  // generate logarithmic level for this entry
  m_iOffsets = Random::iLogLevel(C_FLAT_OFFSETS+1, C_FLAT_FREQ);
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= C_FLAT_OFFSETS);
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < C_FLAT_OFFSETS; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xKeyWsp
  m_xKeyWsp.vInit();
  
  _checkInitialisedSize_(sizeof(EntWsp));
  
  _postcondition_(_MAGIC_,CN_ENTWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWsp::EntWsp(i8 nOffsets)
: Object(CN_ENTWSP)
{
  _precondition_(_MAGIC_,CN_ENTWSP);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  // set no of offsets
  m_iOffsets = nOffsets;
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= C_FLAT_OFFSETS);
  
  // set all right offsets to nil
  on nOffsetNo;
  for (nOffsetNo = 0_on; nOffsetNo < C_FLAT_OFFSETS; nOffsetNo++)
    m_iOffset[*nOffsetNo] = C_NIL_OFFSET;
  
  // m_xKeyWsp
  m_xKeyWsp.vInit();
  
  _checkInitialisedSize_(sizeof(EntWsp));
  
  _postcondition_(_MAGIC_,CN_ENTWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWsp::EntWsp(Id& xId, KeyWsp& xKeyWsp)
: EntWsp()
{
  _precondition_(_MAGIC_,CN_ENTWSP);

  vForm(xId,xKeyWsp);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWsp::~EntWsp(void)
{
  _precondition_(_MAGIC_,CN_ENTWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void EntWsp::vAddKey(KeyWsp& xKeyWsp)
{
  Str* aKeySrc = xKeyWsp.aWspStr();
  i8 nKeyBytes = xKeyWsp.nWspUsed();
  
  m_xKeyWsp.vAppend(aKeySrc,nKeyBytes);
}

/*----------------------------------------------------------------------------*/
/* form a simple entry using given id and key                                 */
/*----------------------------------------------------------------------------*/
void EntWsp::vForm(Id& xId, KeyWsp& xKeyWsp)
{
  m_xId = xId;
  vAddKey(xKeyWsp);
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* EntWsp::aKey(void)
{
  Key* aKey = &m_xKey;
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWsp::nKeySize(void)
{
  return m_xKeyWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWsp::nOffsets(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= C_FLAT_OFFSETS);
  
  return m_iOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset at level                                                  */
/*----------------------------------------------------------------------------*/
i8 EntWsp::iRightOffset(on nOffsetNo)
{
  ASSERT(nOffsetNo >= 0_on && nOffsetNo <= C_FLAT_OFFSETS);
  
  return m_iOffset[*nOffsetNo];
}

/*----------------------------------------------------------------------------*/
/* set right offset at level                                                  */
/*----------------------------------------------------------------------------*/
void EntWsp::vRightOffset(on nOffsetNo, u8 uRightOffset)
{
  ASSERT(nOffsetNo >= 0_on && nOffsetNo <= C_FLAT_OFFSETS);
  
  m_iOffset[*nOffsetNo] = uRightOffset;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntWsp::xId(void)
{
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWsp::vId(Id& xId)
{
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWsp::vShow(void)
{
  i8 iKey;

  ValLib::vCopyIntDatum(&iKey,aKey());
  
  vTrace("EntWsp %p: Id %lli.%lli Offsets: %llu "_Str,
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
RightVec* EntWsp::aRightVec(void)
{
  RightVec* aRightVec = &m_xRight;
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/* get size of RightVec                                                       */
/*----------------------------------------------------------------------------*/
i8 EntWsp::nRightSize(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= C_FLAT_OFFSETS);
 
  i8 nBytes = RightVec::nBytesRequired(m_iOffsets);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(EntWsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
