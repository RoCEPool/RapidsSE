/*============================================================================*/
/* Arrange.hpp -- Arrange things in alphabetic order                          */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Arrange : public Object
  {
  private:
    Lock m_okLock;     // lock
  
    SkipListNL* m_pSkipList; // skip list of things to be arranged
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Arrange(void);
   ~Arrange(void) noexcept(false);
    
    void vBeginIter(void** vppCursor);
    void vEndIter(void** vppCursor);
    bool bFirst(void** vppCursor);
    Thing* pFirst(void** vppCursor);
    void vInsert(Stz* szObjNm, Object* pObject);
    void vInsert(Thing* pThing);
    bool bNext(void** vppCursor);
    Thing* pNext(void** vppCursor);
    Object* pObject(void** vppCursor);
    i8 iSeqNo(void** vppCursor);
    void vSequence(void);
    Stz* szObjNm(void** vppCursor);

    Footer* aFooter(void);
  };
  
  Arrange* _pArrange(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
