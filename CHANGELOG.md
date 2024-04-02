# Changelog

## How to execute the program
In terms of executing the program the user should use below command: 

```
./ipk24chat-client -t <protocol> -p <port> -s <host>
```

Where `host` refers to the IPv4 address of the server, `port` denotes the server port, and `protocol` can be either "tcp" or "udp". The order of the flags is arbitrary, and the flags are case-insensitive.

Executing the program with the `--help/-?` flag will display the help information.


## TODO

- [x] Write the udp_read() PacketHandler functions
- [x] Write the tcp_read() PacketHandler functions
- [x] Write documentation for more tests.
- [x] Fixed ip address lookup for host url.
- [x] Fixed network byte order from little endian to big endian.
- [x] Update the README file with proper documentation.
- [x] Fixed bug related to ncurses window.
