# cnoc-simulator
A NoC simulator written in C

## Features

- 2d NoC
- DOR algorithm
- Store and Forward
- Debugging mode

## Compile

Clone the repository:
```bash
  git clone https://github.com/yellowtrr/cnoc-simulator.git
  cd cnoc-simulator
```
Either use make: 
```bash
  make
  ./cnoc-simulator
```
Or just compile:
```bash
 clang -Wall -O3 packet.c router.c stats.c simulation.c main.c -o cnoc-simulator
```
## Usage
```bash
 ./[executable] [size] [buffer_capacity] [injection_rate] [timesteps] [debuginfo]
```


- size: 2D-mesh size. For example, 3 for a 3x3 2D-mesh

- buffer_capacity: the amount of packets a buffer can store. 

- injection_rate: the probability each router will create a new packet at each new timestep.

- timesteps: total simulation steps.

- debuginfo: 1 for extra debug info, 0 for simple readable output.

#### Example:

```bash
 ./cnoc-simulator 3 3 0.4 250 1
```
Will run a simulation of a 3x3 network, with each buffer being able to store 3 packets, with an injection rate of 0.4 for 250 steps.

