/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/
#pragma once

namespace Data { class Datum; } //! TEMP

namespace Transform
{
  class RowEncoding : public Object
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
  
    i8         m_iBaseOffset;
    bool       m_bGetAll;
    bool       m_abGetCol[C_MAX_COLUMNS];
    i8         m_iLastCol;

    // - - - - - - - - - - - - - - - - - -
  
    i8  m_iHash; // hash of partition column values
    Dtm m_waColVal[C_MAX_COLUMNS]; // column value array
    Wsp_t<C_MAX_RECORD> m_xWsp; // sz workspace

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowEncoding(ColumnVec* pColVec);
    ~RowEncoding(void) noexcept(false);
  
    void vAppendValue(Data::Datum* pDatum, Column* pCol);
    void vDontDecodeVch(void);
    i8   iHashColumn(Column* pCol);
    void vResetValues(void);
    i8   nSizeBytes(void);

  private:
    Footer* aFooter(void);
  };
  
  RowEncoding* _pRowEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
