/*============================================================================*/
/* CsvMeta.hpp -- Metadata about a csv chunk                                  */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class CsvMeta : public Object
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CsvMeta(void);
   ~CsvMeta(void) noexcept(false);
    
  public:
  
  private:
    Footer* aFooter(void);
  };
  
  CsvMeta* _pCsvMeta(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
