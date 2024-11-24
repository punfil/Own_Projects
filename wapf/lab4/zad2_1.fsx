[<Literal>]
let max_buffer_size: int = 1500
[<Literal>]
let min_server_response_time: int = 10

// Validation
[<Literal>]
let server_down: bool = true
[<Literal>]
let wrong_timeout: bool = false

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

let validateTransferBuffer (buffer: TransferBuffer) : Result<validatedTransferBuffer, string> =
    match buffer.data with
    | "" -> Error "Transfer buffer cannot be empty"
    | _ when buffer.buffer_size > max_buffer_size -> Error "More data than buffer size."
    | _ when buffer.buffer_size = 0 -> Error "Buffer size cannot be 0."
    | _ ->
            let vBuffer: validatedTransferBuffer = {
                data = buffer.data
                buffer_size = buffer.buffer_size
            }
            Ok vBuffer


let sendSyn =
    let syn: Syn = {
            syn_id = 1
            time_initiate = System.DateTime.Now
            response_timeout = if wrong_timeout then 0 else min_server_response_time
    }

    if syn.response_timeout < min_server_response_time then
        Error "Expected smaller timeout than minimal server response time."
    else
        Ok syn

let receiveSynAck (syn: Syn) : Result<ReceivedSynAck, string> =
    let receivedSynAck: ReceivedSynAck = {
        syn_id = syn.syn_id
        syn_ack_received = if server_down = true then false else true
    }

    if receivedSynAck.syn_ack_received = false then
        Error "SYN-ACK has not been received."
    else
        Ok receivedSynAck

let sendAck (received: ReceivedSynAck) : Result<SentACK, string> =
    let sentAck: SentACK = {
        sent_ack = true
    }

    Ok sentAck

let sendData (sent: SentACK) (buffer: validatedTransferBuffer): Result<SentData, string> =
    let sentData: SentData = { data = buffer }
    Ok sentData

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
        let! validatedSyn = sendSyn
        let! validatedReceivedSyncAck = receiveSynAck validatedSyn
        let! sentAck = sendAck validatedReceivedSyncAck
        return sendData sentAck validatedTransferBuffer
    }

let buffer: TransferBuffer = { data = "Some data"; buffer_size = 5 }

printfn "Transfer status for out of order builder: %A" (resultBuilderExpression buffer)