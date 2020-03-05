#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/reboot.h>

static struct usb_device *device;


static struct task_struct *stop_thread;
//static ktime_t bed_time;
static bool armed;

static int stop_pc(void *unused)
{
    while (!kthread_should_stop()) {
        if (armed) {
            pr_err("pendriver removed so start");
            kernel_power_off();
        }
        ssleep(1);
    }
    return 0;
}
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;
    iface_desc = interface->cur_altsetting;

    printk(KERN_INFO "Pen drive  i/f %d  and (%04X:%04X) plugged\n",iface_desc->desc.bInterfaceNumber, id->idVendor,
                                id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceclass: %02X\n", iface_desc->desc.bInterfaceClass);
    for(i=0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;
        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",i, endpoint->wMaxPacketSize, endpoint-> wMaxPacketSize);
        
    }
       
   
    if(id->idVendor == 0x058f)
    {
        printk(KERN_INFO "Doel inserted");
       // armed = true;
        
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

        printk(KERN_INFO "pendrive removed");
        armed = true;
    
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
    int ret;
    stop_thread = kthread_run(stop_pc,NULL, "pc_control");
    if (IS_ERR(stop_thread)) {
        ret = PTR_ERR(stop_thread);
    }
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