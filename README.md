# AODV-Network-Routing-Proj
A project for my Network Routing Project  based on ns3


# Running Steps
1. Copy the folders in the scratch folder of the ns3 simulation
2.Build ns3 using ./waf in ns3.33 folder
3.Run the command ./waf -run scratch/<folder_name>/<folder_name> --vis


# Notes
1. The Mob_2_test_final tests just the new GrouGauss Mobility model.
2. To make sure to make all the members private to public in the gauss-markov-mobility-model. h in the folder src/mobility/model and build it again ./waf

