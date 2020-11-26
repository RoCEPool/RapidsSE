/*============================================================================*/
/* BPA.hpp -- buffer pool analyzer                                            */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class BPA : public Object
  {
  public:
    i8  m_naObjectsAtTake[CN_ENUMS];
    i8  m_naObjectsAtDump[CN_ENUMS];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
 
    BPA(void);
   ~BPA(void) noexcept(false);

  public:
    void vDeltaReport(s48K& rsReport);
    void vEpochDump(s48K& rsReport);
    void vEpochTake(void);
    i8   nObjectDelta(void);
    void vObjectReport(s48K& rsReport);
    void vObjectSnapshot(i8* pCountObjects);
    void vReset(void);

    Footer* aFooter(void);
  };

  BPA* _pBPA(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
