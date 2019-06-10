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
  
    RefSet* aAllotRefSet(i8 nRowRefs);
  
    RefSet* aFirstRefSet(void);
    RefSet* aNextRefSet(RefSet* aPriorRefSet);

  private:
    Footer* aFooter(void);
  };
  
  RefBuffer* _pRefBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
