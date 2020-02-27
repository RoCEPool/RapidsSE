/*============================================================================*/
/* Loader.hpp -- Loader                                                       */
/*============================================================================*/
#pragma once

namespace Load
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Loader : public WriteBehind
  {
  public:
 
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Loader(Table* pTable);
   ~Loader(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    Footer* aFooter(void);
    
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Loader* _pLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
