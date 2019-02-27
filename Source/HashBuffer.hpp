/*============================================================================*/
/* HashBuffer.hpp -- HashMap Buffer                                           */
/*============================================================================*/
#pragma once

namespace Structures
{
  class HashBuffer : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HashBuffer(void);
    ~HashBuffer(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
  private:
    Footer* aFooter(void);
 };
  
  HashBuffer* _pHashBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
