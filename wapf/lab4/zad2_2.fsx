// Validation
[<Literal>]
let server_down: bool = true
[<Literal>]
let wrong_timeout: bool = false

type ResultWithState<'T, 'State> =
    | Ok of 'T * 'State
    | Error of string

let bind f result =
    match result with
    | Ok (value, state) -> f value state
    | Error err -> Error err

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

let validateTransferBuffer (buffer: TransferBuffer) : ResultWithState<validatedTransferBuffer, string> =
    match buffer.data with
    | "" -> Error "Transfer buffer cannot be empty"
    | _ when buffer.buffer_size > max_buffer_size -> Error "More data than buffer size."
    | _ when buffer.buffer_size = 0 -> Error "Buffer size cannot be 0."
    | _ ->
        let vBuffer: validatedTransferBuffer = { data = buffer.data; buffer_size = buffer.buffer_size }
        Ok (vBuffer, "Buffer validated")

let sendSyn : ResultWithState<Syn, string> =
    let syn: Syn = {
        syn_id = 1
        time_initiate = System.DateTime.Now
        response_timeout = if wrong_timeout then 0 else min_server_response_time
    }

    if syn.response_timeout < min_server_response_time then
        Error "Expected smaller timeout than minimal server response time."
    else
        Ok (syn, "SYN sent")

let receiveSynAck (syn: Syn) : ResultWithState<ReceivedSynAck, string> =
    let receivedSynAck: ReceivedSynAck = {
        syn_id = syn.syn_id
        syn_ack_received = if server_down = true then false else true
    }

    if not receivedSynAck.syn_ack_received then
        Error "SYN-ACK has not been received."
    else
        Ok (receivedSynAck, "SYN-ACK received")

let sendAck (received: ReceivedSynAck) : ResultWithState<SentACK, string> =
    let sentAck: SentACK = { sent_ack = true }
    Ok (sentAck, "ACK sent")

let sendData (sentAck: SentACK) (buffer: validatedTransferBuffer) : ResultWithState<SentData, string> =
    if sentAck.sent_ack then
        let sentData: SentData = { data = buffer }
        Ok (sentData, "Data sent")
    else
        Error "ACK not sent"

let processTransferBind (buffer: TransferBuffer) =
    let initialResult = validateTransferBuffer buffer
    bind (fun validatedBuffer state1 ->
        let synResult = sendSyn
        bind (fun syn state2 ->
            let receiveResult = receiveSynAck syn
            bind (fun receivedSynAck state3 ->
                let ackResult = sendAck receivedSynAck
                bind (fun sentAck state4 ->
                    sendData sentAck validatedBuffer
                ) ackResult
            ) receiveResult
        ) synResult
    ) initialResult

let buffer: TransferBuffer = { data = "Some data"; buffer_size = 5 }

printfn "Transfer status for builder: %A" (processTransferBind buffer)