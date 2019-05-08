# CS3103 FIFO Queue Simulator Assignment
## Introduction
This repository is created for `City University HK` course `CS3103 - Operating Systems` programming assignment 2 <br />
All credit goes to: Mark Lai

## Assignment Requirement
Aim to create a text base linux app which simulate a First-In-First-Out (FIFO) queue<br />
The model includes a flow, a pflow, a queue, and a server.<br />
The flow is responsible for generating traffic, measured in terms of token, into the queue.<br />
The pflow is responsible for generating traffic, measured in terms of token, into the queue when the queue is empty.<br />
The server tries to empty the queue by fetching a certain number of tokens, if any.<br />
<br />
Your SFIFO needs to accept two integer inputs, `MaxToken` as the ﬁrst and `flowInterval` as the second
```
$ ./SFIFO 500 2
```
### Flow
The flow wakes up periodically with an interval time of `flowInterval` seconds.<br />
Once waking up, the flow generates a random number of tokens, uniformly distributed in [1,10],<br />
and inserts the tokens to the queue. Each token is assigned a unique sequence number in the increasing order.<br />
We assume that the ﬁrst token has sequence number 0, and the sequence number will be increased by 1 for each consecutive token.<br />
We also assume that the flow, after it wakes up, remembers the history of tokens so that the sequence numbers of generated tokens are always increasing.<br />

### pFlow
The pflow wake up whenever the queue is emptied by the server<br />
Once waking up, it generates a random number of tokens, uniformly distributed in [1, 5],<br />
and inserts the tokens to the queue. Otherwise, it is blocked if the queue is not empty.<br />
<br />
If the flow wins the competition and inputs the tokens before the pflow,],<br />
the pflow will inject tokens into a non-empty queue.],<br />
Nevertheless, this special case is acceptable.],<br />

### Queue
The queue temporarily stores the token in the FIFO fashion.<br />
The maximum size of the queue is set to 50 tokens. If the queue is full, the incoming tokens will be dropped.<br />

### Server
The server wakes up periodically with an interval time of 2 seconds.<br />
Once waking up, the server fetches a random number of tokens, uniformly distributed in [1,20].<br />
If the generated random number is larger than the current number of tokens in the queue, all the tokens in the queue will be fetched.<br />

### End Condition
The simulation stops after `MaxToken` number of tokens have been served. A token is considered to be served if it is fetched from the queue by the server or it is dropped due to buﬀer overﬂow.
<br />

## Getting Started
1. Clone the repository

2. Compile the cpp file
```
$ g++ main.cpp -o SFIFO -pthread
```
3. Start the project
```
$ ./SFIFO
```
