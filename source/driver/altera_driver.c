#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define BUTTON 1
#define LED_G 2
#define LED_R 3
#define INPORT 4
#define HEXPORT 5
#define HEXDISPLAY_1 6


MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Basic Driver PCIHello");
MODULE_AUTHOR("Patrick Schaumont");

//-- Hardware Handles

static void *hexport;  // handle to 32-bit output PIO
static void *inport;   // handle to 16-bit input PIO
static void *button;   
static void *led_g;
static void *led_r;
static void *hexdisplay_1;

//-- Char Driver Interface
static int   access_count =  0;
static int   MAJOR_NUMBER = 91;

static int     char_device_open    ( struct inode * , struct file *);
static int     char_device_release ( struct inode * , struct file *);
static ssize_t char_device_read    ( struct file * , char *,       size_t , loff_t *);
static ssize_t char_device_write   ( struct file * , const char *, size_t , loff_t *);

static struct file_operations file_opts = {
      .read = char_device_read,
      .open = char_device_open,
     .write = char_device_write,
   .release = char_device_release
};

static int char_device_open(struct inode *inodep, struct file *filep) {
   access_count++;
   printk(KERN_ALERT "altera_driver: opened %d time(s)\n", access_count);
   return 0;
}

static int char_device_release(struct inode *inodep, struct file *filep) {
   printk(KERN_ALERT "altera_driver: device closed.\n");
   return 0;
}

static ssize_t char_device_read(struct file *filep, char *buf, size_t len, loff_t *off) {
  
  long int buttonData, switchesData;
  size_t count = len;
  if(len == BUTTON){
  	buttonData = ioread32(button);
      put_user(buttonData & 0xFF, buf++);
      put_user((buttonData >> 8) & 0xFF, buf++);
      put_user((buttonData >> 16) & 0xFF, buf++);
      put_user((buttonData >> 24) & 0xFF, buf++);
  
  }else if(len == INPORT){
  	 switchesData = ioread32(inport);
      put_user(switchesData & 0xFF, buf++);
      put_user((switchesData >> 8) & 0xFF, buf++);
      put_user((switchesData >> 16) & 0xFF, buf++);
      put_user((switchesData >> 24) & 0xFF, buf++);
  }
  
    
  return count;
}

static ssize_t char_device_write(struct file *filep, const char *buf, size_t len, loff_t *off) {
  char *ptr = (char *) buf;
  size_t count = len;

  unsigned k = *((int *) ptr);//pega o valo ptr

  switch(len){
    case(LED_G):
      iowrite32(k, led_g);
      break;
    case(LED_R):
      iowrite32(k, led_r);
      break;
    case(HEXPORT):
      iowrite32(k, hexport);
      break;
    case(HEXDISPLAY_1):
      iowrite32(k, hexdisplay_1);
      break;
    default:
      break;
  }

//  short b = 0;
  //  printk(KERN_ALERT "altera_driver: write %d bytes\n", len);
  //while (b <  len) {
    
    //    ptr += 4;//
    //b   += 4;
    
  //}
  return k;
}

//-- PCI Device Interface

static struct pci_device_id pci_ids[] = {
  { PCI_DEVICE(0x1172, 0x0004), },
  { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void pci_remove(struct pci_dev *dev);

static struct pci_driver pci_driver = {
  .name     = "alterahello",
  .id_table = pci_ids,
  .probe    = pci_probe,
  .remove   = pci_remove,
};

static unsigned char pci_get_revision(struct pci_dev *dev) {
  u8 revision;

  pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
  return revision;
}

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id) {
  int vendor;
  int retval;
  unsigned long resource;

  retval = pci_enable_device(dev);
  
  if (pci_get_revision(dev) != 0x01) {
    printk(KERN_ALERT "altera_driver: cannot find pci device\n");
    return -ENODEV;
  }

  pci_read_config_dword(dev, 0, &vendor);
  printk(KERN_ALERT "altera_driver: Found Vendor id: %x\n", vendor);

  resource = pci_resource_start(dev, 0);
  printk(KERN_ALERT "altera_driver: Resource start at bar 0: %lx\n", resource);

  hexport = ioremap_nocache(resource + 0XC200, 0x20);
  inport  = ioremap_nocache(resource + 0XC020, 0x20);
  button  = ioremap_nocache(resource + 0XC700, 0x20);
  led_g = ioremap_nocache(resource + 0XC060, 0x20);
  led_r = ioremap_nocache(resource + 0XC250, 0x20);
  hexdisplay_1 = ioremap_nocache(resource + 0XC040, 0x20);

  return 0;
}


static void pci_remove(struct pci_dev *dev) {
  iounmap(hexport);
  iounmap(inport);
  iounmap(button);
  iounmap(led_g);
  iounmap(led_r);
  iounmap(hexdisplay_1);
}


//-- Global module registration

static int __init altera_driver_init(void) {
   int t = register_chrdev(MAJOR_NUMBER, "de2i150_altera", &file_opts);
   t = t | pci_register_driver(&pci_driver);

   if(t<0)
      printk(KERN_ALERT "altera_driver: error: cannot register char or pci.\n");
   else
     printk(KERN_ALERT "altera_driver: char+pci drivers registered.\n");

   return t;
}

static void __exit altera_driver_exit(void) {
  printk(KERN_ALERT "Goodbye from de2i150_altera.\n");

  unregister_chrdev(MAJOR_NUMBER, "de2i150_altera");
  pci_unregister_driver(&pci_driver);
}

module_init(altera_driver_init);
module_exit(altera_driver_exit);
