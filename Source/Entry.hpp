/*============================================================================*/
/* Entry.hpp -- buffer entry                                                  */
/*============================================================================*/
#pragma once

#pragma pack(push)
#pragma pack(1)

namespace Data
{
  class Entry : public Item
  {
  protected:

  public:
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
    // data area
    Row*  aRow(void);
    Entry* aRefdRowEntry(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key
    Key* aKey(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // index
  
    void vSetEntry(Id xId);
    void vSetEntry(KeyWrk& xKeyWrk);
    void vSetEntry(RowWrk& xRowWrk);
  
    void vShow(i8 iBase, Index* pIndex, bool bShowKey);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // row
    void vContentDump(void);
    void vExtractIndexKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    void vExtractSortKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec);
    char* pExtractSortKey(char *aElement, IdxColVec* pIdxColVec);
    };
  
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
    Key* aKey = reinterpret_cast<Key*>(aDataArea());
    
    return aKey;
  }

}
#pragma pack(pop)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
