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
  
    i8   nItemSize(void);
    RowData* aRowData(void);
    i8   nRowDataSize(void);
    void vRowDataSize(i8 nRowSize);
    void vSetRowEntry(Item* aRowEntry);
  
    void vShow(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
