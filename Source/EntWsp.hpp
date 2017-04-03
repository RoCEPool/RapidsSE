/*============================================================================*/
/* EntWsp.hpp -- entry workspace (skip list variant)                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp : public Object
  {
  private:
    RowId  m_xRowId;   // row id
    i8     m_nLevels;  // number of levels (0, 1 or 2)
    union {
      RightVec m_xRight; // right vector
      u8       m_uRight[C_FLAT_LEVELS - 1]; // right offsets
                                            // we don't hold offset for level 0
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
    ~EntWsp(void);
  
    void   vAddKey(KeyWsp& xKeyWsp);
    Key*   aKey(void);
    i8     nKeySize(void);
    u8     nLevels(void);
    u8     nRight(u8 iLevel);
    void   vRight(u8 iLevel, u8 uRight);
    RowId& xRowId(void);
    void   vRowId(RowId& xRowId);
    void   vShow(void);
    RightVec* aRightVec(void);
    i8        nRightSize(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
