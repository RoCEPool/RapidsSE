/*============================================================================*/
/* Numbering.hpp -- Object Numbering                                    */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Numbering : public Object
  {

  private:
    Atomic  m_inaNextNo[CN_ENUMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Numbering(void);
   ~Numbering(void) noexcept(false);
  
    i8 iNextNo(eCN eClassNo);
  
  private:
    Footer* aFooter(void);
  };
  
Numbering* _pNumbering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
