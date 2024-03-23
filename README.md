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

