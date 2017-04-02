/*============================================================================*/
/* EntWsp.hpp -- entry workspace (skip list variant)                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp : public Object
  {
  private:
    i8     m_nBytes;
    RowId  m_xRowId;   // row id
    i8     m_nLevels;
    union {
      u8       m_uRight[C_FLAT_LEVELS];
      RightVec m_xRight;
    };
    union {
      WspFxd<C_1KB*15> m_xWsp;     // workspace
      Key              m_xKey;
    };
    Footer m_xFooter;  // footer
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWsp*);
    
    EntWsp(void);
    ~EntWsp(void);
  
    void   vAddKey(KeyWsp& xKeyWsp);
    i8     nEntryBytesRequired(void);
    Key*   aKey(void);
    i8     nKeySize(void);
    u8     nLevels(void);
    u8     nRight(u8 iLevel);
    void   vRight(u8 iLevel, u8 uRight);
    RowId& xRowId(void);
    void   vRowId(RowId& xRowId);
    void   vShow(void);
    i8     nSize(void);
    char*  aWspArea(void);
    Str*   aWspStr(void);
    i8     nWspUsed(void);
    RightVec* aRightVec(void);
    i8        nRightSize(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
