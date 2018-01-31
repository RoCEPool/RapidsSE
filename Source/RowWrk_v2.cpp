/*============================================================================*/
/* RowWrk.hpp -- Row Working Entry                                            */
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
void* RowWrk_v2::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowWrk_v2::operator new(size_t iSize, RowWrk_v2* pRowWrk)
{
  RASSERT(iSize);
  return pRowWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowWrk_v2::RowWrk_v2(void)
: EntWrk_v2(CN_ROWWRK)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  _checkInitialisedSize_(sizeof(RowWrk_v2));
  
  _postcondition_(_MAGIC_,CN_ROWWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowWrk_v2::RowWrk_v2(i8 nOffsets)
: EntWrk_v2(CN_ROWWRK, nOffsets)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  _checkInitialisedSize_(sizeof(RowWrk_v2));
  
  _postcondition_(_MAGIC_,CN_ROWWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowWrk_v2::RowWrk_v2(Entry* pEntry)
: EntWrk_v2(CN_ROWWRK, pEntry)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  _checkInitialisedSize_(sizeof(RowWrk_v2));
  
  _postcondition_(_MAGIC_,CN_ROWWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowWrk_v2::~RowWrk_v2(void)
{
  _precondition_(_MAGIC_,CN_ROWWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Row Entry Proper --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void RowWrk_v2::vShow(ColumnVec* pColVec)
{
//  i8 iKey;
//
//  Int::vCopy(&iKey,aKey());
//
//  vTrace("EntWrk %p: Id %lli.%lli Offsets: %llu "_Stz,
//         this,
//         m_xId.iBuffer(),
//         m_xId.iRow(),
//         m_nOffsets
//  );
//
//  vTrace("key (size:%lld value:%lld) "_Stz,
//         nKeySize(),
//         iKey
//         );
//
//  if (m_bHasData) {
//    Row* aThisRow = aRow();
//    aThisRow->vShow(pColVec);
//  }
//
//  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* RowWrk_v2::aRow(void)
{
  Row* aThisRow = (Row*)aData_v2();
  
  return aThisRow;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
