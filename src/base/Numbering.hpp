/*============================================================================*/
/* Numbering.hpp -- Object Numbering                                    */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Numbering : public Object
  {

  private:
    Atomic  m_ixaNextObjectId[CN_ENUMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Numbering(void);
   ~Numbering(void) noexcept(false);
  
    i8 iNextNo(eCN eClassNo);
  
    void vNodeNo(i8 iThisNodeNo);

  private:
    Footer* aFooter(void);
  };
  
Numbering* _pNumbering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
