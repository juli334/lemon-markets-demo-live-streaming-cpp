Live Streaming for lemon.markets

This is a small example about using the live streaming API from lemon.markets using C++.

This example will implement a basic stock ticker for the command line.
Quick Start

    Replace YOUR-API-KEY with your actual market data API key in main.cpp
    sudo docker build -t lemon-markets-demo-live-streaming-cpp .
    sudo docker run lemon-markets-demo-live-streaming-cpp

You should see something like this:

authentication response: {"token":"_aarfg.********","user_id":"********","expires_at":********}<br>
connecting to mqtt.ably.io<br>
subscribing to live data<br>
publishing what live data to stream<br>
waiting 30 seconds for any live data to arrive ...<br>
Message with token value 2 delivery confirmed<br>
Message arrived<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;topic: ********<br>
&nbsp;&nbsp;&nbsp;message: {"isin":"US0378331005","mic":"XMUN","t":1668170131000,"a":1429600,"a_v":140,"b":1429199,"b_v":140}<br>
Message arrived<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;topic: ********<br>
&nbsp;&nbsp;&nbsp;message: {"isin":"US0378331005","mic":"XMUN","t":1668170135711,"a":1429800,"a_v":140,"b":1429400,"b_v":140}<br>
Message arrived<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;topic: ********<br>
&nbsp;&nbsp;&nbsp;message: {"isin":"US0378331005","mic":"XMUN","t":1668170151000,"a":1429800,"a_v":140,"b":1429400,"b_v":140}<br>
Message arrived<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;topic: ********<br>
&nbsp;&nbsp;&nbsp;message: {"isin":"US0378331005","mic":"XMUN","t":1668170153252,"a":1429400,"a_v":140,"b":1428800,"b_v":140}<br>
Message arrived<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;topic: ********<br>
&nbsp;&nbsp;&nbsp;message: {"isin":"US0378331005","mic":"XMUN","t":1668170153934,"a":1430000,"a_v":140,"b":1429600,"b_v":140}<br>
Message arrived<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;topic: ********<br>
&nbsp;&nbsp;&nbsp;message: {"isin":"US0378331005","mic":"XMUN","t":1668170153908,"a":1430000,"a_v":140,"b":1428800,"b_v":140}<br>
disconnecting
