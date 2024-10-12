# BeatBeat
BeatBeat is a straightforward TCP heartbeat system implemented in C++. It features a server that sends random heartbeat numbers to clients, which must respond with their own random numbers. If the server does not receive a response within a specified timeout, it disconnects the client.

## Features

- Server sends random heartbeat numbers to clients at regular intervals.
- Clients receive heartbeats and respond with their own random numbers.
- Automatic disconnection of clients if no response is received within the timeout period.

## Showcase
![image](https://github.com/user-attachments/assets/5e157190-d4d3-4377-a3e3-60b30efbf0b5)


## License
BeatBeat is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
