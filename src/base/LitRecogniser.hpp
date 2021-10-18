/*============================================================================*/
/* LitRecogniser.hpp -- Lit Recogniser                                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class LitRecogniser : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pLitRecogniser);
  
    LitRecogniser(void);
   ~LitRecogniser(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  LitRecogniser* _pLitRecogniser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
