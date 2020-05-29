/*============================================================================*/
/* RegSet.hpp -- task register set                                            */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class RegSet : public Object
  {
  public:
  
    Dir*  m_pDir_0;
    Dir*  m_pDir_1;
    Dir*  m_pDir_2;
    Dir*  m_pDir_3;
  
    File* m_pFile_0;
    File* m_pFile_1;
    File* m_pFile_2;
    File* m_pFile_3;

    s32   m_s32_0;
    s32   m_s32_1;
    s32   m_s32_2;
    s32   m_s32_3;

    s256  m_s256_0;
    s256  m_s256_1;
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RegSet(void);
   ~RegSet(void) noexcept(false);
        
    Footer* aFooter(void);
  };
  
  RegSet* _pRegSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
