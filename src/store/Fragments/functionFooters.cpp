
/*----------------------------------------------------------------------------*/
/*-- Function footer Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include "BASE.h"


// function-vInflateCrwEnd
/*--------------------------------------------------------------------*/
/* vInflateCrwEnd                                                     */
/*--------------------------------------------------------------------*/
int vInflateCrwEnd(struct Dynamic_T* aDynamicT)
{
  {

    #include "vFLAT_Parameters_v2.h"

    L_AdvanceNextRow:

    aSubReturnLabel = && L_AdvanceNextRow;

  // marker-start
  goto L_AdvanceNextRow;

  L_Return:
    goto L_ReturnFinal;

#include "../src/store/vFLAT_InflateColumn.h"
#include "../src/store/vFLAT_SkipColumn.h"

    L_ReturnFinal:
    return 1;
  }
 // marker-end
}

// function-vInflateRowEnd
/*--------------------------------------------------------------------*/
/* vInflateRowEnd                                                     */
/*--------------------------------------------------------------------*/
int vInflateRowEnd(struct Dynamic_T* aDynamicT)
{
  {

    #include "vFLAT_Parameters_v2.h"

    L_AdvanceNextRow:

    aSubReturnLabel = && L_AdvanceNextRow;

    // marker-start
    goto L_AdvanceNextRow;

    L_Return:
    goto L_ReturnFinal;

#include "../src/store/vFLAT_InflateColumn.h"
#include "../src/store/vFLAT_SkipColumn.h"

    L_ReturnFinal:
    return 1;
  }
  // marker-end
}

// function-vIndexScanEnd
/*--------------------------------------------------------------------*/
/* vIndexScanEnd                                                      */
/*--------------------------------------------------------------------*/
int vIndexScanEnd(struct Dynamic_T* aDynamicT)
{
  {

#include "vFLAT_Parameters_v2.h"

    L_AdvanceNextRow:

    aSubReturnLabel = && L_AdvanceNextRow;

    // marker-start
    goto L_AdvanceNextRow;

    L_Return:
    goto L_ReturnFinal;

#include "../src/store/vFLAT_InflateColumn.h"
#include "../src/store/vFLAT_SkipColumn.h"

    L_ReturnFinal:
    return 1;
  }
  // marker-end
}

// function-iEosEnd
/*--------------------------------------------------------------------*/
/* iEosEnd                                                            */
/*--------------------------------------------------------------------*/
int iEosEnd(struct Dynamic_T* aDynamicT)
{
  // marker-start
  L_Return:
    return 1;
}
// marker-end