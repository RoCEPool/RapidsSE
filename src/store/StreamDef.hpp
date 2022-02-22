/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

#include "Definition.hpp"
#include "StreamDefBase.hpp"

namespace STORE
{
  class StreamDef : public StreamDefBase
  {
  public:
  
  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    StreamDef(StreamDef* pStreamDef);
   ~StreamDef(void) noexcept(false);

    static void vPostRecv(void* aStreamDefRht);
    static void vRepair(Object* pObject);

    StreamDef* pDup(void);

    Footer* aFooter(void);
  };
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
