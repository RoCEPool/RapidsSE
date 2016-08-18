/*============================================================================*/
/* IndexColumn.hpp - Index Column                                             */
/*============================================================================*/

#pragma once

#include <stdio.h>
#include "Common.h"

using namespace Common;

namespace DBobjects
{
  class IndexColumn {

  private:
    i8    m_iTxId;
    i8    m_iIndexColumnNo;
    i8    m_iColumnNo;
    i8    m_nIndexColumnMgr;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/