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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space and has free item no's
  
  i8 nRowEntrySize = xRowWrk.nRowEntrySize();
  if (! bHasSpace(nRowEntrySize))
    return nullptr;

  if (! bHasFreeItem()) {
    vTrace("No free row entries\n"_Stz);
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot row entry in this buffer

  bn nBufferNo = this->nPoolBufferNo();
  xRowWrk.vBufferNo(*nBufferNo);
  
  Item* aRowEntry = aAllotRow(xRowWrk);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  i8 i_RowNo = xRowWrk.iItemNo();
  xRowWrk.vSetRowEntry(aRowEntry);
  RASSERT(i_RowNo == xRowWrk.xId().iItem());
  RASSERT(aRowEntry->bValidItem());
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return aRowEntry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
