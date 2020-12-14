/*============================================================================*/
/* ClassData.h -- includes for ClassData namespace                            */
/*============================================================================*/
#pragma once

#include "Strings.h"

//------------------------------------------------------------------------------

namespace ClassData
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ClassNumbers.hpp"
#include "FnDesc.hpp"
#include "FnDefn.hpp"
#include "FnCatalog.hpp"
#include "LimitsFormats.h"

#include "AM.hpp"
#include "eHT.h"

struct SizeHeap {
  
  i8  m_nClassSize;
  eHT m_eWhichHeap;
  
  i8  m_nVtableAdjust; // temp: explore support for vtable

};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
