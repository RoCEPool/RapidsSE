/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWrk;
  class Index;
  
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
    i8  m_iIntKey;    // key debug hack
  
    i8  m_iSeeCount0;
    i8  m_iSeeCount1;
    i8  m_iSeeCount2;
    // ..data..       // variable data area

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
  
    Entry(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~Entry(void);
  
    static i8 nBytesRequired(EntWrk& xEntWrk);
    static i8 nBytesRequired(i8 nOffsets, i8 nKeyBytes, i8 nDataBytes);
    static i8 nBytesRequired(i8 nOffsets, i8 nDataBytes);
    static i8 nBytesRequired(i8 nDataBytes);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry proper
    Id&  xId(void);
    void vId(Id xId);
    i8   iRow(void);
    EntryBuffer* pEntryBuffer(void);
    i8   iBufferNo(void);
    i8   nSizeEntry(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // data area
    Row*  aRow(void);
    char* aRowData(void);
    i8    nRowSize(void);
  
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
    void vReplaceKey(EntWrk& xEntWrk);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // index
    void vSetEntry(Id xId, i8 iKey, Entry* pOldEntry);
    void vSetEntry(i8 xTxId, Id xId, i8 nOffsets, Stz* aData, i8 nDataSize);
    void vShow(i8 iBase, Index* pIndex, bool bShowKey);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // row
    void vContentDump(void);
    void vExtractIndexKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    void vExtractSortKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    char* pExtractSortKey(char *aElement, IdxColVec* pIdxColVec);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
  
    NEO* aNEO(void);
    i4   iNEO(i8 i_Offset);
    void vNEO(i8 i_Offset, i4 iNEO);
    void vNEOdump(void);
    i8   iNEOsize(void);
  
    char* aAddressAfter(void);
  
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
