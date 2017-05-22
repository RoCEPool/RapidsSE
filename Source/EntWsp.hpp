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
      on       m_nOffset[C_FLAT_OFFSETS];    // right offsets
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
    EntWsp(Id& xId, KeyWsp& xKeyWsp);
    ~EntWsp(void);
  
    void   vAddKey(KeyWsp& xKeyWsp);
    void   vForm(Id& xId, KeyWsp& xKeyWsp);
    Key*   aKey(void);
    i8     nKeySize(void);
    u8     nOffsets(void);
    on     nRightOffset(u8 uOffset);
    void   vRightOffset(u8 uOffset, on nRight);
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
