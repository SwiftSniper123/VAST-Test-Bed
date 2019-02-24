# VAST-Test-Bed
Test bed for testing the system

# Installing SUMO
Binaries for SUMO can be found here:
https://sumo.dlr.de/wiki/Downloads#SUMO_-_Latest_Release_.28Version_1.1.0.29

# SUMO Socket Connection
This branch contains code for connecting with a running SUMO instance (GUI or cmd line) on port 1773.

In order to try it out, you must have SUMO binaries installed and be able to run the sumo command on the Command Prompt. 

Run the following command to initialize sumo. It will wait open until a client connects to it: 
```
sumo -c "Configuration file location" --remote-port 1337
```
Now, compile the VAST project. This should step the simulation 5,000 times and output should show on the Command Prompt giving info at each time step.

If sumo is not found running on the specified port and exception will be thrown during run-time. This may be an exception that can be handled appropriately so that the user will be notified sumo is not running, rather than just crashing the program.
