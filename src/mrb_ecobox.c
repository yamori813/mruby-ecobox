/*
** mrb_ecobox.c - Remocon class
**
** Copyright (c) Hiroki Mori 2016
** refer http://va009039.blogspot.com/2012/02/usbsensor-hmecombed.html
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mrb_ecobox.h"
#if defined( __FreeBSD__ )
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

#define VENDOR_ID  0x04bb
#define PRODUCT_ID 0x0f04

#define ECOBOX_RANGE_30CM	0x11
#define ECOBOX_RANGE_60CM	0x12
#define ECOBOX_RANGE_90CM	0x14
#define ECOBOX_RANGE_120CM	0x18

#define HOST_TO_DEVICE		0x00
#define REQUEST_TYPE_VENDOR	0x40
#define RECIPIENT_ENDPOINT	0x02

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  libusb_context *ctx;
  libusb_device_handle *devh;
} mrb_ecobox_data;

static const struct mrb_data_type mrb_ecobox_data_type = {
  "mrb_ecobox_data", mrb_free,
};

static mrb_value mrb_ecobox_init(mrb_state *mrb, mrb_value self)
{
  mrb_ecobox_data *data;

  data = (mrb_ecobox_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_ecobox_data_type;
  DATA_PTR(self) = NULL;

  data = (mrb_ecobox_data *)mrb_malloc(mrb, sizeof(mrb_ecobox_data));

  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_ecobox_open(mrb_state *mrb, mrb_value self)
{
  mrb_ecobox_data *data = DATA_PTR(self);
  data->ctx = NULL;
  libusb_init(&data->ctx);
  data->devh = libusb_open_device_with_vid_pid(data->ctx, VENDOR_ID, PRODUCT_ID);
  if (data->devh == -1) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Can't open USB module.");
    return mrb_nil_value();
  }
//  libusb_claim_interface(data->devh, 0);
  return mrb_fixnum_value(1);
}

static mrb_value mrb_ecobox_get(mrb_state *mrb, mrb_value self)
{
  mrb_ecobox_data *data = DATA_PTR(self);
  int err;
  unsigned char buff[8];
  int len;
  mrb_int timeout;

  mrb_get_args(mrb, "i", &timeout);

  err = libusb_interrupt_transfer(data->devh, 0x81, buff, 8, &len, timeout);

  return mrb_fixnum_value(err);
}

static mrb_value mrb_ecobox_range(mrb_state *mrb, mrb_value self)
{
  mrb_ecobox_data *data = DATA_PTR(self);
  mrb_int value;

  mrb_get_args(mrb, "i", &value);

  libusb_control_transfer(data->devh, HOST_TO_DEVICE|REQUEST_TYPE_VENDOR|RECIPIENT_ENDPOINT, 0x10, value, 0, NULL, 0, 0);

  return mrb_fixnum_value(1);
}

void mrb_mruby_ecobox_gem_init(mrb_state *mrb)
{
  struct RClass *ecobox;
  ecobox = mrb_define_class(mrb, "Ecobox", mrb->object_class);
  mrb_define_const(mrb, ecobox, "ECOBOX_RANGE_30CM",  mrb_fixnum_value(ECOBOX_RANGE_30CM));
  mrb_define_const(mrb, ecobox, "ECOBOX_RANGE_60CM",  mrb_fixnum_value(ECOBOX_RANGE_60CM));
  mrb_define_const(mrb, ecobox, "ECOBOX_RANGE_90CM",  mrb_fixnum_value(ECOBOX_RANGE_90CM));
  mrb_define_const(mrb, ecobox, "ECOBOX_RANGE_120CM",  mrb_fixnum_value(ECOBOX_RANGE_120CM));
  mrb_define_method(mrb, ecobox, "initialize", mrb_ecobox_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, ecobox, "open", mrb_ecobox_open, MRB_ARGS_NONE());
  mrb_define_method(mrb, ecobox, "get", mrb_ecobox_get, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ecobox, "range", mrb_ecobox_range, MRB_ARGS_REQ(1));
  DONE;
}

void mrb_mruby_ecobox_gem_final(mrb_state *mrb)
{
}

