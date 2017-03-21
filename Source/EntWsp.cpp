/*============================================================================*/
/* EntWsp.cpp -- Entry Workspace                                              */
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

  //m_xRowId
  
  // generate logarithemic levels for this entry
  m_nLevels = Random::uLogLevel(IndexBuffer::C_FLAT_LEVELS, 2) + 1;
  
  if (m_nLevels >= 1)
    int i = 1;
  
  // allocate space for the level offsets and initialize with C_NIL
  i8 iLevelOffset = IndexBuffer::C_NIL;
  for (i8 iIndex=0; iIndex < m_nLevels - 1; iIndex++)
    m_xWsp.vAppend(&iLevelOffset,sizeof(i8));
  
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
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWsp::vShow(void)
{
  i8 iKey;
//  i8* pKeyData = reinterpret_cast<i8*>(m_mDataArea);
//  memcpy(&iKey,m_mDataArea,8);
  
  
  
  vTrace("EntWsp %p: Levels: %llu "_Str,
         this,
         m_nLevels
         );
  
  ValLib::vCopyIntDatum(&iKey,m_xWsp.aArea() + sizeof(i8)*(m_nLevels-1));
  vTrace("Row %llu.%llu key (size:%lld value:%lld) "_Str,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         m_xWsp.nUsed() - sizeof(i8)*(m_nLevels-1),
         iKey
         );
  
  for (i8 iIndex=0; iIndex < m_nLevels - 1; iIndex++)
    vTrace("L%lld: %lld "_Str,
           iIndex + 1,
           nOffset(iIndex)
           );
  
  vTrace("\n"_Str);
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
/* get nLevels                                                                */
/*----------------------------------------------------------------------------*/
u8 EntWsp::nLevels(void)
{
  return m_nLevels;
}

/*----------------------------------------------------------------------------*/
/* get next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
u8 EntWsp::nOffset(u8 nLevel)
{
  i8* nLevels = reinterpret_cast<i8*>(m_xWsp.aArea());
  return nLevels[nLevel];
}

/*----------------------------------------------------------------------------*/
/* set next entry offset at level                                             */
/*----------------------------------------------------------------------------*/
void EntWsp::vOffset(u8 nLevel, u8 nOffset)
{
  i8* nLevels = reinterpret_cast<i8*>(m_xWsp.aArea());
  nLevels[nLevel] = nOffset;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry instance                                   */
/*----------------------------------------------------------------------------*/
i8 EntWsp::nSize(void)
{
  //_precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  i8 nBytes = sizeof(RowId)
               + sizeof(i8)
               + sizeof(i8)
               + m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key* EntWsp::pKey(void)
{
  Key* pKey = reinterpret_cast<Key*>(m_xWsp.aArea() + sizeof(i8)*(m_nLevels -1));
  
  return pKey;
}

/*----------------------------------------------------------------------------*/
/* add a key                                                                  */
/*----------------------------------------------------------------------------*/
void EntWsp::vAddKey(KeyWsp& xKeyWsp)
{
  Str* aSrc = xKeyWsp.aWspStr();
  i8 nBytes  = xKeyWsp.nWspUsed();
  
  m_xWsp.vAppend(aSrc,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* EntWsp::aWspArea(void)
{
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get wsp area as Str                                                        */
/*----------------------------------------------------------------------------*/
Str* EntWsp::aWspStr(void)
{
  return m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/* get wsp bytes used                                                         */
/*----------------------------------------------------------------------------*/
i8 EntWsp::nWspUsed(void)
{
  return m_xWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(EntWsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
