# simple-rootkit
A simple rootkit that loads a kernel module, hides the existense of executables, running process, and the loaded module itself.

## Compile and Run
In command line, type
```
make build
```
```
make
```

to run the rootkit:
```
./sneaky_process
```

You will be prompted to enter one key at a time. In the meantime, the sneaky kernel module will be installed and loaded. After that, the 'sneaky_process' executable will be invisible from the ```ls``` and ```find``` command. The process id will also disappear from the resulting list of ```ls /proc``` and ```ps -a -u <your_user_id>```. ```lsmod``` command will not include the 'sneaky_mod' module.
