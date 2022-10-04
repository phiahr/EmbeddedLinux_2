/* Character driver example */
/* June 23 2011 */
/* Sébastien Bilavarn */
/* Polytech'Nice Sophia / University of Nice Spophia Antipolis */

/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */

#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h> 
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/uaccess.h> /* copy_from/to_user */

#include <linux/ioport.h>
#include <asm/io.h>

#include "myleds.h"

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of myleds.c functions */
int myleds_open(struct inode *inode, struct file *filp);
int myleds_release(struct inode *inode, struct file *filp);
ssize_t myleds_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t myleds_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void myleds_exit(void);
int myleds_init(void);
long myleds_ioctl(struct file *filp, unsigned int ioctl_num, unsigned long ioctl_param); 

/* Structure that declares the usual file */
/* access functions */
struct file_operations myleds_fops = {
  .read = myleds_read,
  .write = myleds_write,
  .unlocked_ioctl = myleds_ioctl, 
  .open = myleds_open,
  .release = myleds_release
};

/* Declaration of the init and exit functions */
module_init(myleds_init);
module_exit(myleds_exit);

/* Global variables of the driver */
/* Buffer to store data */
char *myleds_buffer;
void *myleds_map;

int myleds_init(void) {
  int result;
  
  /* Registering device */
  result = register_chrdev(MYDEVICE_MAJOR, "myleds", &myleds_fops);
  if (result < 0) {
   printk(
   "<1>myleds: cannot obtain major number %d\n", MYDEVICE_MAJOR);
   return result;
  }
  /* Memory allocation */
  // myleds_buffer = 
  request_mem_region(XPAR_LEDS_8BIT_BASEADDR,8,"myleds");

  /* Allocating myleds for the buffer */
  myleds_buffer = kmalloc(MAX_BUFFER_SIZE, GFP_KERNEL); 
  if (!myleds_buffer) { 
   result = -ENOMEM;
   goto fail; 
  } 
  memset(myleds_buffer, 0, 1);
  
  printk("<1>Inserting myleds module\n"); 
  return 0;

  fail: 
   myleds_exit(); 
   return result;
}

void myleds_exit(void) {
  /* Freeing the major number */
  unregister_chrdev(MYDEVICE_MAJOR, "myleds");

  /* Freeing buffer myleds */
  if (myleds_buffer) {
   kfree(myleds_buffer);
    release_mem_region(XPAR_LEDS_8BIT_BASEADDR,8);
  }

  
  printk("<1>Removing myleds module\n");

}

int myleds_open(struct inode *inode, struct file *filp) {
  myleds_map = ioremap(XPAR_LEDS_8BIT_BASEADDR, 8);
  /* Success */
  return 0;
}

int myleds_release(struct inode *inode, struct file *filp) {
  iounmap(myleds_map);
  /* Success */
  return 0;
}

ssize_t myleds_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) { 

  int msg_length; 
	
  /* Transfering data to user space */ 
  copy_to_user(myleds_buffer, buf, MAX_BUFFER_SIZE);
  msg_length = strlen(myleds_buffer);
  ioread32(myleds_map);
  printk("myleds_READ: message sent to userspace -> %s\n", myleds_buffer);
	
  return count;
}

ssize_t myleds_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	
  /* Transfering data from user space */ 
  unsigned long err = copy_from_user(myleds_buffer, buf, count);
  unsigned long res;
  kstrtol(myleds_buffer,0, &res);
  printk("<1>myleds uncopied bytes are  %ld\n", err);
  printk("<1>myleds: led value is %ld\n", res);
  iowrite32(res, myleds_map);

  return count;

}

long myleds_ioctl(struct file *filp, unsigned int ioctl_num, unsigned long ioctl_param) {

  return 0;
}

