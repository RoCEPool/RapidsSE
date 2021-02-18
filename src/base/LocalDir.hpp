/*============================================================================*/
/* LocalDir.hpp -- Local Dir (on thread call stack)                           */
/*============================================================================*/
#pragma once

namespace BASE
{
  class LocalDir : public Dir
  {
  private:
  
    // must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pFileAddress);
  
    LocalDir(void);
   ~LocalDir(void) noexcept(false);
      
    //Footer* aFooter(void);
  };
  
  //LocalDir* _pLocalDir(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
