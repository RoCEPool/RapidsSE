/*============================================================================*/
/* HintConstants.hpp - Common constants used in hints                         */
/*============================================================================*/
#pragma once

namespace Parser
{
  //******************
  // Common Hint Keys
  //******************
  static Stz* C_HINT_KEY_JOIN_ALGO    = "joinAlgorithm"_Stz;
  static Stz* C_HINT_KEY_JOIN_ROUTING = "joinRouting"_Stz;


  //********************
  // Common Hint Values
  //********************

  // For C_HINT_KEY_JOIN_ALGO:
  static Stz* C_HINT_VAL_JOIN_ALGO_LOOP  = "loop"_Stz;
  static Stz* C_HINT_VAL_JOIN_ALGO_INDEX = "index"_Stz;
  static Stz* C_HINT_VAL_JOIN_ALGO_HASH  = "hash"_Stz;
  static Stz* C_HINT_VAL_JOIN_ALGO_MERGE = "merge"_Stz;

  // For C_HINT_KEY_JOIN_ROUTING
  // Refer to the strings returned by Routing::szRoutingType(eRT eRouteType);


}