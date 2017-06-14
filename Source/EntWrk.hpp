/*============================================================================*/
/* EntWrk.hpp -- Working Entry                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class EntWrk : public Object
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
  
    union {
      Key m_xKey; // this key overlays into m_xDataWrk
      WspFxd<C_DATA_BYTES> m_xDataWrk; // datum workspace
    };
  
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWrk*);
    
    EntWrk(void);
    EntWrk(i8 nOffsets);
    EntWrk(Entry* pEntry);
    EntWrk(Id& xId, KeyWrk& xKeyWrk);
    ~EntWrk(void);
  
    void   vAddKey(KeyWrk& xKeyWrk);
    char*  aData(void);
    i8     nDataSize(void);
    void   vData(Str* aData, i8 iSize);
    void   vForm(Id& xId, KeyWrk& xKeyWrk);
    bool   bHasData(void);
    Id&    xId(void);
    void   vId(Id& xId);
    Key*   aKey(void);
    i8     nKeySize(void);
    i8     nOffsets(void);
    i8     iRightOffset(on nOffsetNo);
    void   vRightOffset(on nOffsetNo, u8 uRightOffset);
    RightVec* aRightVec(void);
    i8        nRightSize(void);
    void   vShow(IdxColVec* pIdxColVec);
    i8     iTxId(void);
    void   vTxId(i8 iTxId);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
