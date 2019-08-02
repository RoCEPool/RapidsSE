/*============================================================================*/
/* OpDesc.cpp -- the class which populates the op catalog                     */
/*============================================================================*/

#include "PushDown.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpDesc::OpDesc(eCN  eClass,     // class number
               eOK  eKind,      // op kind
               Stz* szCommand,  // first word of prompt command
               eOR  (*fpOpFun)(void* aThis),  // function which does the op
               void (*fpDrain)(void* aThis)   // function which drains the operand queue
              )
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
