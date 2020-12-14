/*============================================================================*/
/* Row.hpp -- Computable form of Row                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Row : public Object
  {
  friend class RowSingle;
  friend class InfoData::InfoSetBase;
  friend class Steps::Step;
  friend class DataBuffers::RowBuffer;

  private:
    Dtm m_wxaColumnCell[1];
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row(void);
    ~Row(void) noexcept(false);
  
    static i8 nSizeBytes(StreamDef* pStreamDef);
    static i8 nRowProper(RowPix* pRowPix);
  
    void vDump(StreamDef* pStreamDef);
  
    m8*  aCellArray(void);
  
    m8*  aColCell(i8 i_Col);
  
    Dtm* aColDtm(i8 i_Col);
    Dtm  wColDtm(i8 i_Col);
  
    i8   iColVal_int(i8 i_Col);
    d8   dColVal_flt(i8 i_Col);
    Stz* szColVal_ofs(i8 i_Col);

    bool bIsColNull(i8 i_Col);
  
    void vColVal_int(i8 i_Col, i8 iVal);
    void vColVal_flt(i8 i_Col, d8 dVal);
    void vColVal_ofs(i8 i_Col, i8 iOfs);
    void vColVal_ofs(i8 i_Col, Stz* szStz);

    void vSetColNull(i8 i_Col);
  

  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
