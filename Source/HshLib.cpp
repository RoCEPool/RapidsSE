/*============================================================================*/
/* HshLib.cpp -- hash utility                                                 */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHash(byte* ayaKey, i8 nBytes)
{
  i8 iHash = XXH32(ayaKey,nBytes,0);

  return iHash;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHash32(byte* ayaKey, i8 nBytes)
{
  i8 iHash = XXH32(ayaKey,nBytes,0);
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* hash key                                                                   */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHash64(byte* ayaKey, i8 nBytes)
{
  i8 iHash = XXH64(ayaKey,nBytes,0);
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* hash reverse of key                                                        */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHashReverse(byte* ayaKey, i8 nBytes)
{
  byte yaPerturbedKey[nBytes];
  
  // reverse the key
  i8 i_Src = nBytes - 1;
  for (i8 i_Dst = 0; i_Dst < nBytes; i_Dst++, i_Src--)
    yaPerturbedKey[i_Dst] = ayaKey[i_Src];

  // hash the reversed key
  i8 iHash = XXH32(yaPerturbedKey,nBytes,0);
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* hash reverse rotation of key                                               */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHashReverseRotate(byte* ayaKey, i8 nBytes, i8 nRotate)
{
  byte yaPerturbedKey[32];//[nBytes];

  // reverse the key
  i8 i_Src = nBytes - 1;
  for (i8 i_Dst = 0; i_Dst < nBytes; i_Dst++, i_Src--)
    yaPerturbedKey[i_Dst] = ayaKey[i_Src];

  // rotate the key
  if (nRotate >= nBytes)
    nRotate = nBytes - 1;
  MemLib::vRotate(yaPerturbedKey,nBytes,nRotate);

  // hash the perturbed key
  i8 iHash = XXH32(yaPerturbedKey,nBytes,0);
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* hash rotation of key                                                       */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHashRotate(byte* ayaKey, i8 nBytes, i8 nRotate)
{
  byte yaPerturbedKey[32];//[nBytes];
  
  // rotate the key
  MemLib::vCopy(yaPerturbedKey,ayaKey,nBytes,CN_NONE);
  if (nRotate >= nBytes)
    nRotate = nBytes - 1;
  MemLib::vRotate(yaPerturbedKey,nBytes,nRotate);

  // hash the perturbed key
  i8 iHash = XXH32(yaPerturbedKey,nBytes,0);

  return iHash;
}

/*----------------------------------------------------------------------------*/
/* hash rotation,reversal of key                                              */
/*----------------------------------------------------------------------------*/
i8 HshLib::iHashRotateReverse(byte* ayaKey, i8 nBytes, i8 nRotate)
{
  byte yaPerturbedKey[32];//[nBytes];
  byte yaPerturbedKey2[32];//[nBytes];

  // rotate the key
  MemLib::vCopy(yaPerturbedKey,ayaKey,nBytes,CN_NONE);
  if (nRotate >= nBytes)
    nRotate = nBytes - 1;
  MemLib::vRotate(yaPerturbedKey,nBytes,nRotate);
  
  // reverse the key
  i8 i_Src = nBytes - 1;
  for (i8 i_Dst = 0; i_Dst < nBytes; i_Dst++, i_Src--)
    yaPerturbedKey2[i_Dst] = yaPerturbedKey[i_Src];

  i8 iHash = XXH32(yaPerturbedKey2,nBytes,0);
  
  return iHash;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
