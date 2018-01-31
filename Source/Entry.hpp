/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWrk;
  
  class Entry : public Item
  {
  protected:
    Id  m_xId;        // id
    i8  m_iTxId;      // transaction id
    i8  m_nOffsets;   // no of offsets for this entry (0,1 or 2)
    // ..data..       // variable data area

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
  
    Entry(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~Entry(void);
  
    static i8 nBytesRequired(EntWrk& xEntWrk);
    static i8 nBytesRequired(EntWrk_v2& xEntWrk);
    static i8 nBytesRequired(i8 nOffsets, i8 nKeyBytes, i8 nDataBytes);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry proper
    Id&  xId(void);
    void vId(Id xId);
    i8   iRow(void);
    EntryBuffer* pEntryBuffer(void);
    i8   nSizeEntry(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // data area
    Row*  aRow(void);
    Row*  aRow_v2(void);
    char* aRowData(void);
    i8    nRowSize(void);
    i8    nRowSize_v2(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // variable data area
    char* aVblData(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
    on   nHighestOffset(void);
    i8   nOffsets(void);
    void vOffsets(i8 nOffsets);
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
    void vSetEntry(EntWrk_v2& xEntWrk);
    void vSetEntry(Id xId, i8 iKey, Entry* pOldEntry);
    void vShow(i8 iBase, Index* pIndex, bool bShowKey);
    void vShow_v2(i8 iBase, Index* pIndex, bool bShowKey);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // row
    void vContentDump(void);
    void vExtractIndexKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    void vExtractIndexKey_v2(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    void vExtractSortKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    char* pExtractSortKey(char *aElement, IdxColVec* pIdxColVec);
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
