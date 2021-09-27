/*============================================================================*/
/* Gizmos.cpp -- Useful Gizmos                                                */
/*============================================================================*/

#include "BASE.h"

GizmoFn __AsciiTable("ascii"_Stz,(fpSELFFN*)Gizmos::vAsciiTable,""_Stz,nullptr);

GizmoFn __Pow2("pow2"_Stz,(fpSELFFN*)Gizmos::vPow2,""_Stz,nullptr);

/*----------------------------------------------------------------------------*/
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void Gizmos::vAsciiTable(TokenVec* pTokenVec)
{
  _static_
  UNUSED(pTokenVec);

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

/*----------------------------------------------------------------------------*/
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void Gizmos::vPow2(TokenVec* pTokenVec)
{
  _static_
  UNUSED(pTokenVec);

  u8 uVal = 1;
  
  for (i8 i = 0; i < 64; i++) {
    vTrace("%2lld: %llu\n"_Stz,i,uVal);
    uVal *= 2;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
