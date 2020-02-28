#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>


static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "Pen drive (%04X:%04X) plugged\n", id->idVendor,
                                id->idProduct);
    if(id->idVendor == 0x058f)
    {
        printk(KERN_INFO "Doel inserted");
        
    }
    if(id->idVendor == 0x8564)
    {
        printk(KERN_INFO "Jetflash inserted");
    }
    
    return 0;
}
static void pen_disconnect(struct usb_interface *interface)
{

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