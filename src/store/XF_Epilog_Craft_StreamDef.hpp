/*============================================================================*/
/* XF_Epilog_Craft_StreamDef.hpp -- Craft Stream Def's (arriving & emitted)   */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Craft_StreamDef : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Craft_StreamDef(CardIter* pCI);
   ~XF_Epilog_Craft_StreamDef(void) noexcept(false);

    static void vCraft(CardIter* pCI);

    void vCraft(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Craft_StreamDef* _pXF_Epilog_Craft_StreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
