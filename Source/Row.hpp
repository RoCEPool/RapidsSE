/*============================================================================*/
/* Row.hpp -- Computable form of Row                                          */
/*============================================================================*/
#pragma once

namespace Data { class Datum; } //! TEMP

namespace Transform
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
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Row(ColumnVec* pColVec);
    ~Row(void) noexcept(false);
  
    void vAppendValue(Data::Datum* pDatum, Column* pCol);
    void vDontDecodeVch(void);
    void vResetValues(void);
    i8   nSizeBytes(void);

  private:
    Footer* aFooter(void);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
