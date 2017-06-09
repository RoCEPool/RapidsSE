/*============================================================================*/
/* BufferItem.hpp -- abstract buffer item                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp;
  
  class BufferItem : public Object
  {
  protected:
    i8  m_nThisBytes; // size of this item
    Id  m_xId;        // id
    i8  m_iOffsets;   // no of offsets for this entry (0,1 or 2)
  //i8  m_iTxId;      // transaction id

  public:
    void* operator new(size_t);
    void* operator new(size_t, BufferItem*);
  
    BufferItem(eCN eClassNo);
    BufferItem(eCN eClassNo, i8 iDataBytes);
    ~BufferItem(void);
  
    static i8 nBytesRequiredEntry(EntWsp& xEntWsp);
    static i8 nBytesRequiredEntry(i8 nOffsets, i8 nKeyBytes);
  
    static i8 nBytesRequiredRow(i8 iStrLength);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
    on     nHighestOffset(void);
    i8     nOffsets(void);
    i8     iRightOffset(on nOffsetNo);
    void   vRightOffset(on nOffsetNo, i8 iRightOffset);
    RightVec* aRightVec(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry
    EntryData* aEntryData(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // row
    void  vContentDump(void);
    char* aRowData(void);
    void  vExtractKey(KeyWsp& xKeyWsp, IdxColVec* pColVec);
    i8    nSizeRow(void);
    i8    iTxId(void);
    void  vTxId(i8 iTxId);
  
    Footer* pFooter(void);
  };
  
  BufferItem* _pBufferItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
