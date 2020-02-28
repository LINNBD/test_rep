## To find the information of your usb stick

### follow this command

<ul>
    <li> Remove Usb-storage using "sudo rmmod usb_storage" </li>
    <li> or use  modprobe -r uas usb_storage ( if usb_storage is used by uas) </li>
    <li> Find the usb devices product id and device id using lsusb set your product id and device id  </li>
    <li> then build this module  using make command </li>
    <li> then sudo insmod "module name" </li>
</ul>

