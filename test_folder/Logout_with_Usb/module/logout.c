#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>

#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/string.h> 

static struct usb_device *device;

static struct kobject *example_kobject;
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    
       
   
    if(id->idVendor == 0x058f)
    {
     
       example_kobject = kobject_create_and_add("InsertedPen", kernel_kobj);
    
        printk(KERN_INFO "Doel inserted");
         if(!example_kobject) return -ENOMEM;
       
        
    }
    if(id->idVendor == 0x8564)
    {
        printk(KERN_INFO "Jetflash inserted");
    }
    device = interface_to_usbdev(interface);
    
    return 0;
}
static void pen_disconnect(struct usb_interface *interface)
{
       kobject_put(example_kobject);
            printk(KERN_INFO "pendrive removed");
    
}

static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(0x058f, 0x6387) },
    { USB_DEVICE(0x8564,0x1000) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table);

static struct usb_driver pen_driver =
{
    .name = "pen_driver",
    .id_table = pen_table,
    .probe = pen_probe,
    .disconnect = pen_disconnect,
};

static int __init pen_init(void)
{
    return usb_register(&pen_driver);
}

static void __exit pen_exit(void)
{
    usb_deregister(&pen_driver);
 
}

module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINNBD");
MODULE_DESCRIPTION("USB Pen Registration Driver");