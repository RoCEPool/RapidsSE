/*============================================================================*/
/* Objects.hpp -- Object Numbering                                    */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Objects : public Object
  {

  private:
    Atomic  m_ixaNextObjectId[CN_ENUMS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Objects(void);
   ~Objects(void) noexcept(false);
  
    i8 iNextNo(eCN eClassNo);
  
    void vNodeNo(i8 iThisNodeNo);

  private:
    Footer* aFooter(void);
  };
  
  Objects* _pObjects(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
