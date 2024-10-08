/*
 *  usb notify header
 *
 * Copyright (C) 2011-2013 Samsung, Inc.
 * Author: Dongrak Shin <dongrak.shin@samsung.com>
 *
*/

 /* usb notify layer v2.0 */

#ifndef __LINUX_USB_NOTIFY_H__
#define __LINUX_USB_NOTIFY_H__

#include <linux/notifier.h>
#include <linux/host_notify.h>
#include <linux/external_notify.h>
#include <linux/usblog_proc_notify.h>

enum otg_notify_events {
	NOTIFY_EVENT_NONE,
	NOTIFY_EVENT_VBUS,
	NOTIFY_EVENT_HOST,
	NOTIFY_EVENT_CHARGER,
	NOTIFY_EVENT_SMARTDOCK_TA,
	NOTIFY_EVENT_SMARTDOCK_USB,
	NOTIFY_EVENT_AUDIODOCK,
	NOTIFY_EVENT_LANHUB,
	NOTIFY_EVENT_LANHUB_TA,
	NOTIFY_EVENT_MMDOCK,
	NOTIFY_EVENT_HMT,
	NOTIFY_EVENT_GAMEPAD,
	NOTIFY_EVENT_DRIVE_VBUS,
	NOTIFY_EVENT_ALL_DISABLE,
	NOTIFY_EVENT_HOST_DISABLE,
	NOTIFY_EVENT_CLIENT_DISABLE,
	NOTIFY_EVENT_OVERCURRENT,
	NOTIFY_EVENT_SMSC_OVC,
	NOTIFY_EVENT_SMTD_EXT_CURRENT,
	NOTIFY_EVENT_MMD_EXT_CURRENT,
	NOTIFY_EVENT_DEVICE_CONNECT,
	NOTIFY_EVENT_GAMEPAD_CONNECT,
	NOTIFY_EVENT_LANHUB_CONNECT,
	NOTIFY_EVENT_VBUSPOWER,
	NOTIFY_EVENT_VIRTUAL,
};

#define VIRT_EVENT(a) (a+NOTIFY_EVENT_VIRTUAL)
#define PHY_EVENT(a) (a%NOTIFY_EVENT_VIRTUAL)
#define IS_VIRTUAL(a) (a >= NOTIFY_EVENT_VIRTUAL ? 1 : 0)

enum otg_notify_event_status {
	NOTIFY_EVENT_DISABLED,
	NOTIFY_EVENT_DISABLING,
	NOTIFY_EVENT_ENABLED,
	NOTIFY_EVENT_ENABLING,
	NOTIFY_EVENT_BLOCKED,
	NOTIFY_EVENT_BLOCKING,
};

enum otg_notify_evt_type {
	NOTIFY_EVENT_EXTRA = (1 << 0),
	NOTIFY_EVENT_STATE = (1 << 1),
	NOTIFY_EVENT_DELAY = (1 << 2),
	NOTIFY_EVENT_NEED_VBUSDRIVE = (1 << 3),
	NOTIFY_EVENT_NOBLOCKING = (1 << 4),
	NOTIFY_EVENT_NOSAVE = (1 << 5),
	NOTIFY_EVENT_NEED_HOST = (1 << 6),
	NOTIFY_EVENT_NEED_CLIENT = (1 << 7),
};

enum otg_notify_block_type {
	NOTIFY_BLOCK_TYPE_NONE = 0,
	NOTIFY_BLOCK_TYPE_HOST = (1 << 0),
	NOTIFY_BLOCK_TYPE_CLIENT = (1 << 1),
	NOTIFY_BLOCK_TYPE_ALL = (1 << 0 | 1 << 1),
};

enum otg_notify_gpio {
	NOTIFY_VBUS,
	NOTIFY_REDRIVER,
};

enum otg_op_pos {
	NOTIFY_OP_OFF,
	NOTIFY_OP_POST,
	NOTIFY_OP_PRE,
};

enum ovc_check_value {
	HNOTIFY_LOW,
	HNOTIFY_HIGH,
	HNOTIFY_INITIAL,
};

struct otg_notify {
	int vbus_detect_gpio;
	int redriver_en_gpio;
	int is_wakelock;
	int unsupport_host;
	int smsc_ovc_poll_sec;
	int auto_drive_vbus;
	int booting_delay_sec;
	int disable_control;
	int device_check_sec;
	int speed;
	const char *muic_name;
	int (*pre_gpio) (int gpio, int use);
	int (*post_gpio) (int gpio, int use);
	int (*vbus_drive) (bool);
	int (*set_host) (bool);
	int (*set_peripheral)(bool);
	int (*set_charger)(bool);
	int (*post_vbus_detect)(bool);
	int (*set_lanhubta)(int);
	int (*set_battcall)(int, int);
	void *o_data;
	void *u_notify;
};

struct otg_booster {
	char *name;
	int (*booster) (bool);
};

#ifdef CONFIG_USB_NOTIFY_LAYER
extern const char *event_string(enum otg_notify_events event);
extern const char *status_string(enum otg_notify_event_status status);
extern void send_otg_notify(struct otg_notify *n,
					unsigned long event, int enable);
extern struct otg_booster *find_get_booster(struct otg_notify *n);
extern int register_booster(struct otg_notify *n, struct otg_booster *b);
extern int register_ovc_func(struct otg_notify *n,
				int (*check_state)(void *), void *data);
extern int get_usb_mode(struct otg_notify *n);
extern unsigned long get_cable_type(struct otg_notify *n);
extern int is_usb_host(struct otg_notify *n);
extern void *get_notify_data(struct otg_notify *n);
extern void set_notify_data(struct otg_notify *n, void *data);
extern struct otg_notify *get_otg_notify(void);
extern int set_otg_notify(struct otg_notify *n);
extern void put_otg_notify(struct otg_notify *n);
extern bool is_blocked(struct otg_notify *n, int type);
#else
static inline const char *event_string(enum otg_notify_events event)
			{return NULL; }
static inline const char *status_string(enum otg_notify_event_status status)
			{return NULL; }
static inline void send_otg_notify(struct otg_notify *n,
					unsigned long event, int enable) { }
static inline struct otg_booster *find_get_booster(struct otg_notify *n)
			{return NULL; }
static inline int register_booster(struct otg_notify *n,
					struct otg_booster *b) {return 0; }
static inline int register_ovc_func(struct otg_notify *n,
			int (*check_state)(void *), void *data) {return 0; }
static inline int get_usb_mode(struct otg_notify *n) {return 0; }
static inline unsigned long get_cable_type(struct otg_notify *n) {return 0; }
static inline int is_usb_host(struct otg_notify *n) {return 0; }
static inline void *get_notify_data(struct otg_notify *n) {return NULL; }
static inline void set_notify_data(struct otg_notify *n, void *data) {}
static inline struct otg_notify *get_otg_notify(void) {return NULL; }
static inline int set_otg_notify(struct otg_notify *n) {return 0; }
static inline void put_otg_notify(struct otg_notify *n) {}
static inline bool is_blocked(struct otg_notify *n, int type) {return false; }
#endif
#endif /* __LINUX_USB_NOTIFY_H__ */
