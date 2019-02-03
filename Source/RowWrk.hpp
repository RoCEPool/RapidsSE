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
    i8   nRowDataSize(void);
    void vRowDataSize(i8 nRowSize);
    i8   nRowEntrySize(void);
    void vSetRowData(Item* aRowEntry);
  
    void vShow(ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
