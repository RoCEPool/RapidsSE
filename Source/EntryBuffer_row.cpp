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
  // allot row entry in this buffer

  bn nBufferNo = this->nPoolBufferNo();
  xRowWrk.vBufferNo(*nBufferNo);
  
  Item* aRowEntry = aAllotRowEntry(xRowWrk);
  if (! aRowEntry)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy row data into row entry
  
  xRowWrk.vCopyRowData(aRowEntry);
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //vDumpItems();
  
  RASSERT(aRowEntry->bValidItem());
  return aRowEntry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
