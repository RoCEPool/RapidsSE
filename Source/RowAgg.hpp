/*============================================================================*/
/* RowAgg.hpp -- Row Aggregation                                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry;
  
  class RowAgg : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowAgg*);
    
    RowAgg(void);
    ~RowAgg(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };
  
  RowAgg* _pRowAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
