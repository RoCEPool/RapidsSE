/*============================================================================*/
/* RluDiary.hpp -- Rlu Diary                                                  */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class RluDiary : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RluDiary(void);
   ~RluDiary(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  RluDiary* _pRluDiary(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
