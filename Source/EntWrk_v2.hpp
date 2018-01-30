/*============================================================================*/
/* EntWrk.hpp -- Working Entry                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class EntWrk_v2 : public Object
  {
  static const i8 C_DATA_BYTES = C_1KB*15;
  
  private:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry
  
    Id m_xId;   // id
    i8 m_iTxId; // transaction id
    bool m_bHasData;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
  
    i8 m_iOffsets; // number of offsets (0, 1 or 2)
    union {
      RightVec m_xRight; // right vector
      i8 m_iOffset[C_FLAT_OFFSETS_MAX]; // right offsets
                                        // no offset for level 0
    };
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // datums
    WspFxd<C_DATA_BYTES> m_xDataWrk; // datum workspace
  
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWrk*);
    
    EntWrk_v2(void);
    EntWrk_v2(i8 nOffsets);
    EntWrk_v2(Entry* pEntry);
    EntWrk_v2(Id& xId, i8 iTxId, KeyWrk& xKeyWrk);
    ~EntWrk_v2(void);
  
    void   vAddKey(KeyWrk& xKeyWrk);
    char*  aData(void);
    char*  aData_v2(void);
    i8     nDataSize(void);
    void   vDataSize(i8 iSize);
    void   vData(Stz* aData, i8 iSize);
    void   vForm(Id& xId, KeyWrk& xKeyWrk);
    bool   bHasData(void);
    void   vHasData(bool bHasData);
    Id&    xId(void);
    void   vId(Id& xId);
    Key*   aKey(void);
    i8     nKeySize(void);
    i8     nOffsets(void);
    void   vOffsets(i8 nOffsets);
    i8     iRightOffset(on nOffsetNo);
    void   vRightOffset(on nOffsetNo, u8 uRightOffset);
    RightVec* aRightVec(void);
    i8        nRightSize(void);
    Row*   aRow(void);
    void   vShow(ColumnVec* pColVec);
    i8     iTxId(void);
    void   vTxId(i8 iTxId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
