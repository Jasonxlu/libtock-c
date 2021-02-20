#include "gpio_async.h"
#include "tock.h"

#define CONCAT_PORT_DATA(port, data) (((data & 0xFFFF) << 16) | (port & 0xFFFF))


struct gpio_async_data {
  bool fired;
  int value;
  int callback_type;
};

static struct gpio_async_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void gpio_async_cb(__attribute__ ((unused)) int callback_type,
                          __attribute__ ((unused)) int value,
                          __attribute__ ((unused)) int unused,
                          void* ud) {
  struct gpio_async_data* myresult = (struct gpio_async_data*) ud;
  myresult->callback_type = callback_type;
  myresult->value         = value;
  myresult->fired         = true;
}


int gpio_async_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sub = subscribe2(DRIVER_NUM_GPIO_ASYNC, 0, callback, callback_args);
  if (sub.success == 0) {
    return tock_error_to_rcode(sub.error);
  } else {
    return TOCK_SUCCESS;
  }
}

int gpio_async_make_output(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 1, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_set(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 2, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_clear(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 3, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }

}

int gpio_async_toggle(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 4, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_make_input(uint32_t port, uint8_t pin, GPIO_InputMode_t pin_config) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 5, pin, CONCAT_PORT_DATA(port, pin_config));
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_read(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 6, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_enable_interrupt(uint32_t port, uint8_t pin, GPIO_InterruptMode_t irq_config) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 7, pin, CONCAT_PORT_DATA(port, irq_config));
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_disable_interrupt(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 8, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_disable(uint32_t port, uint8_t pin) {
  syscall_return_t com = command2(DRIVER_NUM_GPIO_ASYNC, 9, pin, port);
  if (com.type == TOCK_SYSCALL_SUCCESS) {
    return TOCK_SUCCESS;
  } else if (com.type > TOCK_SYSCALL_SUCCESS) {
    // Returned an incorrect success code
    return TOCK_FAIL;
  } else {
    return tock_error_to_rcode(com.data[1]);
  }
}

int gpio_async_interrupt_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sub = subscribe2(DRIVER_NUM_GPIO_ASYNC, 1, callback, callback_args);
  if (sub.success) {
    return TOCK_SUCCESS;
  } else {
    return tock_error_to_rcode(sub.error);
  }
}



int gpio_async_make_output_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_make_output(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_set_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_set(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_clear_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_clear(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_toggle_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_toggle(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_make_input_sync(uint32_t port, uint8_t pin, GPIO_InputMode_t pin_config) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_make_input(port, pin, pin_config);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_read_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_read(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_enable_interrupt_sync(uint32_t port, uint8_t pin, GPIO_InterruptMode_t irq_config) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_enable_interrupt(port, pin, irq_config);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_disable_interrupt_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_disable_interrupt(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}

int gpio_async_disable_sync(uint32_t port, uint8_t pin) {
  int err;
  result.fired = false;

  err = gpio_async_set_callback(gpio_async_cb, (void*) &result);
  if (err < 0) return err;

  err = gpio_async_disable(port, pin);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return result.value;
}
