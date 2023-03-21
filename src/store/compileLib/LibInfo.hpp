/*============================================================================*/
/* LibInfo.hpp - Stateful information about compiling and using a dynamic lib */
/*============================================================================*/
#pragma once

namespace STORE
{
  class LibInfo : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LibInfo(void);
    ~LibInfo(void);

    Footer* aFooter(void);
  };

  LibInfo* _pLibInfo(void* aAddr);
} // STORE

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
