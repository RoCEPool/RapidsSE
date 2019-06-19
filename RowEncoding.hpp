/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowEncoding : public Object
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
  
    // - - - - - - - - - - - - - - - - - -
  
    Dtm m_waColVal[C_MAX_COLUMNS]; // column value array
    Wsp_t<C_MAX_RECORD> m_xWsp;    // sz workspace
  
    struct {
      eMC m_eEncoding;
      i8  m_nEncodedBytes;
    } xEncode[C_MAX_COLUMNS];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowEncoding(ColumnVec* pColVec);
    ~RowEncoding(void) noexcept(false);
  
    void vEncode(Row* pRow);
    void vReset(void);

  private:
    Footer* aFooter(void);
  };
  
  RowEncoding* _pRowEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
