from fastapi.responses import HTMLResponse


def http_endpoint_realtime_interactive(port: int, ip: str = "localhost"):
    html = f"""
<!DOCTYPE html>
<html>
    <head>
        <title>Chat</title>
    </head>
    <body>
        <h1>WebSocket Chat</h1>
        <form action="" onsubmit="sendMessage(event)">
            <label for="ipInput">IP:</label>
            <input type="text" id="ipInput" value="{ip}" style="width: 120px;" />
            <label for="messageTest">Message text:</label>
            <input type="text" id="messageText" autocomplete="off"/>
            <button>Send</button>
        </form>
        <ul id='messages'>
        </ul>
        <script>
            const ipInput = document.getElementById('ipInput');
            let ws;
            ipInput.addEventListener('change', function() {{
                if (ws) {{
                    ws.close();
                }}
                ws = new WebSocket("ws://" + ipInput.value + ":{port}/realtime");
                ws.onmessage = onMessage;
            }});
            function onMessage(event) {{
                var messages = document.getElementById('messages')
                var message = document.createElement('li')
                var content = document.createTextNode("RCVD: " + event.data)
                message.appendChild(content)
                messages.appendChild(message)
            }}
            function sendMessage(event) {{
                if (!ws) {{
                    throw new Error("ws not started")
                }}
                var input = document.getElementById("messageText")
                ws.send(input.value)
                // Add SENT message to history
                var messages = document.getElementById('messages')
                var message = document.createElement('li')
                var content = document.createTextNode("SENT: " + input.value)
                message.appendChild(content)
                messages.appendChild(message)
                input.value = ''
                event.preventDefault()
            }}
        </script>
    </body>
</html>
"""
    return lambda: HTMLResponse(html)
