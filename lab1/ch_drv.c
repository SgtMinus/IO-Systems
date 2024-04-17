#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define BUF_SIZE 256

static dev_t first;
static struct cdev c_dev;
static struct class *cl;

char ibuf[BUF_SIZE];
char rbuf[BUF_SIZE];
int rbuf_off = 0;

static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}

static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static int process(const char *str)
{

  int size = strlen(str);
  int res;
  char operator= ' ';
  int operatorIndex = 0;
  int operand_1 = 0;
  int operand_2 = 0;
  int i = 0;
  for (i = 0; i < size; i++)
  {
    if (str[i] >= '0' && str[i] <= '9')
      continue;
    else
    {
      operator= str[i];
      operatorIndex = i;
      break;
    }
  }

  for (i = 0; i < operatorIndex; i++)
  {
    operand_1 *= 10;
    operand_1 += str[i] - '0';
  }
  for (i = operatorIndex + 1; i < size; i++)
  {
    if (str[i] >= '0' && str[i] <= '9')
    {
      operand_2 *= 10;
      operand_2 += str[i] - '0';
    } else break;
  }

  switch (operator)
  {
  case '+':
    res = operand_1 + operand_2;
    break;
  case '-':
    res = operand_1 - operand_2;
    break;
  case '*':
    res = operand_1 * operand_2;
    break;
  case '/':
    res = operand_1 / operand_2;
    break;
  default:
    printk(KERN_ERR "Invalid operation!");
  }

  printk(KERN_INFO "Op1 is  %d\n", operand_1);
  printk(KERN_INFO "Op2 is  %d\n", operand_2);
  printk(KERN_INFO "Oper is  %c\n", operator);
  printk(KERN_INFO "Res is  %d\n", res);

  return res;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  int count = strlen(rbuf);
  printk(KERN_INFO "Driver: read()\n");

  if (*off > 0 || len < count)
  {
    return 0;
  }

  if (copy_to_user(buf, rbuf, count) != 0)
  {
    return -EFAULT;
  }

  *off = count;
  return count;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{

  printk(KERN_INFO "Driver: write()\n");

  if (len > BUF_SIZE)
    return 0;

  if (copy_from_user(ibuf, buf, len) != 0)
  {
    return -EFAULT;
  }
  
  int result;
  result = process(ibuf);



  char b[16];
  int i;
  sprintf(b, "%d", result);
  for (i = 0; i<strlen(b); i++){
    rbuf[rbuf_off] = b[i];
    rbuf_off++;
  }
  

  rbuf[rbuf_off] = ' ';
  rbuf_off++;
  rbuf[rbuf_off] = '\0';

  printk(KERN_INFO "Buf is  %s\n", rbuf);
  printk(KERN_INFO "Buf Size is %d\n", rbuf_off);

  return len;
}

static struct file_operations mychdev_fops =
    {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
        .read = my_read,
        .write = my_write};

static int __init ch_drv_init(void)
{
  printk(KERN_INFO "Hello!\n");
  if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0)
  {
    return -1;
  }
  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
  {
    unregister_chrdev_region(first, 1);
    return -1;
  }

  if (device_create(cl, NULL, first, NULL, "var2") == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }

  cdev_init(&c_dev, &mychdev_fops);
  if (cdev_add(&c_dev, first, 1) == -1)
  {
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  return 0;
}

static void __exit ch_drv_exit(void)
{
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "Bye!!!\n");
}

module_init(ch_drv_init);
module_exit(ch_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gleb Makarov");
MODULE_DESCRIPTION("IO Lab 2");
