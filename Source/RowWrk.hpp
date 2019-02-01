/*============================================================================*/
/* RowWrk.hpp -- Row Working Entry                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class Entry;
  
  class RowWrk : public EntWrk
  {
  
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowWrk*);
    
    RowWrk(void);
    ~RowWrk(void) noexcept(false);
  
    Row* aRow(void);
    i8   nRowSize(void);
    void vRowSize(i8 nRowSize);
    void vSetEntry(Entry* pEntry);
    void vShow(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
