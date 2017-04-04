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
  m_nLevels = Random::uLogLevel(C_FLAT_LEVELS,2);
  ASSERT(m_nLevels >= 0 && m_nLevels < C_FLAT_LEVELS);

  // set all right offsets to nil
  for (i8 iRight = 0; iRight < C_FLAT_LEVELS - 1; iRight++)
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
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void EntWsp::vAddKey(Key* pKey)
{
  Str* aKeySrc = pKey->aWspStr();
  i8 nKeyBytes = pKey->nSize();
  
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
u8 EntWsp::nLevels(void)
{
  ASSERT(m_nLevels >= 0 && m_nLevels < C_FLAT_LEVELS);
  
  return m_nLevels;
}

/*----------------------------------------------------------------------------*/
/* get right offset at level                                                  */
/*----------------------------------------------------------------------------*/
u8 EntWsp::uRightOffset(u8 iLevel)
{
  ASSERT(iLevel > 0 && iLevel < C_FLAT_LEVELS);
 
  i8 iRight = iLevel - 1;
  
  return m_uOffset[iRight];
}

/*----------------------------------------------------------------------------*/
/* set right offset at level                                                  */
/*----------------------------------------------------------------------------*/
void EntWsp::vRightOffset(u8 iLevel, u8 uRight)
{
  ASSERT(iLevel > 0 && iLevel < C_FLAT_LEVELS);
  
  i8 iRight = iLevel - 1;
  
  m_uOffset[iRight] = uRight;
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
  
  vTrace("EntWsp %p: Row %llu.%llu Levels: %llu "_Str,
         this,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         m_nLevels
         );
  
  for (i8 iRight = 0; iRight < m_nLevels - 1; iRight++)
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
  ASSERT(m_nLevels >= 0 && m_nLevels < C_FLAT_LEVELS);
  ASSERT(m_nLevels >= 0 && m_nLevels < C_FLAT_LEVELS);

  i8 nBytes = RightVec::nBytesRequired(m_nLevels);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(EntWsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
