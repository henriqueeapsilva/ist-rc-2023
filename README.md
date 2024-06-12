# Computer Networks

## Project Specification
 
Project developed within the scope of the Computer Networks course of Instituto Superior Tecnico (IST).
The statement can be found in the root directory of this repository (statement.pdf).

> The goal of this project is to implement a simple auction platform.
> Users can open (host) an auction to sell some asset, and close it, as well list ongoing auctions and make bids.
> The development of the project requires implementing an Auction Server (AS) and a User Application (User).
> The AS and multiple User application instances are intended to operate simultaneously on different machines connected to the Internet.

## User Application

The program implementing the users of the auction platform (User) is invoked using:
`
./user [-n ASIP] [-p ASport]
`

- ASIP: IP address of the machine where the auction server (AS) runs (optional).
- ASport: Well-known port (TCP and UDP) where the AS accepts requests (optional, assumes the value 58000+GN if omitted).

#### User Commands
`
login UID password:
`
- Sends a message to the AS, using UDP, to confirm the ID, UID, and password of the user. Displays the result: successful login, incorrect login attempt, or new user registered.

`
logout:
`
- Sends a message to the AS, using UDP, asking to logout the currently logged-in user. Displays the result: successful logout, unknown user, or user not logged in.

`
unregister:
`
- Sends a message to the AS, using UDP, asking to unregister the currently logged-in user. A logout operation is also performed. Displays the result: successful unregister, unknown user, or incorrect unregister attempt.

`
exit:
`
- Requests to exit the User application. If a user is still logged in, informs the user to first execute the logout command. Otherwise, terminates the application. Local command, not involving communication with the AS.

`
open name asset_fname start_value timeactive:
`
- Establishes a TCP session with the AS and sends a message asking to open a new auction. Displays the result and the assigned auction identifier (AID).

`
close AID:
`
- Sends a message to the AS, using TCP, asking to close an ongoing auction. Displays the result.

`
myauctions or ma:
`
- Sends a message to the AS, using UDP, asking for a list of auctions started by the logged-in user. Displays the result.

`
mybids or mb:
`
- Sends a message to the AS, using UDP, asking for a list of auctions for which the logged-in user has placed a bid. Displays the result.

`
list or l:
`
- Sends a message to the AS, using UDP, asking for a list of auctions. Displays the result.

`
show_asset AID or sa AID:
`
- Establishes a TCP session with the AS and sends a message asking to receive the image file of the asset in sale for auction number AID. Displays the result.

`
bid AID value or b AID value:
`
- Sends a message to the AS, using TCP, asking to place a bid for auction AID of value value. Displays the result.

`
show_record AID or sr AID:
`
- Sends a message to the AS, using UDP, asking to see the record of auction AID. Displays the result.

### Auction Server

The program implementing the Auction Server (AS) is invoked with the command:
`
./AS [-p ASport] [-v]
`

    ASport: Well-known port where the AS server accepts requests, both in UDP and TCP (optional, assumes the value 58000+GN if omitted).
    If the -v option is set when invoking the program, it operates in verbose mode.

### Protocol (UDP Request) (Client-Server)

- `LIN <uid> <password>`
- `LOU <uid> <password>`
- `UNR <uid> <password>`
- `LMA <uid>`
- `LMB <uid>`
- `LST`
- `SRC <aid>`

### Protocol (UDP Reply) (Server-Client)

- `RLI <status>`
- `RLO <status>`
- `RUR <status>`
- `RMA <status>[ <aid> <state>]*`
- `RMB <status>[ <aid> <state>]*`
- `RLS <status>[ <aid> <state>]*`
- `RRC <status>
      [<host-uid> <auc-name> <fname> <start-value> <date> <time> <timeactive>]
      [B <bidder-uid> <bid-value> <bid-date> <bid-time> <bid-time-elapsed>]
      [E <end-date> <end-time> <end-elapsed-time>]`

### Protocol (TCP Request) (Client-Server)

- `OPA <uid> <password> <name> <start-value> <timeactive> <fname> <fsize> <fdata>`
- `CLS <uid> <password> <aid>`
- `SAS <aid>`
- `BID <uid> <password> <aid> <value>`

### Protocol (TCP Reply) (Server-Client)

- `ROA <status>[ aid]`
- `RCL <status>`
- `RSA <status>[ <fname> <fsize> <fdata>]`
- `RBD <status>`

### Auction Server File Structure
```
  (root)
  |- USERS
  |  |- (uid1)
  |  |  |- (uid1)_pass.txt     < password
  |  |  |- (uid1)_login.txt
  |  |  |- HOSTED
  |  |  |  |- (aid1).txt
  |  |  |  \- (aid2).txt
  |  |  \- BIDDED
  |  |     |- (aid3).txt
  |  |     \- (aid4).txt
  |  |- (uid2)
  |  \- (uid3)
  \- AUCTIONS
     |- (aid1)
     |  |- START_(aid1).txt    < uid name fname value timeactive datetime fulltime
     |  |- ASSET
     |  |  \- (asset_fname1)
     |  |- END_(aid1).txt      < datetime sec_time
     |  \- BIDS
     |     |- (bid_value1).txt < uid value datetime sec_time
     |     |- (bid_value2).txt
     |     \- (bid_value3).txt
     |- (aid2)
     \- (aid3)
```

### Auxiliary Files and Directories

- directory "assets": asset files to use in open command;
- files auction.c/auction.h: functions to validate the parameters of the various commands;
- files utils.c/utils.h: useful functions to read and write to files and sockets;
- files database.c/database.h: functions to manage the AS database;
- directory "output": only created by command show_asset, where it stores the downloaded asset files;

### Timeouts

- to change AS timeout, change the TIMEOUT macro in server.c
- to change User timeout, change the TIMEOUT macro in user.c
