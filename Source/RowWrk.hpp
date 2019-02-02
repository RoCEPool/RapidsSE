/*============================================================================*/
/* RowWrk.hpp -- Row Working Item                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowWrk : public EntWrk
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowWrk*);
    
    RowWrk(void);
    ~RowWrk(void) noexcept(false);
  
    RowData* aRowData(void);
    i8   nRowSize(void);
    void vRowSize(i8 nRowSize);
    void vSetEntry(Item* pEntry);
    void vShow(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
