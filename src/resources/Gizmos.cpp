/*============================================================================*/
/* Gizmos.cpp -- Useful Gizmos                                                */
/*============================================================================*/

#include "Resources.h"

#include "eOC.h"

GizmoFn __AsciiTable("ascii"_Stz,(fpSELFFN*)Gizmos::vAsciiTable,""_Stz,-1);

/*----------------------------------------------------------------------------*/
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void Gizmos::vAsciiTable(void* aGizmoArg)
{
  _static_
  
  vTrace("Gizmos::vAsciiTable\n"_Stz);
  
  
  // if (1 == iNumTokens) {
    
    for (i4 i_Char = 0; i_Char < 128; i_Char++) {
      if (0 == i_Char % 8)
        vTrace("| "_Stz);
      vTrace("%02x %-3s | "_Stz,i_Char,StrLib::azAscii(i_Char));
      if (i_Char && 0 == (i_Char+1) % 8)
        vTrace("\n"_Stz);
    }

//    vTrace("\n"_Stz);
//    for (i4 i_Char = 0; i_Char < 32; i_Char++)
//      vTrace("%s"_Stz,aOC[i_Char]);
//    vTrace("\n"_Stz);

    return;
  //}
  
//  Token* pTkn1 = pInpTknVec->pGet(1);
//
//  char* aChar = (char*)pTkn1->szChars();
//  vTrace("Ascii: "_Stz);
//  while (*aChar)
//    vTrace("%02x "_Stz,*aChar++);
//  vTrace("\n"_Stz);
//
//  aChar = (char*)pTkn1->szChars();
//  vTrace("     : "_Stz);
//  while (*aChar)
//    vTrace(" %c "_Stz,*aChar++);
//  vTrace("\n"_Stz);
    

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
