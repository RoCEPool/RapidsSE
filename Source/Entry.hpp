/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

#pragma pack(push)
#pragma pack(1)

namespace DBobjects
{
  class EntryBuffer;
  class EntWrk;
  class Index;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Entry : public Item
  {
  protected:

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
  
  
    static i8 nBytesRequired(EntWrk& xEntWrk);
    static i8 nBytesRequired(i8 nEmbeddedBytes);
    static i8 nBytesRequired(i8 nEmbeddedBytes, i8 nOffsets);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry proper
    EntryBuffer* pReferencedEntryBuffer(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // data area
    Row*  aRow(void);
    char* aRowData(void);
    i8    nRowSize(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // data entry
    Entry* aDataEntry(void);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // embedded data area
    char* aEmbeddedData(void);
  
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
  
    Footer* aFooter(void);
  
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
    _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
    
    Key* aKey = reinterpret_cast<Key*>(aEmbeddedData());
    
    return aKey;
  }

}
#pragma pack(pop)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
