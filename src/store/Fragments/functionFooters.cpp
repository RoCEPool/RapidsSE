
/*----------------------------------------------------------------------------*/
/*-- Function footer Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include "BASE.h"
#include "ClangStuff.h"
#include "ClangDynamic.h"

// function-vInflateCrwEnd
/*--------------------------------------------------------------------*/
/* vInflateCrwEnd                                                     */
/*--------------------------------------------------------------------*/
int vInflateCrwEnd(struct Dynamic_T* aDynamicT)
{
  {
    i8* paW;

    #include "vFLAT_Parameters.h"

    __L_LABEL_LOOP__:

    aSubReturnLabel = && __L_LABEL_LOOP__;

  // marker-start
  goto __L_LABEL_LOOP__;

  L_Return:
    if (paW != &aDynamicT->m_aW[0])
      NEVER_REACH;

    goto L_ReturnFinal;

#include "../src/store/vFLAT_InflateColumn.h"
#include "../src/store/vFLAT_SkipColumn.h"

    L_ReturnFinal:
    return 1;
  }
 // marker-end
}

// function-vInflateRowIndexEnd
/*--------------------------------------------------------------------*/
/* vInflateRowIndexEnd                                                */
/*--------------------------------------------------------------------*/
int vInflateRowIndexEnd(struct Dynamic_T* aDynamicT)
{
  {

    i8* paW;

    #include "vFLAT_Parameters.h"

    __L_LABEL_LOOP__:

    aSubReturnLabel = && __L_LABEL_LOOP__;

    // marker-start
    goto __L_LABEL_LOOP__;

    L_Return:
    if (paW != &aDynamicT->m_aW[0])
      NEVER_REACH;
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

    i8* paW;

    __L_LABEL_LOOP__:

    // marker-start
    goto __L_LABEL_LOOP__;

    L_Return:
    if (paW != &aDynamicT->m_aW[0])
      NEVER_REACH;
    goto L_ReturnFinal;

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

    i8* paW;

#include "vFLAT_Parameters.h"

    __L_LABEL_LOOP__:

    aSubReturnLabel = && __L_LABEL_LOOP__;

    // marker-start
    goto __L_LABEL_LOOP__;

    L_Return:
    if (paW != &aDynamicT->m_aW[0])
      NEVER_REACH;
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
  i8* paW;

  // marker-start
  L_Return:
    //if (paW != &aDynamicT->m_aW[0])
    //  NEVER_REACH;
    return 1;
}
// marker-end

// function-iStreamDefEnd
/*--------------------------------------------------------------------*/
/* iStreamDefEnd                                                      */
/*--------------------------------------------------------------------*/
int iStreamDefEnd(struct Dynamic_T* aDynamicT)
{
  // marker-start
  return 1;
}
// marker-end