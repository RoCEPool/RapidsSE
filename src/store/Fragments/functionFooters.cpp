
/*----------------------------------------------------------------------------*/
/*-- Function footer Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include "BASE.h"


// function-vInflateCrwEnd
/*--------------------------------------------------------------------*/
/* flattened function -- CRWBUFFER                                    */
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

// function-iEosEnd
/*--------------------------------------------------------------------*/
/* flattened function -- EOS                                          */
/*--------------------------------------------------------------------*/
int iEosEnd(struct Combined_T* aCombinedT)
{
  // marker-start
  L_Return:
    return 1;
}
// marker-end