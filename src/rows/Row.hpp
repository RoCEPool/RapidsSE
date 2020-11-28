/*============================================================================*/
/* Row.hpp -- Computable form of Row                                        */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Row : public Object
  {
  public:
  //private:
    Dtm m_waColVal[1];
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row(void);
    ~Row(void) noexcept(false);
  
    static i8 nSizeBytes(StreamDef* pStreamDef);
    void vDump(StreamDef* pStreamDef);
  
    Dtm* aColDtm(i8 i_Col);
    char* azColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    d8   dColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);
    Dtm  wColVal(i8 i_Col);

    bool bIsColNull(i8 i_Col);

    Stz* szStzGet_row(i8 i_Col);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
