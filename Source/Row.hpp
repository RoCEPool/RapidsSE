/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/
#pragma once

namespace Data
{
  class Row : public Object
  {
  public:
    ColumnVec* m_pColVec;
  
    i8 m_iColumns;
    i8 m_iBaseOffset;
    i8 m_aiValue[C_MAX_COLUMNS];
  
    i8 m_i_Wsp;
    Wsp_t<C_MAX_RECORD> m_xWsp;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Row(ColumnVec* pColumnVec);
    ~Row(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
