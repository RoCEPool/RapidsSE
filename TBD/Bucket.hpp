/*----------------------------------------------------------------------------*/
/*  Bucket.hpp - Bucket                                                       */
/*  RapidsSE                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <stdio.h>
#include "Common.h"

using namespace Common;

namespace SuperStructure {
  
class Bucket {
  
private:
  i8  m_iBucketNo;
  i8  m_iParitionNo;
  i8  m_iNodeNo;
  i8  m_nPartitionMgr;
};
  
}
