
#include <stddef.h>
#include <linux/ch9.h>
#include <linux/ch9-extra.h>


struct usb_section {
	struct usb_config_descriptor config;
	struct usb_interface_descriptor interface;
	struct usb_endpoint_descriptor endpoints[2];
};

struct usb_str {
	__u8  bLength;
	__u8  bDescriptorType;

	__le16 wData[];		/* UTF-16LE encoded */
};

struct usb_descriptor {
	struct usb_device_descriptor device;
	struct usb_qualifier_descriptor qualifier;
	struct usb_section highspeed;
	struct usb_section fullspeed;
};

__xdata struct usb_descriptor descriptor = {
	.device = {
		.bLength		= USB_DT_DEVICE_SIZE,
		.bDescriptorType	= USB_DT_DEVICE,
		.bcdUSB			= USB_BCD_V20,
		.bDeviceClass 		= USB_CLASS_VENDOR_SPEC,
		.bDeviceSubClass	= USB_SUBCLASS_VENDOR_SPEC,
		.bDeviceProtocol	= 0xff,
		.bMaxPacketSize0	= 64,
		.idVendor		= 0x04B4,
		.idProduct		= 0x1004,
		.bcdDevice		= 0x0001,
		.iManufacturer		= 1,
		.iProduct		= 2,
		.iSerialNumber		= 0,
		.bNumConfigurations	= 1
	},
	.qualifier = {
		.bLength		= USB_DT_DEVICE_QUALIFIER_SIZE,
		.bDescriptorType	= USB_DT_DEVICE_QUALIFIER,
		.bcdUSB			= USB_BCD_V20,
		.bDeviceClass		= USB_CLASS_VENDOR_SPEC,
		.bDeviceSubClass	= USB_SUBCLASS_VENDOR_SPEC,
		.bDeviceProtocol	= 0xff,
		.bMaxPacketSize0	= 64,
		.bNumConfigurations	= 1,
		.bRESERVED		= 0,
	},
	.highspeed = {
		.config = {
			.bLength		= USB_DT_CONFIG_SIZE,
			.bDescriptorType	= USB_DT_CONFIG,
			.wTotalLength		= sizeof(descriptor.highspeed),
			.bNumInterfaces		= 1,
			.bConfigurationValue	= 1,
			.iConfiguration		= 0,
			.bmAttributes		= USB_CONFIG_ATT_ONE,
			.bMaxPower		= 0x32, // FIXME: ???
		},
		.interface = {
			.bLength		= USB_DT_INTERFACE_SIZE,
			.bDescriptorType	= USB_DT_INTERFACE,
			.bInterfaceNumber	= 0,
			.bAlternateSetting	= 0,
			.bNumEndpoints		= 2,
			.bInterfaceClass	= USB_CLASS_VENDOR_SPEC,
			.bInterfaceSubClass	= USB_SUBCLASS_VENDOR_SPEC,
			.bInterfaceProtocol	= 0xff,
			.iInterface		= 3,
		},
		.endpoints = {
			{
				.bLength		= USB_DT_ENDPOINT_SIZE,
				.bDescriptorType 	= USB_DT_ENDPOINT,
				.bEndpointAddress	= USB_ENDPOINT_NUMBER(0x2) | USB_DIR_OUT,
				.bmAttributes		= USB_ENDPOINT_XFER_BULK,
				.wMaxPacketSize		= 512,
				.bInterval		= 0,
			},
			{
				.bLength		= USB_DT_ENDPOINT_SIZE,
				.bDescriptorType 	= USB_DT_ENDPOINT,
				.bEndpointAddress	= USB_ENDPOINT_NUMBER(0x6) | USB_DIR_IN,
				.bmAttributes		= USB_ENDPOINT_XFER_BULK,
				.wMaxPacketSize		= 512,
				.bInterval		= 0,
			},
		},
	},
	.fullspeed = {
		.config = {
			.bLength		= USB_DT_CONFIG_SIZE,
			.bDescriptorType	= USB_DT_CONFIG,
			.wTotalLength		= sizeof(descriptor.fullspeed),
			.bNumInterfaces		= 1,
			.bConfigurationValue	= 1,
			.iConfiguration		= 0,
			.bmAttributes		= USB_CONFIG_ATT_ONE,
			.bMaxPower		= 0x32, // FIXME: ???
		},
		.interface = {
			.bLength		= USB_DT_INTERFACE_SIZE,
			.bDescriptorType	= USB_DT_INTERFACE,
			.bInterfaceNumber	= 0,
			.bAlternateSetting	= 0,
			.bNumEndpoints		= 2,
			.bInterfaceClass	= USB_CLASS_VENDOR_SPEC,
			.bInterfaceSubClass	= USB_SUBCLASS_VENDOR_SPEC,
			.bInterfaceProtocol	= 0xff,
			.iInterface		= 3,
		},
		.endpoints = {
			{
				.bLength		= USB_DT_ENDPOINT_SIZE,
				.bDescriptorType 	= USB_DT_ENDPOINT,
				.bEndpointAddress	= USB_ENDPOINT_NUMBER(0x2) | USB_DIR_OUT,
				.bmAttributes		= USB_ENDPOINT_XFER_BULK,
				.wMaxPacketSize		= 64,
				.bInterval		= 0,
			},
			{
				.bLength		= USB_DT_ENDPOINT_SIZE,
				.bDescriptorType 	= USB_DT_ENDPOINT,
				.bEndpointAddress	= USB_ENDPOINT_NUMBER(0x6) | USB_DIR_IN,
				.bmAttributes		= USB_ENDPOINT_XFER_BULK,
				.wMaxPacketSize		= 64,
				.bInterval		= 0,
			},
		},
	},
};
