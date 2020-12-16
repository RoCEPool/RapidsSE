/*============================================================================*/
/* Fault.cpp -- Fault Handler                                                 */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* handle an invalid default value in a switch statement                      */
/*----------------------------------------------------------------------------*/
void Fault::SwitchDefault(i8 eVal,
                          i8 e_Enum,
                          const char* azFunc,
                          const char* azFilePath,
                          i8 iLineNo)
{
  _static_
  
  vThrow(FK_CHECK,VL_FATAL,azFunc,azFilePath,iLineNo,
         "Invalid default in switch statement: switch(%lld)"_Stz,eVal);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
