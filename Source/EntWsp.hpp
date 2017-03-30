/*============================================================================*/
/* EntWsp.hpp -- entry workspace                                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp : public Object
  {
  private:
    i8               m_nBytes;
    RowId            m_xRowId;   // row id
    i8               m_nLevels;
    u8               m_uRight[C_FLAT_LEVELS];
    WspFxd<C_1KB*15> m_xWsp;     // workspace
    Footer           m_xFooter;  // footer
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWsp*);
    
    EntWsp(void);
    ~EntWsp(void);
  
    void vShow(void);
  
    RowId& xRowId(void);
    void vRowId(RowId& xRowId);
  
    u8     nLevels(void);
    
    u8     nRight(u8 iLevel);
    void   vRight(u8 iLevel, u8 uRight);
    
    void vAddKey(KeyWsp& xKeyWsp);
    i8   nSize(void);
  
    char* aWspArea(void);
    Str*  aWspStr(void);
    i8    nWspUsed(void);
  
    Key* aKey(void);
    i8   nKeySize(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
