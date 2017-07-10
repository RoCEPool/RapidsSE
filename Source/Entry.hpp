/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWrk;
  
  class Entry : public Object
  {
  protected:
    i8  m_nThisBytes; // size of this item
    Id  m_xId;        // id
    i8  m_nOffsets;   // no of offsets for this entry (0,1 or 2)
    i8  m_iTxId;      // transaction id
    // ..data..       // variable data area 

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
  
    Entry(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~Entry(void);
  
    static i8 nBytesRequired(EntWrk& xEntWrk);
    static i8 nBytesRequired(i8 nOffsets, i8 nKeyBytes, i8 nDataBytes);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry proper
    Id&  xId(void);
    void vId(Id xId);
    EntryBuffer* pIndexBuffer(void);
    i8   nSizeEntry(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // data area
    char* aRow(void);
    i8    nRowSize(void);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // variable data area
    char* aVblData(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
    on   nHighestOffset(void);
    i8   nOffsets(void);
    i8   iRightOffset(on nOffsetNo);
    void vRightOffset(on nOffsetNo, i8 iRightOffset);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key
    Key* aKey(void);
    i8   nKeySize(IdxColVec* pIdxColVec);
    void vReplaceKey(EntWrk& xEntWrk);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // index
    void vSetEntry(EntWrk& xEntWrk);
    void vSetEntry(Id xId, i8 iKey, s256& sRow);
    void vShow(i8 iBase, Index* pIndex);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // row
    void vContentDump(void);
    void vExtractKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
  
    Footer* aFooter(void);
  
  private:
    bn     nBufferNo(void);
    RightVec* aRightVec(void);
  };
  
  Entry* _pEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
