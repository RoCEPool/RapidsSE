/*============================================================================*/
/* LocalFile.hpp -- Local File (on thread call stack)                         */
/*============================================================================*/
#pragma once

namespace BASE
{
  class LocalFile : public File
  {
  private:
  
    // must be last
    //Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pFileAddress);
  
    LocalFile(void);
   ~LocalFile(void) noexcept(false);
      
    //Footer* aFooter(void);
  };
  
  //LocalFile* _pLocalFile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
