/*============================================================================*/
/* MetaDataTables.hpp -- MetaData Tables                                      */
/*============================================================================*/
#pragma once

#include "eMD.h"

namespace STORE
{
  class MetaDataTables : public Object
  {
  private:

    StreamDef* m_apStreamDef[MD_ENUMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaDataTables(void);
    ~MetaDataTables(void) noexcept(false);

    StreamDef* pStreamDef(eMD eTable);
    void vStreamDef(eMD eTable, StreamDef* pStreamDef);

  private:

    Footer* aFooter(void);
  };

  MetaDataTables* _pMetaDataTables(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
