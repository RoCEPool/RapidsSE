/*============================================================================*/
/* ObjectTracker.hpp -- count of allocs and frees by class                    */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ObjectTracker : public Object
  {

  public:

    struct {
      u8 m_uAllocateCount;      // number of times this class was allocated
      u8 m_uFreeCount;          // number of times this class was freed
    } m_xClassCount[CN_ENUMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void* pAddr);

    ObjectTracker();

    ~ObjectTracker(void) noexcept(false);

    void vDump(Stz* szName);
    void vTrackAllocClass(eCN eClassNo);
    void vTrackFreeClass(eCN eClassNo);

  private:

    Footer* aFooter(void);

  };

  ObjectTracker* _pObjectTracker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
