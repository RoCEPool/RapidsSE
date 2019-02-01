/*============================================================================*/
/* EntryBuffer_row.cpp -- row related functions for Entry Buffer              */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*
 
    +-----+-----+
    | row | neo |
    +-----+-----+
 
 */


/*----------------------------------------------------------------------------*/
/* append a row entry into this buffer                                        */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::aAppendRow(RowWrk& xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(iLevelNo() == 0);
  
  Entry* aRowEntry;
  
  // rows only ever have one next offset
  xRowWrk.vOffsets(1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute required entry space for row and offset
  
  i8 nEntryBytes  = xRowWrk.nEntrySize();
  i8 nOffsetBytes = xRowWrk.nOffsetSize();
  i8 nEntryPlusOffsets = nEntryBytes+nOffsetBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  if (! bHasSpace(nEntryPlusOffsets)) {
    //if (nItems() > 7260)
    //vTrace("Row entries %lld\n"_Stz,nItems());
    return nullptr;
  }
  
  //  if (nItems() >= C_MAX_ITEMS - 16) {
  //    vTrace("Too many row entries %lld\n"_Stz,nItems());
  //    return nullptr;
  //  }
  
  RASSERT(nItems() < C_MAX_ITEMS - 16);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nPoolBufferNo();
  
  i8 i_RowNo = iGetItemNo(); //nItems();
  if (C_NO_MORE_ITEMS == i_RowNo) {
    vTrace("No free row entries\n"_Stz);
    return nullptr;
  }
  
  RASSERT(i_RowNo >=0 and i_RowNo < C_MAX_ITEMS);
  
  Id xRowId(*nBufferNo, i_RowNo);
  
  xRowWrk.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer for the row and offsets
  
  aRowEntry = aAllotEntry(nEntryBytes);
  ASSERT(aRowEntry);
  
  NEO* aNEO = aAllotNEO(nOffsetBytes);
  ASSERT(aNEO);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  aRowEntry->vSetEntry(xRowWrk);
  
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i4 iEntryOffset = iToOffset(aRowEntry);
  RASSERT(i_RowNo >= 0 && i_RowNo < C_MAX_ITEMS);
  m_iaItemOffsets[i_RowNo] = iEntryOffset;
  
  if (m_aLastEntry)
    m_aLastEntry->vNEO(0,iEntryOffset);
  aRowEntry->vNEO(0,-1);
  
  m_aLastEntry = aRowEntry;
  
  // set head entry offset
  if (m_aHeadEntry->iNEO(0) == -1)
    m_aHeadEntry->vNEO(0, iEntryOffset);
  
  RASSERT(aRowEntry->bValidItem());
  
  return aRowEntry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
