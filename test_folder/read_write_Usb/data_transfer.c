#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<asm/uaccess.h>



#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x82
#define MAX_PKT_SIZE 512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static int pen_open(struct inode *i, struct file *f){
    return 0;
}

static int pen_close(struct inode *i, struct file *f){
    return 0;
}

static ssize_t pen_read(struct file *f, char __user *buf, size_t cnt, loff_t *ppos)
{
    int retval;
    int read_cnt;

    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN), bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
    printk(KERN_INFO "Inside Read function\n");

    if(retval){
        printk(KERN_INFO "Bulk message returned %d\n", retval);
        return retval;
    }
    if (raw_copy_to_user(buf, bulk_buf, MIN(cnt, read_cnt))){
        return -EFAULT;
    }
    
    return MIN(cnt, read_cnt);
}

static ssize_t pen_write(struct file *f, const char __user *buf, size_t cnt, loff_t *ppos){
    int retval;
    int wrote_cnt = MIN(cnt, MAX_PKT_SIZE);
    if(raw_copy_from_user(bulk_buf, buf,  MIN(cnt, MAX_PKT_SIZE))){
        return -EFAULT;
    }
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT), bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000);
    if(retval){
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
    printk(KERN_INFO "Inside write function\n");
    return wrote_cnt;

}

static struct file_operations fops = {
    .open = pen_open,
    .release = pen_close,
    .read = pen_read,
    .write = pen_write,
};


static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    int retval;
    device = interface_to_usbdev(interface);
    class.name = "usb/pen%d";
    class.fops = &fops;
    retval = usb_register_dev(interface, &class);
    if(retval <= 0 ){
        printk(KERN_INFO "Not able to get minor for this device");
    }
    else {
        printk(KERN_INFO "Minor obtained : %d\n", interface->minor);
        
    }
    
    return retval;
}
static void pen_disconnect(struct usb_interface *interface)
{

        printk(KERN_INFO "pendrive removed");
        usb_deregister_dev(interface, &class);
    
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
    int result;
    result = usb_register(&pen_driver);
    if(result){
        printk(KERN_INFO "Usb register failed error no %d", result);
    }
    return result;
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