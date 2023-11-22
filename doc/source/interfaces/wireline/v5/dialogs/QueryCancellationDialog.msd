#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Query Cancellation Dialog"

participant "Client\nConnection 1" as c1
participant "Client\nConnection 2" as c2
participant "Server" as s

activate c1
activate s

note over c1
  """
  The client sends a query to the server
  with a single statement. 
  e.g.
  "SELECT * FROM t1;
  """
end note
c1->s: QueryExecuteRequest

s->c1: QueryReceiptResponse

note over c1
  """
  Client remembers unique 
  statement ID and secret key from
  QueryReceiptResponse message
  """
end note

note over s
  """
  Server begins to execute the 
  statement and return results
  """
end note

s->c1: RowMetadataResponse
s->c1: RowDataResponse
s->c1: RowDataResponse

activate c2

note over c1,c2
  """
  Client decides to cancel the query
  & creates a new connection
  """
end note

c2-->s: Establish connection
ref over c2, s
  "Connection Dialog"
end ref

note over c2
  """
  Client uses statement ID and
  secret key previously saved to 
  send QueryCancellationRequest
  """
end note

c2->s: QueryCancellationRequest


loop [while has warning]
  s->c2: WarningResponse
end

alt [cancellation failed]
  s->c2: ErrorResponse

else [cancellation succeeded]
  s->c2: QueryCancellationResponse
end

note over (:---:) c2
  "Dialog ends"
end note

c2-->c2: Client closes\nthis connection
destroy c2


note over s
  """
  The statement is now cancelled
  & client is sent an error
  """
end note

s->c1: ErrorResponse
    
note over s
  """
  Execution of all statements has completed.
  """
end note

s->c1: QueryExecuteResponse

note over (:---:) c1, s
  "Dialog ends"
end note