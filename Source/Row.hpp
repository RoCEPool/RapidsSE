/*============================================================================*/
/* Row.hpp -- Computable form of Row                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  class Row : public Object
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
    i8         m_iBaseOffset;
    bool       m_bGetAll;
    bool       m_abGetCol[C_MAX_COLUMNS];
    i8         m_iLastCol;
    
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
  
    void vDontDecodeVch(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
