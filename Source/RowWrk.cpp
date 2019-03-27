/*============================================================================*/
/* RowWrk.hpp -- Row Working                                                  */
/*============================================================================*/

#include "Data.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowWrk::operator new(size_t)
{
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowWrk::operator new(size_t, RowWrk* pRowWrk)
{
  return pRowWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowWrk::RowWrk(void)
: EntWrk(CN_ROWWRK)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  vLevels(1);
  
  _checkInitialisedSize_(sizeof(RowWrk));
  
  _postcondition_(_MAGIC_,CN_ROWWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowWrk::~RowWrk(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ROWWRK,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy the row data from row work into this row entry                        */
/*----------------------------------------------------------------------------*/
void RowWrk::vCopyRowData(Item* aRowEntry)
{
  RowData* aSrcRow = aRowData();
  RowData* aDstRow = _aRowData(aRowEntry->aDataArea());
  i8 nRowSize  = this->nRowDataSize();
  
  MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_ENTRY);
}

/*----------------------------------------------------------------------------*/
/* copy the row data from row work into this row cacheline                    */
/*----------------------------------------------------------------------------*/
void RowWrk::vCopyRowData(CacheLine* aRow)
{
  RowData* aSrcRow = aRowData();
  RowData* aDstRow = _aRowData(aRow);
  i8 nRowSize  = this->nRowDataSize();
  
  MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_ENTRY);
}

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
RowData* RowWrk::aRowData(void)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  RowData* aThisRow = (RowData*)aEmbedded();
  return aThisRow;
}

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
i8 RowWrk::nRowDataSize(void)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  return nDataSize();
}

/*----------------------------------------------------------------------------*/
/* set row data size                                                          */
/*----------------------------------------------------------------------------*/
void RowWrk::vRowDataSize(i8 nRowDataSize)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  vDataSize(nRowDataSize);
}

/*----------------------------------------------------------------------------*/
/* get size of row entry                                                      */
/*----------------------------------------------------------------------------*/
i8 RowWrk::nRowEntrySize(void)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  RASSERT(nLevels() == 1);
  
  i8 nItemSize = nEntrySize() + nOffsetSize();
  
  return nItemSize;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void RowWrk::vShow(ColumnVec* pColVec)
{
  _precondition_(_MAGIC_,CN_ROWWRK);
  
  RowData* aThisRow = aRowData();
  aThisRow->vShow(pColVec);
  vTrace("\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
