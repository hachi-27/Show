**Table of Contents**

- [General Information](#General Information)
  - [Ping-Pong/Keep Alive](#Ping-Pong/Keep-Alive)
  - [Websocket authentication](#Websocket-authentication)
- [Public events](#public-events)
  - [Market ticker change](#market-ticker-change)
  - [Market trades](#market-trades)
  - [Market order book](#market-order-book)
- [Private events](#private-events)
  - [Order change](#order-change)
  - [Trade change](#trade-change)
  - [Wallet balance change](#wallet-balance-change)

# General Information
* To get realtime wazirx socket events you need to connect to our websocket server
* You can use any websocket client library to connect to our server
* Our connection endpoint is - `wss://stream-internal.wazirx.com/stream`
* In each websocket request, you can send `id` as a string which is an optional parameters. We will return the same `id` string in the response for you to track the request/response
* You can subscribe/unsubscribe to multiple streams in one frame, by sending them all in array eg `{"event":"subscribe", "streams":["!ticker@arr","btcinr@depth","btcinr@trades"]}`

## Ping-Pong/Keep Alive
To keep the connection alive, user needs to send ping frame to server every 10 minutes. If the websocket server does not receive a ping frame from the connection within a 15 minute period, the connection will be disconnected

## Websocket authentication
The API client must request an authentication key via the following REST API endpoint to connect to WebSockets Private endpoints. The key should be used within 15 minutes of creation. The key does not expire once a connection to a WebSockets server is maintained.

Content-Type is  `application/x-www-form-urlencoded`

```
POST https://x.wazirx.com/api/v2/streams/create_auth_token
```

**Headers:**

Name | Type | Mandatory | Description
------------ | ------------ | ------------ | ------------
access-key | STRING | YES |
signature | STRING | YES |
tonce | LONG | YES |


## Public events
### Market ticker change
You will receive this event if there is any change in any market(s). 

**Stream Name:** !ticker@arr
**Update Speed:** 3000ms
**Payload:**
```json5
{
  "data":{
    "usdtinr": {
      "base_unit": "usdt",
      "quote_unit": "inr",
      "type": "SPOT",
      "volume": "292069.9",
      "open": "76.9",
      "high": "77.2",
      "low": "76.67",
      "buy": "76.67",
      "sell": "76.8",
      "last": "76.73",
      "at": 1592384186
    },
    "stream":"!ticker@arr"
  }
}
```
To get this data, send following frame on websocket - 
`{"event":"subscribe", "streams":["!ticker@arr"]}`
To stop receiving this data, send following frame on websocket -
`{"event":"unsubscribe", "streams":["!ticker@arr"]}`

### Market trades
You will receive this event whenever a new trade happens in the market. 

**Stream Name:** <market>@trades
**Update Speed:** Real-time
**Payload:**
```json5
{
  "data":{
    "trades":[{
        "amount":"0.0012", 
        "date":1629978336,
        "market":"btcinr",
        "price":"3675000.0",
        "tid":183483216,
        "type":"buy"
      }]
  },
  "stream":"btcinr@trades"
}
```
To get this data, send following frame on websocket -
`{"event":"subscribe", "streams":["btcinr@trades"]}`
To stop receiving this data, send following frame on websocket -
`{"event":"unsubscribe", "streams":["btcinr@trades"]}`

### Market order book
With this event you will receive full order book details for this market. 

**Stream Name:** <market>@depth
**Update Speed:** 3000ms
**Payload:**
```json5
{
  "data": {
    "asks": [
      [
        "9291.0",
        // PRICE
        "0.0119"
        // QTY
      ]
    ],
    "bids": [
      [
        "9253.0",
        // PRICE
        "1.0456"
        // QTY
      ]
    ],
    "timestamp":1629981000
  },
  "stream":"btcinr@depth"
}
```
To get this data, send following frame on websocket -
`{"event":"subscribe", "streams":["btcinr@depth"]}`
To stop receiving this data, send following frame on websocket -
`{"event":"unsubscribe", "streams":["btcinr@depth"]}`


## Private events
To subscribe to all the private events, you have to send the auth key in the socket message for authentication
Auth token is the one which is generated in `Websocket authentication` 

### Order change
You will receive this event whenever any order is created / updated / cancelled. 

**Stream Name:** orderUpdate
**Update Speed:** Real-time
**Payload:**
```json5
{
  "data": {
    "id": 12343382,
    "market": "btcusdt",
    "ord_type": "limit",
    "kind": "bid",
    "state": "wait",
    "origin_volume": "0.1",
    "price": "10.0",
    "avg_price": "10.0",
    "volume": "0.1",
    "at": 1592384186,
  },
  "stream":"orderUpdate"
}
```
To get this data, send following frame on websocket -
`{"event":"subscribe", "streams":["orderUpdate"], "auth_key":"your_auth_key"}`
To stop receiving this data, send following frame on websocket -
`{"event":"unsubscribe", "streams":["orderUpdate"]}`

### Trade change
You will receive this event whenever any of your open order is traded

**Stream Name:** ownTrade
**Update Speed:** Real-time
**Payload:**
```json5
{
  "data":{
    "at":1629980435,
    "fee":"0.1099996",
    "fee_currency":"inr",
    "funds":"54.9998",
    "id":17319337,
    "kind":"bid",
    "market":"btcinr",
    "order_id":26903215,
    "price":"118.0",
    "volume":"0.4661"
  },
  "stream":"ownTrade"
}
```
To get this data, send following frame on websocket -
`{"event":"subscribe", "streams":["ownTrade"], "auth_key":"your_auth_key"}`
To stop receiving this data, send following frame on websocket -
`{"event":"unsubscribe", "streams":["ownTrade"]}`


### Wallet balance change
You will receive this event whenever there is any change in wallet balance. 

**Stream Name:** balanceUpdate
**Update Speed:** Real-time
**Payload:**
```json5
{
  "data": {
    "id": 2092,
    "currency": "usdt",
    "balance": "2606.4",
    "locked": "87.1",
  },
  "stream":"balanceUpdate"
}
```
To get this data, send following frame on websocket -
`{"event":"subscribe", "streams":["balanceUpdate"], "auth_key":"your_auth_key"}`
To stop receiving this data, send following frame on websocket -
`{"event":"unsubscribe", "streams":["balanceUpdate"]}`
