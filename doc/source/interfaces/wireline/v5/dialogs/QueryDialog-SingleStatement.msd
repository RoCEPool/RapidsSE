#//# --------------------------------------------------------------------------------------
#//# Created using Sequence Diagram for Mac
#//# https://www.macsequencediagram.com
#//# https://itunes.apple.com/gb/app/sequence-diagram/id1195426709?mt=12
#//# --------------------------------------------------------------------------------------

title "Query Dialog (single statement)"

participant "Client" as c
participant "Server" as s

activate c
activate s


c->s: QueryExecuteRequest


alt [request has an error]
  s->c: ErrorResponse
  note over c
    "Dialog ends"
  end note
  destroy c
else [request is valid]
  activate c
  note over s
    """
    Command string is parsed successfully.
    One statement to be executed is identified.
    """
  end note
  s->c: QueryReceiptResponse
end

note over s
  "Server begins to execute the statement"
end note

loop [while has warning]
  s->c: WarningResponse
end

alt [statement generates an error]
  s->c: ErrorResponse
  note left of c
    "Note: Dialog continues"
  end note

else [statement generates a result set]
  s->c: RowMetadataResponse
  
  loop [while rows in result set]
    s->c: RowDataResponse
  end
  
  s->c: StatementResponse
  note left of c
    "End of statement execution"
  end note
  
else [statement succeeds but has no result set]
  s->c: StatementResponse
  
  note left of c
    "End of statement execution"
  end note
end

note over s
  """
  No more statements to execute.
  Server indicates the dialog is complete.
  """
end note

s->c: QueryExecuteResponse

note over (:---:) c, s
  "Dialog ends"
end note