/*============================================================================*/
/* IndexColumn.hpp - Index Column                                             */
/*============================================================================*/
#pragma once

#include "DBobjects.h"

using namespace System;

namespace DBobjects
{
  class IndexColumn
  {
  private:
    i8    m_iIndexColumnNo;
    i8    m_iColumnNo;
    i8    m_nIndexColumnMgr;
    
    // must be last
    Footer m_xFooter;
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
