/*============================================================================*/
/* Profile.hpp - runtime profile of task                                      */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Profile : public Operand
  {
  private:
    
    // timestamp
    // version
    // git hash
  
    RU_Memory m_oRU_Memory;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Profile(void);
   ~Profile(void) noexcept(false);
    
    RU_Memory* pRU_Memory();
    void       vWriteReport(File* pFile);
    
    Footer* aFooter(void);
  };
  
  Profile* _pProfile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
