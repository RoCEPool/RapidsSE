/*============================================================================*/
/* Question.hpp - Question                                                    */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Question : public Object
  {
  private:

    Fact* m_pRoot; //
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Question(void);
   ~Question(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Question* _pQuestion(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
