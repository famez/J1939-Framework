### J1939 Dissector for wireshark

#### To install the plugin

- Install the J1939 framework as explained in the README.md at the top level of the repository.
- Download and extract the source code of wireshark located in https://www.wireshark.org/download/src/
- Copy the folder in which this README.md file is located into the following path inside the wireshark source code (creating the folder j1939): **plugins/epan/j1939/**
- Modify the file CMakeListsCustoms.txt located in the root of the wireshark source code as follows:

```
set(CUSTOM_PLUGIN_SRC_DIR
plugins/epan/j1939
)
```


- Configure (using **CMake**), build and install wireshark from the source code:
```bash
cd **wireshark source code**
cmake .
cd plugins/epan/j1939/
make && sudo make install
```



![alt text](https://github.com/famez/J1939-Framework/blob/master/wireshark/dissector/J1939-plugin.png)
