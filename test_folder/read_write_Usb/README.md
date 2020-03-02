# Command Read and write data into pendrive
#### First go to working directory then 
- command: "make"
- command: remove usb-storage using "sudo rmmod uas usb_storage"
- command "sudo insmod data_transfer.ko"
#### Insert pendrive 

- command: "dmesg"
#### Now compile user program "userProgram.c"
- command: gcc -o "test" userProgram.c

#### Now it's time to read and write data check your  device list directory dev/pen0.
##### change the mode of pen0 device using "sudo chmod 777 pen0"
#### Now Run
- command: ./test

