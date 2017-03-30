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
    
    u8     nOffset(u8 nLevel);
    void   vOffset(u8 nLevel, u8 nOffset);
    
    void vAddKey(KeyWsp& xKeyWsp);
    Key* pKey(void);
    i8   nSize(void);
  
    char* aWspArea(void);
    Str*  aWspStr(void);
    i8    nWspUsed(void);
  
    EntryData* aEntryData(void);
    i8         nEntryDataSize(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
