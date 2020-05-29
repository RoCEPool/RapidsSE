/*============================================================================*/
/* ColumnVec.hpp - Column Vector                                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  class ColumnVec : public Object
  {
  static const i8 C_COLUMN_VEC = 1024; // max number of column

  private:
    i8      m_nColumns;  // number of actual columns
    Column* m_pColumn[C_COLUMN_VEC]; // vector of columns
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    ColumnVec(void);
    ~ColumnVec(void);
  
    void    vAddColumn(Column* pColumn);
    i8      nColumns(void);
    Column* pColumn(i8 iColumn);
  
    Footer* pFooter(void);
  };
  
  ColumnVec* _pColumnVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
