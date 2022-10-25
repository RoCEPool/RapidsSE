
namespace java com.rapidsdata.wirelineprotocol.messages.v5
namespace cpp  com.rapidsdata.wirelineprotocol.messages.v5

// Data types of column values.
enum DataType {
  NULL,
  BOOLEAN,
  TINYINT,
  SMALLINT,
  INTEGER,
  BIGINT,
  DECIMAL,
  FLOAT,
  DATE,
  TIME,
  TIMESTAMP,
  INTERVAL,
  VARCHAR,
  VARBINARY
}

// Information about a column in a result set.
struct ColumnMetadata {

  // The name of the column.
  1: required string columnName;

  // The type of the column.
  2: required DataType columnType;

  // The precision of the column type, if applicable.
  3: optional i32 precision;

  // The scale of the column type, if applicable.
  4: optional i32 scale;

  // The nullability of the column type, if applicable.
  5: optional bool isNullable;

  // The name of the charset for this column.
  6: optional string charsetName;

  // The name of the collation for this column.
  7: optional string collationName;

  // True if this column is automatically numbered.
  8: optional bool isAutoIncrement
}
