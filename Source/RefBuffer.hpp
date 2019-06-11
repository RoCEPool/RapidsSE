/*============================================================================*/
/* RefBuffer.hpp -- Reference Buffer                                          */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class RefBuffer : public DataBuffer,
                    public MiniHeap_t<RefSet>
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RefBuffer(void);
    ~RefBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    RefSet* aAllotRefSet(i8 nRowRefs);
  
    RefSet* aFirstRefSet(void);
    RefSet* aNextRefSet(RefSet* aPriorRefSet);
    void    vShow(void);
    
  private:
    Footer* aFooter(void);
  };
  
  RefBuffer* _pRefBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
