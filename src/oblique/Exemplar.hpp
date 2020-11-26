/*============================================================================*/
/* Exemplar.hpp -- Best Practive Example Class                                */
/*============================================================================*/
#pragma once

namespace Oblique
{
  class Exemplar : public Object
  {
  private:
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Exemplar(void);
   ~Exemplar(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  Exemplar* _pExemplar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
