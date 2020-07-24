/*============================================================================*/
/* Profile.hpp - runtime profile of task                                      */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Profile : public Operand
  {
  private:
    
    s256 m_sOpName;
    
    // timestamp
    // version
    // git hash
  
    RU_Memory m_oRU_Memory;
    RU_Cpu    m_oRU_Cpu;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Profile(Stz* szOpName);
   ~Profile(void) noexcept(false);
    
    void       vOpName(Stz* aName);
    RU_Cpu*    pRU_Cpu();
    RU_Memory* pRU_Memory();
    void       vWriteReport(File* pFile);
    
    Footer* aFooter(void);
  };
  
  Profile* _pProfile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
