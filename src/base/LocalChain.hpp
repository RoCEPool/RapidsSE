/*============================================================================*/
/* LocalChain.hpp -- Locals Frame Chain                                       */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_LOCALS_arguments = 24;
  
  class LocalChain : public Object
  {
  public:
    LocalChain* m_pNext;             // nest frame in chain
    TypedCell*  m_aDiscardPoint;     // discard point (when in call stack)
    Local       m_xLocal[C_LOCALS_arguments];  // set of local values
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalChain(void);
    ~LocalChain(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    TypedCell* aDiscardPoint(void);
    void vDiscardPoint(TypedCell* aDiscardPoint);
    void vDump(void);

    Local* pLocal(i8 i_Local);
 
    LocalChain* pNext(void);
    void vNext(LocalChain* pLocalSet);
  
    // must be last
    Footer* aFooter(void);
  };
  
  LocalChain* _pLocalChain(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
