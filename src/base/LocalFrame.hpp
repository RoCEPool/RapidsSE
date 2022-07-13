/*============================================================================*/
/* LocalFrame.hpp -- Locals Frame Chain                                       */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_LOCALS_PER_FRAME = 24;
  
  class LocalFrame : public Object
  {
  public:
    LocalFrame*     m_pNext;             // nest frame in chain
    TypeValueArea*  m_aDiscardPoint;     // discard point (when in call stack)
    SenCell         m_xaSenCell[C_LOCALS_PER_FRAME];  // set of local variables
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalFrame(void);
   ~LocalFrame(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    TypeValueArea* aDiscardPoint(void);
    void vDiscardPoint(TypeValueArea* aDiscardPoint);
    void vDump(void);

    SenCell* pSenCell(i8 i_Local);
 
    LocalFrame* pNext(void);
    void vNext(LocalFrame* pLocalFrame);
  
    // must be last
    Footer* aFooter(void);
  };
  
  LocalFrame* _pLocalFrame(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
