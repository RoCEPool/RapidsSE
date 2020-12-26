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

  // Paths of nodes
  static const Stz* C_ROOT_PATH         = "/__root__"_Stz;
  static const Stz* C_PRIMITIVES_PATH   = "/__root__/primitives"_Stz;
  static const Stz* C_DEFINITIONS_PATH  = "/__root__/definitions"_Stz;
  static const Stz* C_PLAN_PATH         = "/__root__/plan"_Stz;
  static const Stz* C_METADATA_PATH     = "/__root__/metadata"_Stz;
  static const Stz* C_STATISTICS_PATH   = "/__root__/statistics"_Stz;

  // Type definition paths
  static const Stz* C_STRING_OBJ_DEFN = "/definitions/object/valueObject/stringObject"_Stz;
  static const Stz* C_STRING_OBJ_DEFN_NONNULL = "/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

  // Statement types
  enum eST {
    ST_NONE = 0,
    ST_CREATETABLE,
    ST_DROPTABLE,
    ST_CREATEINDEX,
    ST_DROPINDEX,
    ST_CTAS,
    ST_SELECT,
    ST_INSERT,
    ST_UPDATE,
    ST_DELETE,
    ST_INSERTSELECT,

    ST_ENUMS
  };

  // Definition paths of the above statement types
  static Stz* aST_definition[] {
      /*ST_NONE         */ ""_Stz
      /*ST_CREATETABLE  */ ""_Stz
      /*ST_DROPTABLE    */ ""_Stz
      /*ST_CREATEINDEX  */ ""_Stz
      /*ST_DROPINDEX    */ ""_Stz
      /*ST_CTAS         */ ""_Stz
      /*ST_SELECT       */ "/__root__/definitions/object/planObject/statement/dml/select"_Stz
      /*ST_INSERT       */ ""_Stz
      /*ST_UPDATE       */ ""_Stz
      /*ST_DELETE       */ ""_Stz
      /*ST_INSERTSELECT */ ""_Stz
      /*ST_ENUMS        */ ""_Stz
  };


}