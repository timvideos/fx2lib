#!/usr/bin/python

import sys

strings = [x.strip() for x in sys.stdin.readlines()]

print """
#include <linux/ch9.h>

__xdata struct usb_descriptor_strings {"""
for i, string in enumerate(strings):
	print """\
	struct usb_string_%(i)i {
		__u8 bLength;
		__u8 bDescriptorType;
		__le16 wData[%(l)i];
	} string%(i)i;""" % {'l': len(string), 'i': i}
print """\
} descriptor_strings = {"""
for i, string in enumerate(strings):
	d = ["((__le16)('%s'))" % s for s in string]

	print """\
	// "%(s)s"
	.string%(i)i = {
		.bLength = sizeof(struct usb_string_%(i)i),
		.bDescriptorType = USB_DT_STRING,
		.wData = {%(d)s},
	},""" % {
		's': string,
		'i': i,
		'l': len(string),
		'd': ", ".join(d),
		}
print """\
};
"""
