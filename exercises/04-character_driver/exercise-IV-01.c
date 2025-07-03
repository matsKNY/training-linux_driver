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
#include <linux/string.h>

/******************************************************************************
 * Definitions:
 ******************************************************************************/
/* Number of characters a string (null-terminating byte not included) should
 * contain to represent the maximal reachable value for an `uint64_t`: */
#define UINT64_STR_MAXCHAR 20

/******************************************************************************
 * Variables - file operations:
 ******************************************************************************/
/* Mutex associated with `static uint64_t counter`: */
// TODO

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
    /* Internal absolute counter - incremented by one each time the `/dev` node
     * associated with the module is read by an external source (e.g. one call
     * to `cat` equals one "read"): */
    static uint64_t counter = 0;

    /* Number of characters read - to be returned: */
    ssize_t nb_read = 0;

    /* Kernel-side buffer to send the value of the counter to user-side: */
    void*  k_buf_tmp = NULL;
    size_t k_buf_tmp_size = 0;

    /***************************************************************************
     * Core:
     **************************************************************************/
    /* If the received offset is greater than 0, it means that the "current"
     * value of the counter was already read. Thus, we must return 0 to notify
     * the reader that the "read session" ended: */
    if (0 < *offset)
    {
        nb_read = 0;
        *offset = 0;

        goto l_fops_read_increment_ts;
    }

    /* Allocate a kernel-side buffer to send the value of the counter to
     * userland (+2 = newline + null-terminating characters): */
    k_buf_tmp = kmalloc(UINT64_STR_MAXCHAR + 2, GFP_KERNEL);
    /***/
    if (NULL == k_buf_tmp)
    {
        nb_read = -ENOMEM;
        goto l_fops_read_increment_ts;
    }
    /***/
    memset(k_buf_tmp, 0, UINT64_STR_MAXCHAR + 2);

    /* TODO - mutex lock */
    /* Fill the kernel-side buffer with the value of the counter: */
    if (0 >= snprintf(k_buf_tmp, UINT64_STR_MAXCHAR + 2, "%llu\n", counter))
    {
        nb_read = -EINVAL;
        /* TODO - mutex unlock */

        goto l_fops_read_increment_ts;
    }

    /* Check that the supplied userland buffer is large enough to hold the value
     * of the counter: */
    k_buf_tmp_size = strnlen(k_buf_tmp, UINT64_STR_MAXCHAR + 1);
    /***/
    if (( k_buf_tmp_size + 1) > length)
    {
        nb_read = -ENOMEM;
        /* TODO - mutex unlock */

        goto l_fops_read_increment_ts;
    }

    /* Update the counter: */
    counter++;
    /* TODO - mutex unlock */

    /* Send the kernel-side buffer to userland (null-terminating character
     * included).
     * The offset is set to the number of sent characters so as to notify the
     * reader during its next call to `read` that the "read session" ended: */
    nb_read = simple_read_from_buffer(
        u_buf_out, length, offset, k_buf_tmp, k_buf_tmp_size + 1
    );

    /***************************************************************************
     * Termination stack:
     **************************************************************************/
l_fops_read_increment_ts:
    /* Termination for `k_buf_tmp`: */
    if (NULL != k_buf_tmp) {kfree(k_buf_tmp);}

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
	.name  = "counter",
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
