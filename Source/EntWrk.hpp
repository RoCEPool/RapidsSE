/*============================================================================*/
/* EntWrk.hpp -- Entry Working Object (skip list variant)                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class EntWrk : public Object
  {
  static const i8 C_KEY_BYTES  = C_1KB*15;
  static const i8 C_DATA_BYTES = C_1KB*15;
  
  private:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // entry
  
    Id m_xId;   // id
    i8 m_iTxId; // transaction id
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // offsets
  
    i8 m_iOffsets; // number of offsets (0, 1 or 2)
    union {
      RightVec m_xRight; // right vector
      i8 m_iOffset[C_FLAT_OFFSETS_MAX]; // right offsets
                                        // no offset for level 0
    };
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key
  
    union {
      Key m_xKey; // this key overlays into m_xKeyWrk
      WspFxd<C_KEY_BYTES> m_xKeyWrk; // key workspace
    };
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // datums
  
    union {
      i8 m_iInt; // this int overlays into m_xDataWrk
      char* m_aStr; // this str overlays into m_xDataWrk
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
    void   vForm(Id& xId, KeyWrk& xKeyWrk);
    Key*   aKey(void);
    i8     nKeySize(void);
    i8     nOffsets(void);
    i8     iRightOffset(on nOffsetNo);
    void   vRightOffset(on nOffsetNo, u8 uRightOffset);
    Id&    xId(void);
    void   vId(Id& xId);
    void   vShow(void);
    RightVec* aRightVec(void);
    i8        nRightSize(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
