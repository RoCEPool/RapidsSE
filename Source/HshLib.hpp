/*============================================================================*/
/* HshLib.hpp -- hash utility                                                 */
/*============================================================================*/
#pragma once

namespace Datums
{
  class HshLib
  {
  public:
    static i8 iHash(byte* ayaKey, i8 nBytes);

    static i8 iHashReverse(byte* ayaKey, i8 nBytes);
    static i8 iHashReverseRotate(byte* ayaKey, i8 nBytes, i8 nRotate);

    static i8 iHashRotate(byte* ayaKey, i8 nBytes, i8 nRotate);
    static i8 iHashRotateReverse(byte* ayaKey, i8 nBytes, i8 nRotate);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
