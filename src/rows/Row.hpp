/*============================================================================*/
/* Row.hpp -- Computable form of Row                                        */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Row : public Object
  {
  public:
    Dtm m_waColVal[1];
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row(void);
    ~Row(void) noexcept(false);
  
    static i8 nSizeBytes(StreamDef* pStreamDef);
    void vDump(StreamDef* pStreamDef);
  
    Stz* szStzGet_row(i8 i_Col);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
