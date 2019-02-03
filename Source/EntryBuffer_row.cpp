/*============================================================================*/
/* EntryBuffer_row.cpp -- row related functions for Item Buffer              */
/*============================================================================*/

#include "Data.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*
 
    <---- row entry ---->
    +------+------+-----+
    | item | data | neo |
    +------+------+-----+
 
 */


/*----------------------------------------------------------------------------*/
/* append a row entry into this buffer                                        */
/*----------------------------------------------------------------------------*/
Item* EntryBuffer::aAppendRow(RowWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(iLevelNo() == 0);
  
  // rows only ever have one next offset
  RASSERT(1 == xRowWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  i8 nRowEntrySize = xRowWrk.nRowEntrySize();
  if (! bHasSpace(nRowEntrySize))
    return nullptr;
  
  RASSERT(nItems() < C_MAX_ITEMS - 16);

  if (! bHasFreeItem()) {
    vTrace("No free row entries\n"_Stz);
    return nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  i8 i_RowNo = iGetItemNo(); //nItems();
  RASSERT(C_NO_MORE_ITEMS != i_RowNo);
  RASSERT(i_RowNo >= 0 and i_RowNo < C_MAX_ITEMS);
  
  bn nBufferNo = this->nPoolBufferNo();
  Id xRowId(*nBufferNo, i_RowNo, 1);
  xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot row entry in this buffer
  
  Item* aRowEntry = aAllotRow(xRowWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  xRowWrk.vSetRowEntry(aRowEntry);
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set offset in itemoffsets[]
  
  ASSERT(i_RowNo == xRowWrk.xId().iItem());
  
  RASSERT(aRowEntry->bValidItem());
  return aRowEntry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
