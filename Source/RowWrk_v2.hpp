/*============================================================================*/
/* RowWrk.hpp -- Row Working Entry                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class RowWrk_v2 : public EntWrk
  {
  
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowWrk_v2*);
    
    RowWrk_v2(void);
    RowWrk_v2(i8 nOffsets);
    RowWrk_v2(Entry* pEntry);
    ~RowWrk_v2(void);
    
    Row*   aRow(void);
    i8     nRowSize(void);
    void   vShow(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
