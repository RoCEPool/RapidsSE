/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

#include "eOI.h"

#include "InfoSetBase.hpp"

namespace STORE
{
  class InfoSet : public InfoSetBase
  {
  protected:
    eOI  m_eOpInfoSet;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    InfoSet(StreamDef* pStreamDef);
   ~InfoSet(void) noexcept(false);

    static void vPostRecv(void* aInfoSet);
    static void vRepair(Object* pObject);

    void vOpInfoSet(eOI eOpInfoSet);
    eOI  eOpInfoSet(void);
  
    Footer* aFooter(void);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
