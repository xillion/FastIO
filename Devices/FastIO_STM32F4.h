#if defined(TARGET_STM32F4)

#include "mbed.h"
#include "pinmap.h"

typedef struct {
    uint32_t mask;
} fastio_vars;

#define PINMASK(pin)             (1 << STM_PIN(pin))
#define PINMASK_CLR(pin)         ((1<<16) << STM_PIN(pin))
#define PORT(pin)                ((GPIO_TypeDef *)(GPIOA_BASE + 0x0400 * STM_PORT(pin)))

#define INIT_PIN            RCC->AHB1ENR |= (1 << STM_PORT(pin)); (PORT(pin)->MODER &= ~(GPIO_MODER_MODER0_1 << (STM_PIN(pin) * 2))); container.mask = PINMASK
#define DESTROY_PIN     

#define SET_DIR_INPUT(pin)       (PORT(pin)->MODER &= ~(GPIO_MODER_MODER0_0 << (STM_PIN(pin) * 2)))
#define SET_DIR_OUTPUT(pin)      (PORT(pin)->MODER |= (GPIO_MODER_MODER0_0 << (STM_PIN(pin) * 2)))
#define SET_MODE(pin, pull)      pin_mode(pin, pull);

#define WRITE_PIN_SET(pin)       (PORT(pin)->BSRR = PINMASK(pin))
#define WRITE_PIN_CLR(pin)       (PORT(pin)->BSRR = PINMASK_CLR(pin))

#define READ_PIN(pin)            ((PORT(pin)->IDR & container.mask) ? 1 : 0)

#endif
