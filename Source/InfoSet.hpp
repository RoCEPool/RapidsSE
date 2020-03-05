/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

namespace InfoData
{
  class InfoSet : public Operand
  {
  protected:
    EnameSet m_oEnameSet;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    ~InfoSet(void) noexcept(false);
    
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
