/**
 * \file      exercise-IV-01.c
 * \author    Mathieu Stoffel
 * \date      {Thursday, July 3, 2025}
 * \license   GPL-2.0
 *
 * Implementation file for the kernel module associated with exercise 01 of
 * Chapter IV.
 */
/******************************************************************************
 * Includes: 
 ******************************************************************************/
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
/*******/
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#define _BSD_SOURCE 
#define _SVID_SOURCE 
#define _POSIX_C_SOURCE 200809
#endif
/*******/
#include <linux/miscdevice.h>
#include <linux/module.h>

/******************************************************************************
 * Functions - file operations:
 ******************************************************************************/
/*******
 * File operation - read - increment:
 *  + Increments an internal counter by 1 each time the `/dev` node of the
 *    module is read.
 *******/
static ssize_t fops_read_increment(
    struct file* f_info, char* u_buf_out, size_t length, loff_t* offset
)
{
    /***************************************************************************
     * Variables:
     **************************************************************************/
    /* Number of characters read - to be returned: */
    ssize_t nb_read = 0;

    /***************************************************************************
     * Core:
     **************************************************************************/

    return nb_read;
}

/******************************************************************************
 * Variables - definition of the module:
 ******************************************************************************/
/* File operation structure associated with the module: */
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = fops_read_increment,
};

/* Registration structure associated with the module: */
static struct miscdevice mod_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = THIS_MODULE->name,
	.fops  = &fops,
	.mode  = 0666,
};

/******************************************************************************
 * Core - registration:
 ******************************************************************************/
/* Register the module: */
module_misc_device(mod_miscdevice);

/* Define module metadata: */
MODULE_DESCRIPTION("exercise-IV-01");
MODULE_AUTHOR("Mathieu Stoffel <mathieu.stoffel@eviden.com>");
MODULE_LICENSE("GPL v2");
