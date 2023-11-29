#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Query Dialog Example With a Single Statement"

participant "Client" as c
participant "Server" as s

activate c
activate s

note over c
  """
  The client sends a query to the server
  with a single statement. 
  e.g.
  "SELECT * FROM t1;
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
  Execution of all statements has completed.
  """
end note

s->c: QueryExecuteResponse

note over (:---:) c, s
  "Dialog ends"
end note