/*============================================================================*/
/* ResultRow.hpp -- result row                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ResultRow : public Item
  {
  protected:

  public:
    void* operator new(size_t);
    void* operator new(size_t, ResultRow*);
  
    ResultRow(i8 nSizeRow);
    ~ResultRow(void);
  
    Row* aRow(void);

  };
  
  ResultRow* _pResultRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
