# Live Streaming for lemon.markets

This is a small example about using the live streaming API from lemon.markets using C++.

This example will implement a basic stock ticker for the command line.

## Quick Start

1. Replace `YOUR-API-KEY` with your actual market data API key in `main.cpp`
2. sudo docker build -t lemon-markets-demo-live-streaming-cpp .
3. sudo docker run lemon-markets-demo-live-streaming-cpp

You should see something like this:

    authentication response: {"token":"_aarfg.********","user_id":"********","expires_at":********}
    connecting to mqtt.ably.io
    subscribing to live data
    publishing what live data to stream
    waiting 30 seconds for any live data to arrive ...
    Message with token value 2 delivery confirmed
    Message arrived
         topic: ********
       message: {"isin":"US0378331005","mic":"XMUN","t":1668170131000,"a":1429600,"a_v":140,"b":1429199,"b_v":140}
    Message arrived
         topic: ********
       message: {"isin":"US0378331005","mic":"XMUN","t":1668170135711,"a":1429800,"a_v":140,"b":1429400,"b_v":140}
    Message arrived
         topic: ********
       message: {"isin":"US0378331005","mic":"XMUN","t":1668170151000,"a":1429800,"a_v":140,"b":1429400,"b_v":140}
    Message arrived
         topic: ********
       message: {"isin":"US0378331005","mic":"XMUN","t":1668170153252,"a":1429400,"a_v":140,"b":1428800,"b_v":140}
    Message arrived
         topic: ********
       message: {"isin":"US0378331005","mic":"XMUN","t":1668170153934,"a":1430000,"a_v":140,"b":1429600,"b_v":140}
    Message arrived
         topic: ********
       message: {"isin":"US0378331005","mic":"XMUN","t":1668170153908,"a":1430000,"a_v":140,"b":1428800,"b_v":140}
    disconnecting
