Blockchain based peer-to-peer network
=
This project is a blockchain based distributed peer-to-peer network. Clients can connect to any peer in the network and send transactions. They can also query for the blockchain and read it.
We have also implemented fault tolerance and recovery for any peer node in the network. This project is intended to show how real life blockchains reach consensus over a distributed network. 
The project uses SHA256 algorithm from open source and is written entirely in C++11.

Features implemented
-
- Peer-to-peer server client network architecture
- Blockchain copy on each server that can be read by the client
- Fault tolerance and recovery for the blockchain.

How to run
-
- Make sure to have a C++11 based compiler installed on the machine and the machine should be using Linux.
- Clone the repo.
- Go to src folder
  `cd src`
- Compile the executables:  
  `make all`
- Run the server program on the machine which will host the server, for example let's suppose the three servers have ip 10.200.125.71/72/73 and port is set to 11111, then the commands needed would be:
  * On host 10.200.125.71  
    `./server 11111 2 10.200.125.72 11111 10.200.125.73 11111`
  * On host 10.200.125.72  
    `./server 11111 2 10.200.125.71 11111 10.200.125.73 11111`
  * On host 10.200.125.73  
    `./server 11111 2 10.200.125.71 11111 10.200.125.72 11111`
- Send some transactions from the client along with unique client id (to make sure transactions are distinct). From the client machine(which is on the same network as other machines) run the
  following command to send 10 transactions worth 500 to host ip 10.200.125.72 at port 11111 with unique client id 1:  
  `./client 10.200.125.72 11111 1 0 10 500`
- Wait for some time(although block generation takes place quickly). Then run the following commands to check the status of the blockchain on all three servers:  
  `./client 10.200.125.71 11111 1 1`  
  `./client 10.200.125.72 11111 1 1`  
  `./client 10.200.125.72 11111 1 1`  
- This should give the status of the blockchain on all three servers. This shows that even though transaction was sent to server 10.200.125.72 only, it got added to the blockchain copies in all 3 servers.
  Hence consensus is achieved.

Fault tolerance and recovery
-
Our implementation is highly fault tolerant. Try crashing some of the peer servers, sending transactions to the network, then restarting the failed server program. Then query the previously failed server
to see it's blockchain status. It should be consistent with atleast one other node in the network. This shows the fault tolerance and recovery aspects of our system.
