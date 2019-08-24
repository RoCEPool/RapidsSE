/*============================================================================*/
/* Command.hpp -- Command                                                     */
/*============================================================================*/
#pragma once

namespace Sessions
{
  class Command : public Focal
  {
  private:
    TokenVec* m_p_TknVec;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* aAddr);
    
    Command(TokenVec* p_TknVec);
   ~Command(void) noexcept(false);
  
  public:
  
  private:
    Footer* aFooter(void);
  };
  
  Command* _pCommand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
