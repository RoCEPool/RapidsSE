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
  ASSERT(0 == m_xRowId.uBuffer() && 0 == m_xRowId.uRow());
  
  // generate logarithmic level for this entry
  m_nOffsets = Random::uLogLevel(C_FLAT_OFFSETS+1, C_FLAT_FREQ);
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= C_FLAT_OFFSETS);
  
  // set all right offsets to nil
  for (i8 iRight = 0; iRight < C_FLAT_OFFSETS - 1; iRight++)
    m_uOffset[iRight] = C_NIL_OFFSET;
  
  // m_xKeyWsp
  m_xKeyWsp.vInit();
  
  _checkInitialisedSize_(sizeof(EntWsp));
  
  _postcondition_(_MAGIC_,CN_ENTWSP,_FOOTER_);
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
u8 EntWsp::nOffsets(void)
{
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= C_FLAT_OFFSETS);
  
  return m_nOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset at level                                                  */
/*----------------------------------------------------------------------------*/
u8 EntWsp::uRightOffset(u8 iOffset)
{
  ASSERT(iOffset >= 0 && iOffset <= C_FLAT_OFFSETS);
  
  return m_uOffset[iOffset];
}

/*----------------------------------------------------------------------------*/
/* set right offset at level                                                  */
/*----------------------------------------------------------------------------*/
void EntWsp::vRightOffset(u8 iOffset, u8 uRight)
{
  ASSERT(iOffset >= 0 && iOffset <= C_FLAT_OFFSETS);
  
  m_uOffset[iOffset] = uRight;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
RowId& EntWsp::xRowId(void)
{
  return m_xRowId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWsp::vRowId(RowId& xRowId)
{
  m_xRowId = xRowId;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWsp::vShow(void)
{
  i8 iKey;

  ValLib::vCopyIntDatum(&iKey,aKey());
  
  vTrace("EntWsp %p: Row %llu.%llu Offsets: %llu "_Str,
         this,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         m_nOffsets
         );
  
  for (i8 iRight = 0; iRight < m_nOffsets - 1; iRight++)
    vTrace("R%lld: %lld "_Str,
           iRight,
           uRightOffset(iRight)
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
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= C_FLAT_OFFSETS);
 
  i8 nBytes = RightVec::nBytesRequired(m_nOffsets);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(EntWsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
