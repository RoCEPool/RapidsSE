#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Query Dialog Example With Multiple Statements"

participant "Client" as c
participant "Server" as s

activate c
activate s

note over c
  """
  The client sends a query to the server
  with multiple (batched) statements. 
  e.g.
  "SELECT * FROM t1;
  DROP TABLE t1;
  SELECT * FROM t2;
  DROP TABLE t3;"
  """
end note
c->s: QueryExecuteRequest

s->c: QueryReceiptResponse

note over s
  """
  Server begins to execute the first statement:
  SELECT * FROM t1;
  """
end note

s->c: RowMetadataResponse
s->c: RowDataResponse
s->c: RowDataResponse
s->c: RowDataResponse
s->c: StatementResponse

note over s
  """
  Server begins to execute the second statement:
  DROP TABLE t1;
  This statement does not generate a result set.
  """
end note

s->c: StatementResponse


note over s
  """
  Server begins to execute the third statement:
  DROP TABLE t2;
  Table t2 is empty, so this statement generates
  an empty result set.
  """
end note

s->c: RowMetadataResponse
s->c: StatementResponse

note over s
  """
  Server begins to execute the fourth statement:
  DROP TABLE t3;
  Table t3 does not exist so an error is generated.
  """
end note

s->c: ErrorResponse

    
note over s
  """
  Execution of all statements has completed.
  """
end note

s->c: QueryExecuteResponse

note over (:---:) c, s
  "Dialog ends"
end note