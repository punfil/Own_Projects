// Validation
[<Literal>]
let server_down: bool = false
[<Literal>]
let wrong_timeout: bool = false

let max_buffer_size: int = 1500
let min_server_response_time: int = 10

type TransferBuffer = {
    data: string
    buffer_size: int
}

type validatedTransferBuffer = {
    data: string
    buffer_size: int
}

type Syn = {
    syn_id: int
    time_initiate: System.DateTime
    response_timeout: int
}

type ReceivedSynAck = {
    syn_id: int
    syn_ack_received: bool
}

type SentACK = {
    sent_ack: bool
}

type SentData = {
    data: validatedTransferBuffer
}

let validateTransferBuffer (buffer: TransferBuffer) =
    match buffer.data with
    | "" -> Error "Transfer buffer cannot be empty"
    | _ when buffer.buffer_size > max_buffer_size -> Error "More data than buffer size."
    | _ when buffer.buffer_size = 0 -> Error "Buffer size cannot be 0."
    | _ ->
        Ok {
            data = buffer.data
            buffer_size = buffer.buffer_size
        }

let sendSyn =
    let syn: Syn = {
        syn_id = System.Random().Next()
        time_initiate = System.DateTime.Now
        response_timeout = if wrong_timeout then 0 else min_server_response_time
    }

    if syn.response_timeout < min_server_response_time then
        Error "Expected smaller timeout than minimal server response time."
    else
        Ok syn

let receiveSynAck (syn: Syn) =
    let receivedSynAck: ReceivedSynAck = {
        syn_id = syn.syn_id
        syn_ack_received = if server_down = true then false else true
    }

    if not receivedSynAck.syn_ack_received then
        Error "SYN-ACK has not been received."
    else
        Ok receivedSynAck

let sendAck (received: ReceivedSynAck)=
    Ok {
        sent_ack = true
    }

let sendData (sentAck: SentACK) (buffer: validatedTransferBuffer) =
    if sentAck.sent_ack then
        Ok {
            data = buffer
        }
    else
        Error "ACK not sent"

let processTransferBind (buffer: TransferBuffer) =
    buffer
    |> validateTransferBuffer
    |> Result.bind (fun validatedBuffer ->
            sendSyn
            |> Result.bind receiveSynAck
            |> Result.bind sendAck
            |> Result.bind (fun ackResult -> sendData ackResult validatedBuffer))

let buffer: TransferBuffer = { data = "Some data"; buffer_size = 5 }

printfn "Transfer status for out of order bind: %A" (processTransferBind buffer)