/*============================================================================*/
/* RowWrk.hpp -- Row Working Entry                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class RowWrk : public EntWrk
  {
  
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowWrk*);
    
    RowWrk(void);
    ~RowWrk(void);
    
    Row*   aRow(void);
    i8     nRowSize(void);
    void   vShow(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
