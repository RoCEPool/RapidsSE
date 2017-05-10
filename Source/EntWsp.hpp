/*============================================================================*/
/* EntWsp.hpp -- entry workspace (skip list variant)                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp : public Object
  {
  private:
    Id     m_xId;       // id
    i8     m_nOffsets;  // number of offsets (0, 1 or 2)
    union {
      RightVec m_xRight; // right vector
      u8       m_uOffset[C_FLAT_OFFSETS];    // right offsets
                                             // no offset for level 0
    };
    union {
      Key              m_xKey;    // key
      WspFxd<C_1KB*15> m_xKeyWsp; // key workspace
    };
    Footer m_xFooter;  // footer
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWsp*);
    
    EntWsp(void);
    EntWsp(i8 nOffsets);
    ~EntWsp(void);
  
    void   vAddKey(KeyWsp& xKeyWsp);
    void   vAddKey(Key* pKey);
    Key*   aKey(void);
    i8     nKeySize(void);
    u8     nOffsets(void);
    u8     uRightOffset(u8 iOffset);
    void   vRightOffset(u8 iOffset, u8 uRight);
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
