/*============================================================================*/
/* PlanConstants.hpp - Common constants for the plan graph                    */
/*============================================================================*/
#pragma once

namespace Parser
{
  // Separators
  static const Stz* C_PROPERTY_SEPARATOR  = ":"_Stz;
  static const Stz* C_CHILD_SEPARATOR     = "/"_Stz;

  // Names / identifiers
  static const Stz* C_ROOT_NAME         = "__root__"_Stz;
  static const Stz* C_PRIMITIVES_NAME   = "primitives"_Stz;
  static const Stz* C_DEFINITIONS_NAME  = "definitions"_Stz;
  static const Stz* C_PLAN_NAME         = "plan"_Stz;
  static const Stz* C_METADATA_NAME     = "metadata"_Stz;
  static const Stz* C_STATISTICS_NAME   = "statistics"_Stz;

  // Paths of nodes
  static const Stz* C_ROOT_PATH         = "/__root__"_Stz;
  static const Stz* C_PRIMITIVES_PATH   = "/__root__/primitives"_Stz;
  static const Stz* C_DEFINITIONS_PATH  = "/__root__/definitions"_Stz;
  static const Stz* C_PLAN_PATH         = "/__root__/plan"_Stz;
  static const Stz* C_METADATA_PATH     = "/__root__/metadata"_Stz;
  static const Stz* C_STATISTICS_PATH   = "/__root__/statistics"_Stz;

  // Common type definition paths
  static const Stz* C_STRING_OBJ_DEFN = "/definitions/object/valueObject/stringObject"_Stz;
  static const Stz* C_STRING_OBJ_DEFN_NONNULL = "/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

  static const Stz* C_ROOT_DEFN_PATH  = "/__root__/primitives/root"_Stz;
  static const Stz* C_TRUNK_DEFN_PATH = "/__root__/primitives/trunk"_Stz;

  static const Stz* C_ROOT_CHILDREN_NAMES[] = { C_PRIMITIVES_NAME,
                                                C_DEFINITIONS_NAME,
                                                C_PLAN_NAME,
                                                C_METADATA_NAME,
                                                C_STATISTICS_NAME,
                                                nullptr };
}