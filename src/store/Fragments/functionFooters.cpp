
/*----------------------------------------------------------------------------*/
/*-- Function footer Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include "BASE.h"


// function-vInflateCrwEnd
/*--------------------------------------------------------------------*/
/* vInflateCrwEnd                                                     */
/*--------------------------------------------------------------------*/
int vInflateCrwEnd(struct Combined_T* aCombinedT)
{
  {

    #include "vFLAT_Parameters.h"

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
int vInflateRowEnd(struct Combined_T* aCombinedT)
{
  {

    #include "vFLAT_Parameters.h"

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
int vIndexScanEnd(struct Combined_T* aCombinedT)
{
  {

#include "vFLAT_Parameters.h"

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
int iEosEnd(struct Combined_T* aCombinedT)
{
  // marker-start
  L_Return:
    return 1;
}
// marker-end