/*============================================================================*/
/* BndLib.hpp - bound helper                                                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class BndLib
  {
  private:

  public:
    static bool bIsBefore(Key& xKeyThis, Key& xKeyTest,
                          RelOpAry& xOpAry, IdxColVec& xColVec);

    static bool bPassesFilter(Key& xKeyThis, Key& xKeyTest,
                              RelOpAry& xOpAry, IdxColVec& xColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
