[<Literal>]
let max_buffer_size: int = 1500
[<Literal>]
let min_server_response_time: int = 10

// Validation
[<Literal>]
let server_down: bool = false
[<Literal>]
let wrong_timeout: bool = false

let (>>=) result switch =
    match result with
    | Ok s -> switch s
    | Error f -> Error f

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
    data: validatedTransferBuffer
}

type SynWithoutdata = {
    syn_id: int
    time_initiate: System.DateTime
    response_timeout: int
}

type ReceivedSynAck = {
    syn_id: int
    data: validatedTransferBuffer
    syn_ack_received: bool
}

type ReceivedSynAckWithoutData = {
    syn_id: int
    syn_ack_received: bool
}

type SentACK = {
    data: validatedTransferBuffer
    sent_ack: bool
}

type SentACKWithoutData = {
    sent_ack: bool
}

type SentData = {
    data: validatedTransferBuffer
}

let (|ValidBuffer|InvalidBuffer|) (buffer: TransferBuffer) =
    if buffer.data = "" then InvalidBuffer "Transfer buffer cannot be empty"
    elif buffer.buffer_size > max_buffer_size then InvalidBuffer "More data than buffer size."
    elif buffer.buffer_size = 0 then InvalidBuffer "Buffer size cannot be 0."
    else ValidBuffer buffer


let validateTransferBuffer (buffer: TransferBuffer) : Result<validatedTransferBuffer, string> =
    match buffer with
    | ValidBuffer validBuffer ->
        let vBuffer: validatedTransferBuffer = {
            data = validBuffer.data
            buffer_size = validBuffer.buffer_size
        }
        Ok vBuffer
    | InvalidBuffer errorMsg -> Error errorMsg

let (|ValidTimeout|InvalidTimeout|) (timeout: int) =
    if timeout < min_server_response_time then
        InvalidTimeout "Expected smaller timeout than minimal server response time."
    else
        ValidTimeout timeout

let sendSyn (validatedBuffer: validatedTransferBuffer) =
    let syn: Syn = {
        syn_id = 1
        time_initiate = System.DateTime.Now
        response_timeout = if wrong_timeout then 0 else min_server_response_time
        data = validatedBuffer
    }

    match syn.response_timeout with
    | ValidTimeout validTimeout -> 
        Ok syn
    | InvalidTimeout errorMsg -> 
        Error errorMsg

let sendSynWithoutData =
    let syn: SynWithoutdata = {
            syn_id = 1
            time_initiate = System.DateTime.Now
            response_timeout = if wrong_timeout then 0 else min_server_response_time
    }

    match syn.response_timeout with
    | ValidTimeout validTimeout -> 
        Ok syn
    | InvalidTimeout errorMsg -> 
        Error errorMsg

let (|SynAckReceived|SynAckNotReceived|) (receivedsynAck: bool) =
    if receivedsynAck then
        SynAckReceived
    else
        SynAckNotReceived "SYN-ACK has not been received."

let receiveSynAck (syn: Syn) : Result<ReceivedSynAck, string> =
    let receivedSynAck: ReceivedSynAck = {
        syn_id = syn.syn_id
        data = syn.data
        syn_ack_received = not server_down
    }

    match receivedSynAck.syn_ack_received with
    | SynAckReceived ->
        Ok receivedSynAck
    | SynAckNotReceived errorMsg ->
        Error errorMsg

let receiveSynAckWithoutData (syn: SynWithoutdata) : Result<ReceivedSynAckWithoutData, string> =
    let receivedSynAck: ReceivedSynAckWithoutData = {
        syn_id = syn.syn_id
        syn_ack_received = not server_down
    }

    match receivedSynAck.syn_ack_received with
    | SynAckReceived ->
        Ok receivedSynAck
    | SynAckNotReceived errorMsg ->
        Error errorMsg


let sendAck (received: ReceivedSynAck) : Result<SentACK, string> =
    let sentAck: SentACK = {
        data = received.data
        sent_ack = true
    }

    Ok sentAck

let sendAckWithoutData (received: ReceivedSynAckWithoutData) : Result<SentACKWithoutData, string> =
    let sentAck: SentACKWithoutData = {
        sent_ack = true
    }

    Ok sentAck

let sendData (sent: SentACK) : Result<SentData, string> =
    let sentData: SentData = { data = sent.data }
    Ok sentData

let sendDataBufferSeparate (sent: SentACKWithoutData) (buffer: validatedTransferBuffer) =
    let sentData: SentData = { data = buffer }
    Ok sentData

let processTransferBind data =
    data
    |> validateTransferBuffer
    |> Result.bind sendSyn
    |> Result.bind receiveSynAck
    |> Result.bind sendAck
    |> Result.bind sendData

let processTransferOperator data =
    validateTransferBuffer data
    >>= sendSyn
    >>= receiveSynAck
    >>= sendAck
    >>= sendData

type ResultBuilder() =
    member this.Bind (x: Result<'c,'d>, f) =
        match x with
        | Ok a -> f a
        | Error err -> Error err

    member this.Return (x: 'a)=
        Result.Ok x

let resultBuilder: ResultBuilder = new ResultBuilder()

let resultBuilderExpression (data: TransferBuffer) =
    resultBuilder {
        let! validatedTransferBuffer = data |> validateTransferBuffer
        let! validatedSyn = sendSyn validatedTransferBuffer
        let! validatedReceivedSyncAck = receiveSynAck validatedSyn
        let! sentAck = sendAck validatedReceivedSyncAck
        return sendData sentAck
    }

let processTransferBindOOO (buffer: TransferBuffer) =
    buffer
    |> validateTransferBuffer
    |> Result.bind (fun validatedBuffer ->
            sendSynWithoutData
            |> Result.bind receiveSynAckWithoutData
            |> Result.bind sendAckWithoutData // Opcjonalnie można by tutaj zrobić Result.map fun (ackResult -> (ackResult, validatedBuffer)), dłuższe o linijkę.
            |> Result.bind (fun ackResult -> sendDataBufferSeparate ackResult validatedBuffer))

let resultBuilderExpressionOOO (data: TransferBuffer) =
    resultBuilder {
        let! validatedTransferBuffer = data |> validateTransferBuffer
        let! validatedSyn = sendSynWithoutData
        let! validatedReceivedSyncAck = receiveSynAckWithoutData validatedSyn
        let! sentAck = sendAckWithoutData validatedReceivedSyncAck
        return sendDataBufferSeparate sentAck validatedTransferBuffer
    }

let buffer: TransferBuffer = { data = "Some data"; buffer_size = 5 }

printfn "Transfer status for bind: %A" (processTransferBind buffer)
printfn "Transfer status for operator: %A" (processTransferOperator buffer)
printfn "Transfer status for builder: %A" (resultBuilderExpression buffer)
printfn "Transfer status for out of order bind: %A" (processTransferBindOOO buffer)
printfn "Transfer status for out of order builder: %A" (resultBuilderExpression buffer)