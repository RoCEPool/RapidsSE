/*============================================================================*/
/* NmStrList.hpp - list of name-strings                                       */
/*============================================================================*/
#pragma once

namespace Transform
{
  class NmStrList : public ListNL_t<NmStr>
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    NmStrList(void);
    ~NmStrList(void) noexcept(false);
    
  
    Footer* aFooter(void);
  };
  
  NmStrList* _pNmStrList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
