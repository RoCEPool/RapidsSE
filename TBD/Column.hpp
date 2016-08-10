/*----------------------------------------------------------------------------*/
/*  Column.hpp - Column                                                       */
/*  RapidsSE                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <stdio.h>
#include "Common.h"

using namespace Common;

namespace SuperStructure {
  
  enum eCT // Column Type
  {
    /* 0 */ CT_NONE=0,
    /* 1 */ CT_XYZ
  };

  static const char *pCT[]
  {
    /* 0 */ "NONE",
    /* 1 */ "XYZ"
  };

  class Column {

  private:
  i8    m_iTxId;
  i8    m_iColumnNo;
  s32   m_sColumnName;
  eCT   m_eColumnType;
  bool  m_bIsNullable;
  i8    m_nColumnMgr;
  };

}

