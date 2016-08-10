/*****************************************************************************/
/* Random.h -- random number generator utility                               */
/*****************************************************************************/
#pragma once

namespace UL {

class Random
{
public:
  static inw iRandom(void);
  static inw iRandom(inw iLow, inw iHigh);

  static uiw uRandom(void);
  
  static inw iLogLevel(inw nLevels);
  
  static inw nLevelLog(inw nLevels);
};

}

/*****************************************************************************/
/* end of file                                                               */
/*****************************************************************************/
