/*============================================================================*/
/* ResultRow.hpp -- result row                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ResultRow
  {
  protected:
    i8  m_nResultRowSize; // size of this result row
    // ... data
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, ResultRow*);
  
  //ResultRow(i8 nSizeRow);
    ResultRow(Row* aSrcRow, i8 nResultRowSize, i8 nRowSize);
    ~ResultRow(void);
  
    static i8 nBytesRequired(i8 nRowSize);
  
    Row* aRow(void);
    void vShow(ColumnVec* pColVec);
    i8 nResultRowSize(void);
  };
  
  ResultRow* _pResultRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
