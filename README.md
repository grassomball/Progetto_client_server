# Client-Server Password Generator

A client-server application that allows users to generate customized passwords via TCP requests. The server generates passwords of various types and lengths based on client requests.

## Features

- **Server**
  - Listens for TCP connections on a specific port.
  - Handles up to 5 simultaneous clients.
  - Generates passwords based on client specifications.
  - Provides 4 types of passwords:
    - **n**: Numeric only.
    - **a**: Lowercase alphabetic only.
    - **m**: Mixed (lowercase letters and numbers).
    - **s**: Secure (uppercase letters, lowercase letters, numbers, and symbols).
- **Client**
  - Automatically connects to the server.
  - Sends requests to generate passwords.
  - Displays the generated passwords.
  - Disconnects by sending the command `q`.

## Requirements

- **Libraries**:
  - On Windows: Winsock2 (`ws2_32.lib`).
  - On Linux/macOS: Standard sockets (`<sys/socket.h>`).
- **Compiler**: GCC or another compatible C compiler.
- ### Download and Setup

1. Download the repository as a ZIP file from GitHub.
2. Extract the ZIP file on your computer.
3. Open **Eclipse** and import the two projects (`client` and `server`) into your workspace.

### Compilation

#### Using Eclipse
- Make sure both `client` and `server` projects are imported into Eclipse.
- Build the projects using Eclipse's built-in build functionality.
- Then open two command prompt; one for server, one for client
  1) SERVER => Navigate to the server directory => cd c: path_unzip/server/Debug and after run the server => server.exe
  2) CLIENT => Navigate to the client directory => cd c:path_unzip/client/Debug and after run the client => client.exe
 
  N.B. <i> path_unzip = percorso dove è stata scompattato il progetto github in riferimento a client e server
