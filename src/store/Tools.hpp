/*============================================================================*/
/* Tools.hpp -- Drafting Tools                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"
#include "eBK.h"

namespace STORE { class Bench; }


namespace STORE
{
  class Tools : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Tools(void);
   ~Tools(void) noexcept(false);
  

  private:
  
    static void vOrderStreamDef(StreamDef* pStreamDef,
                                Card_GbyList* pCard_ByList);

  
    Footer* aFooter(void);
  };

  Tools* _pTools(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
