/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class NEO // next entry offset
  {
  public:
    i4 m_iOffset[1];  // offset where the entry is
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
    static i8 nBytesRequired_v3(EntWrk_v2& xEntWrk);
    static i8 nBytesRequired(i8 nOffsets, i8 nKeyBytes, i8 nDataBytes);
    static i8 nBytesRequired_v3(i8 nOffsetBytes, i8 nKeyBytes, i8 nDataBytes);
  
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
    Key* aKey_v2(void);
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
    void vExtractIndexKey_v2(KeyWrk_v2& xKeyWrk, IdxColVec* pIdxColVec);
    void vExtractSortKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    char* pExtractSortKey(char *aElement, IdxColVec* pIdxColVec);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
  
    NEO* aNEO(void);
    i4   iNEO(i8 iOffset);
    void vNEO(i8 iOffset, i4 iNEO);

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
