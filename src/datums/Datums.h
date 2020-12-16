/*============================================================================*/
/* Datums.h -- Datums namespace                                               */
/*============================================================================*/
#pragma once

#include "SenVM.h"

//------------------------------------------------------------------------------

namespace Datums
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Datum.hpp"

typedef Datum_t<C_48KB> Datum;
typedef Datum_t<32>     Datum_y32;

#include "ColPix.h"
#include "RowPix.h"
#include "Column.hpp"
typedef VectorNL_t<CN_PTNCOLVEC,Column,1024> PtnColVec;

#include "SN_Bcd.hpp"
#include "SN_Bol.hpp"
#include "SN_Dte.hpp"
#include "SN_Flt.hpp"
#include "SN_Int.hpp"
#include "SN_Ivl.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Vch.hpp"

#include "SP_Bcd.hpp"
#include "SP_Bol.hpp"
#include "SP_Dte.hpp"
#include "SP_Flt.hpp"
#include "SP_Int.hpp"
#include "SP_Ivl.hpp"
#include "SP_Tme.hpp"
#include "SP_Tsp.hpp"
#include "SP_Vch.hpp"

#include "CsvValue.hpp"
#include "DatumToken.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
