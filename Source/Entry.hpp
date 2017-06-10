/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp;
  
  class Entry : public Object
  {
  protected:
    i8  m_nThisBytes; // size of this item
    Id  m_xId;        // id
    i8  m_iOffsets;   // no of offsets for this entry (0,1 or 2)
  //i8  m_iTxId;      // transaction id
    // ..data..       // variable data area 

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    Entry(EntWsp& xEntWsp);
    Entry(i8 nBytes, i8 uOffsets);
    Entry(eCN eClassNo);
    Entry(eCN eClassNo, i8 iDataBytes);
    ~Entry(void);
  
    static i8 nBytesRequiredEntry(EntWsp& xEntWsp);
    static i8 nBytesRequiredEntry(i8 nOffsets, i8 nKeyBytes);
  
    static i8 nBytesRequiredRow(i8 iStrLength);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry proper
    Id&    xId(void);
    void   vId(Id xId);
    EntryBuffer* pIndexBuffer(void);
    void   vSetEntry(EntWsp& xEntWsp);
    i8     nSizeEntry(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // variable data area
    char* aVblData(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
    on     nHighestOffset(void);
    i8     nOffsets(void);
    i8     iRightOffset(on nOffsetNo);
    void   vRightOffset(on nOffsetNo, i8 iRightOffset);
    RightVec* aRightVec(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key
    Key*   aKey(void);
    void   vShow(i8 iOffset, IdxColVec* pColVec);
    void   vKey(EntWsp& xEntWsp);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // row
    void  vContentDump(void);
    void  vExtractKey(KeyWsp& xKeyWsp, IdxColVec* pColVec);
    i8    iTxId(void);
    void  vTxId(i8 iTxId);
  
    Footer* pFooter(void);
  
  private:
    bn     nBufferNo(void);
  };
  
  Entry* _pBufferItem(void* aAddr);
  Entry* _pEntry(void* aAddr);
  Entry* _aRow(void* aAddr);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
