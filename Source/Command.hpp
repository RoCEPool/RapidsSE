/*============================================================================*/
/* Command.hpp -- Command                                                     */
/*============================================================================*/
#pragma once

namespace Commands
{
  class Command : public Focal
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* aAddr);
    
    Command(void);
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
