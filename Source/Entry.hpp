/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

#pragma pack(push)
#pragma pack(1)


//#define USE_LW_FOOTER

#ifdef USE_LW_FOOTER
  #define LW_FOOTER_CHECK() aFooter()->vAssertFooter();
#else
  #define LW_FOOTER_CHECK()
#endif

namespace DBobjects
{
  class EntryBuffer;
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
  //i8  m_iTxId;      // transaction id
  //char  m_nOffsets;   // no of offsets for this entry (0..3)
#ifdef WITH_DEBUG_VERSION
    i8  m_iIntKey;    // key debug hack
#endif
  
  public:
#ifdef WITH_DEBUG_VERSION
    i8  m_iEntryNo;   // verify of entry order
#endif
    // ..embedded object..

  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
  
    Entry(eCN eClassNo, i8 nThisBytes, i8 uOffsets);
    ~Entry(void) noexcept(false);
  
    void vAssertEntry(void);
    bool bValidEntry(void);
  
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
    // data entry
    Entry* aDataEntry(void);
    
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
  
  //Entry* _pEntry(void* aAddr);
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Entry* _aEntry(void* aAddr)
  {
    Entry* aEntry = reinterpret_cast<Entry*>(aAddr);
    
    //RASSERT_TYPE(aEntry,CN_ENTRY);
    
    return aEntry;
  }
  
  /*--------------------------------------------------------------------------*/
  /* get Key                                                                  */
  /*--------------------------------------------------------------------------*/
  inline Key* Entry::aKey(void)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY); LW_FOOTER_CHECK();
    
    Key* aKey = reinterpret_cast<Key*>(aEmbeddedObject());
    
    return aKey;
  }

}
#pragma pack(pop)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
