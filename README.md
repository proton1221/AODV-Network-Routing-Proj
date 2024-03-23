# AODV-Network-Routing-Proj
A project for my Network Routing Project  based on ns3


# Running Steps
1. Copy the folders in the scratch folder of the ns3 simulation
2.Build ns3 using ./waf in ns3.33 folder
3.Run the command ./waf -run scratch/<folder_name>/<folder_name> --vis

# To generate flowmon files
1. Run each folder once
2. Change the tr_name in the folder by adding a "-150" in it and changing the nWifis in it from 75 to 150.
3. Give the flowmon file names in flow.py to the variables in it written as "_75 and _150".
4. A ".png" file will be generated for the same for showing plots.


# Notes
1. The Mob_2_test_final tests just the new GrouGauss Mobility model.
2. To make sure to make all the members private to public in the gauss-markov-mobility-model. h in the folder src/mobility/model and build it again ./waf



# Paramaters of the dense-fest - Scenario -1 

| Parameter                 | Description                                             | Value/Type                                                |
|---------------------------|---------------------------------------------------------|-----------------------------------------------------------|
| Simulation Time           | Total duration of the simulation                        | 75 seconds                                                |
| Start-Up Time             | Duration before data applications start                 | 50 seconds (implied, based on application start time)     |
| Number of Nodes           | Total number of nodes in the simulation                 | 100                                                       |
| Number of Sinks           | Number of source/sink pairs                             | 10                                                        |
| Mobility Model            | Mobility model used for node movement                   | GaussMarkovMobilityModel                                  |
| Node Speed                | Maximum node speed in mobility model                    | 2 m/s                                                     |
| Pause Time                | Pause time for node movement in mobility model          | 0-5 seconds (min-max, random)                             |
| WiFi Standard             | The WiFi standard used                                  | 802.11b                                                   |
| Transmission Power        | Transmit power for WiFi devices                         | 7.5 dBm                                                   |
| Data Rate                 | Application data rate                                   | 2048 bps                                                  |
| Physical Mode             | Physical layer transmission mode                        | DsssRate11Mbps                                            |
| Routing Protocol          | Network layer routing protocol                          | AODV (default), OLSR, DSDV, DSR selectable                |
| Area                      | Area of movement for the nodes                          | 300x1500 m                                                |
| Packet Size               | Size of packets sent by applications                    | 64 bytes                                                  |
| On/Off Application Rate   | On period rate for generating packets                   | Constant 1.0                                              |
| Protocol                  | Protocol number (for selecting routing protocol)        | 2 (AODV by default)                                       |
| On/Off Application        | Type of application used to generate traffic            | OnOffApplication with UDP                                 |


# Parametes of Scenario 2

| Parameter                   | Description                                                 | Value/Type                                                |
|-----------------------------|-------------------------------------------------------------|-----------------------------------------------------------|
| Simulation Time             | Total duration of the simulation                            | 100 seconds                                               |
| Number of Nodes             | Total number of nodes in the simulation                     | 50                                                        |
| Mobility Model              | Mobility model used for node movement                       | RandomWaypointMobilityModel with custom parameters        |
| Node Speed                  | Speed range for nodes in the mobility model                 | Uniform Random Variable [Min=0.0, Max=20.0] m/s          |
| Pause Time                  | Pause time for node movement in the mobility model          | Uniform Random Variable [Min=0.0, Max=30.0] seconds       |
| Simulation Area             | Area in which nodes move                                    | 1000.0 x 1000.0 meters                                    |
| WiFi Standard               | The WiFi standard used                                      | 802.11b                                                   |
| Transmission Power          | Transmit power for WiFi devices                             | 7.5 dBm                                                   |
| Data Rate                   | Application data rate                                       | 2048 bps                                                  |
| Physical Mode               | Physical layer transmission mode                            | DsssRate11Mbps                                            |
| Routing Protocol            | Network layer routing protocol                              | AODV (default), selectable to OLSR, DSDV, DSR             |
| Number of Sinks             | Number of source/sink pairs sending data                    | 10                                                        |
| Packet Size                 | Size of packets sent by applications                        | 64 bytes                                                  |
| Trace Mobility              | Enable/disable mobility tracing                             | False                                                     |
| Transmission Range          | Implied by TX power and propagation loss model, not set     | Depends on TX power and Friis propagation loss model      |
| On/Off Application          | Type of application used to generate traffic                | OnOffApplication with UDP                                 |



# Parameters of GroupGauss -Scenario- 3

| Parameter                 | Description                                        | Value/Type                                 |
|---------------------------|----------------------------------------------------|--------------------------------------------|
| Simulation Time           | Total duration of the simulation                   | 100 seconds                                |
| Start-Up Time             | Duration before data applications start            | 50 seconds (inferred)                      |
| Number of Nodes           | Total number of nodes in the simulation            | 75                                         |
| Number of Sinks           | Number of source/sink pairs                        | 10                                         |
| Mobility Model            | Mobility model used for node movement              | GroupGaussMarkovMobilityModel              |
| Mean Velocity             | Mean velocity of nodes within the group            | Uniform Random Variable [Min=0, Max=10] m/s|
| Alpha                     | Level of randomness in the mobility model          | 0.8                                        |
| TimeStep                  | Time step for mobility model updates               | 0.8 seconds                                |
| Bounds                    | Simulation area for the mobility model             | Box (0, 1000, 0, 1000, 0, 1000)            |
| NumberOfGroupNodes        | Number of nodes in each group for the mobility model| 5                                         |
| Stickiness                | Stickiness parameter for the group mobility model  | 0.5                                        |
| WiFi Standard             | The WiFi standard used                             | 802.11b                                    |
| Transmission Power        | Transmit power for WiFi devices                    | 5 dBm                                      |
| Rate                      | Application data rate                              | 2048 bps                                   |
| Physical Mode             | Physical layer transmission mode                   | DsssRate11Mbps                             |
| Routing Protocol          | Network layer routing protocol                     | AODV                                       |
| Area                      | Area of movement for the nodes                     | 300x1500                                   |
| Packet Size               | Size of packets sent by applications               | 64 bytes                                   |
| On/Off Application        | Type of application used to generate traffic       | OnOffApplication with UDP                  |









