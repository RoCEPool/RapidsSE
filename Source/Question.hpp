/*============================================================================*/
/* Question.hpp - Question                                                    */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Question : public Object
  {
  private:

    Kbase* m_pKbase; //
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Question(Kbase* pKbase);
   ~Question(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Question* _pQuestion(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
