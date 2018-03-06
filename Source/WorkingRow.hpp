/*============================================================================*/
/* WorkingRow -- working row                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class WorkingRow
  {
  protected:
    i8  m_nWorkingRowSize; // size of this working row
    // ... data
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, WorkingRow*);
  
    WorkingRow(Row* aSrcRow, i8 nWorkingRowSize, i8 nRowSize);
    ~WorkingRow(void);
  
    static i8 nBytesRequired(i8 nRowSize);
  
    Row* aRow(void);
    void vShow(ColumnVec* pColVec);
    i8   nRowSize(void);
  };
  
  WorkingRow* _pWorkingRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
