/*============================================================================*/
/* ClassFlags.hpp -- Class Flags                                              */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class ClassFlags : public Object
  {
  private:
    bool m_bAll;
    bool m_bClass[SR_ENUMS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pAddr);
  
    ClassFlags(void);
    ~ClassFlags() noexcept(false);
  
    bool bAll(void);
    void vAll(bool bAll);
  
    bool bClass(eCN eClassNo);
    void vClass(eCN eClassNo, bool bFlag);
  
    bool bFlagged(eCN eClassNo);
  
    Footer* aFooter(void);
  };
  
  ClassFlags* _pClassFlags(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
