# BeatBeat
BeatBeat is a straightforward TCP heartbeat system implemented in C++. It features a server that sends random heartbeat numbers to clients, which must respond with their own random numbers. If the server does not receive a response within a specified timeout, it disconnects the client.

## Features

- Challenge-Response Mechanism: The server sends random heartbeat challenges to each client, which must reply with their own random numbers. </br>
- Periodic Heartbeats: The server sends heartbeat challenges at regular intervals to ensure active communication. </br>
- Automatic Disconnection: Clients are automatically disconnected if the server does not receive a response within the defined timeout period.

## Showcase
![image](https://github.com/user-attachments/assets/5e157190-d4d3-4377-a3e3-60b30efbf0b5)


## License
BeatBeat is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
