/*============================================================================*/
/* StreamDefRht.hpp -- Stream Definition right                                */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "StreamDefBase.hpp"

namespace STORE
{
  class StreamDefRht : public StreamDefBase {

  public:

  private:

    // must be last
    Footer m_xFooter;

  public:

    void* operator new(size_t);
    void  operator delete(void*);

    StreamDefRht(void);
    StreamDefRht(StreamDefBase *pStreamDef);
    ~StreamDefRht(void) noexcept(false);

    static void vPostRecv(void* aStreamDefRht);
    static void vRepair(Object *pObject);

    Footer* aFooter(void);

  };
  StreamDefRht* _pStreamDefRht(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
