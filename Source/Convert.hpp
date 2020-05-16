/*============================================================================*/
/* Convert.hpp -- Fast Convert of Csv Record                                  */
/*============================================================================*/
#pragma once

namespace Load
{
  class Convert : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Convert(Format* pFmt);
   ~Convert(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };

  Convert* _pConvert(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
