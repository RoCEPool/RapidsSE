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
    i8  m_nOffsets;   // no of offsets for this entry (0..3)
    i8  m_iIntKey;    // key debug hack
  
    // ..embedded object..

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
  
    Entry(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~Entry(void);
  
    void vAssertEntry(void);
  
    static i8 nBytesRequired(EntWrk& xEntWrk);
    static i8 nBytesRequired(i8 nEmbeddedBytes);
    static i8 nBytesRequired(i8 nEmbeddedBytes, i8 nOffsets);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry proper
    Id&  xId(void);
    void vId(Id xId);
    i8   iRow(void);
    EntryBuffer* pReferencedEntryBuffer(void);
    i8   iBufferNo(void);
    i8   nSizeEntry(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // data area
    Row*  aRow(void);
    char* aRowData(void);
    i8    nRowSize(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // embedded object area
    char* aEmbeddedObject(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
    i8   nOffsets(void);
    void vOffsets(i8 nOffsets);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key
    Key* aKey(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // index
  
    void vSetEntry(Id xId, i8 iKey, Entry* pOldEntry);
  
    void vSetEntry(i8 xTxId, Id xId, i8 nOffsets, Stz* aData, i8 nDataSize);
    void vSetEntry(KeyWrk& xKeyWrk);
    void vSetEntry(RowWrk& xRowWrk);
  
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
  };
  
  Entry* _pEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
