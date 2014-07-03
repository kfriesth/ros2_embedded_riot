/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cpu_stm32f1
 * @{
 *
 * @file
 * @brief       Low-level GPIO driver implementation
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 * @author      Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 *
 * @}
 */

#include "cpu.h"
#include "periph/gpio.h"
#include "periph_conf.h"

typedef struct {
    void (*cb)(void);
} gpio_state_t;

static gpio_state_t config[GPIO_NUMOF];

int gpio_init_out(gpio_t dev, gpio_pp_t pullup)
{
    GPIO_TypeDef *port;
    uint32_t pin;

    switch (dev) {
#ifdef GPIO_0_EN
        case GPIO_0:
            GPIO_0_CLKEN();
            port = GPIO_0_PORT;
            pin = GPIO_0_PIN;
            break;
#endif
#ifdef GPIO_1_EN
        case GPIO_1:
            GPIO_1_CLKEN();
            port = GPIO_1_PORT;
            pin = GPIO_1_PIN;
            break;
#endif
#ifdef GPIO_2_EN
        case GPIO_2:
            GPIO_2_CLKEN();
            port = GPIO_2_PORT;
            pin = GPIO_2_PIN;
            break;
#endif
#ifdef GPIO_3_EN
        case GPIO_3:
            GPIO_3_CLKEN();
            port = GPIO_3_PORT;
            pin = GPIO_3_PIN;
            break;
#endif
#ifdef GPIO_4_EN
        case GPIO_4:
            GPIO_4_CLKEN();
            port = GPIO_4_PORT;
            pin = GPIO_4_PIN;
            break;
#endif
#ifdef GPIO_5_EN
        case GPIO_5:
            GPIO_5_CLKEN();
            port = GPIO_5_PORT;
            pin = GPIO_5_PIN;
            break;
#endif
#ifdef GPIO_6_EN
        case GPIO_6:
            GPIO_6_CLKEN();
            port = GPIO_6_PORT;
            pin = GPIO_6_PIN;
            break;
#endif
#ifdef GPIO_7_EN
        case GPIO_7:
            GPIO_7_CLKEN();
            port = GPIO_7_PORT;
            pin = GPIO_7_PIN;
            break;
#endif
#ifdef GPIO_8_EN
        case GPIO_8:
            GPIO_8_CLKEN();
            port = GPIO_8_PORT;
            pin = GPIO_8_PIN;
            break;
#endif
#ifdef GPIO_9_EN
        case GPIO_9:
            GPIO_9_CLKEN();
            port = GPIO_9_PORT;
            pin = GPIO_9_PIN;
            break;
#endif
#ifdef GPIO_10_EN
        case GPIO_10:
            GPIO_10_CLKEN();
            port = GPIO_10_PORT;
            pin = GPIO_10_PIN;
            break;
#endif
#ifdef GPIO_11_EN
        case GPIO_11:
            GPIO_11_CLKEN();
            port = GPIO_11_PORT;
            pin = GPIO_11_PIN;
            break;
#endif
        case GPIO_UNDEFINED:
        default:
            return -1;
    }

    if (pin < 8) {
        port->CRL &= ~(0xf << (4 * pin));
        port->CRL |= (0x3 << (4* pin)); /* Output mode, 50 MHz */
                                        /* general purpose push-pull set implicitly */
        port->ODR |= (1 << pin);        /* set pin to low signal */
    }
    else {
        port->CRH &= ~(0xf << (4 * (pin-8)));
        port->CRH |= (0x3 << (4* (pin-8))); /* Output mode, 50 MHz */
                                            /* general purpose push-pull set implicitly */
        port->ODR |= (1 << pin);            /* set pin to low signal */
    }

    return 0; /* all OK */
}

int gpio_init_in(gpio_t dev, gpio_pp_t pullup)
{
    GPIO_TypeDef *port;
    uint32_t pin;

    switch (dev) {
#ifdef GPIO_0_EN
        case GPIO_0:
            GPIO_0_CLKEN();
            port = GPIO_0_PORT;
            pin = GPIO_0_PIN;
            break;
#endif
#ifdef GPIO_1_EN
        case GPIO_1:
            GPIO_1_CLKEN();
            port = GPIO_1_PORT;
            pin = GPIO_1_PIN;
            break;
#endif
#ifdef GPIO_2_EN
        case GPIO_2:
            GPIO_2_CLKEN();
            port = GPIO_2_PORT;
            pin = GPIO_2_PIN;
            break;
#endif
#ifdef GPIO_3_EN
        case GPIO_3:
            GPIO_3_CLKEN();
            port = GPIO_3_PORT;
            pin = GPIO_3_PIN;
            break;
#endif
#ifdef GPIO_4_EN
        case GPIO_4:
            GPIO_4_CLKEN();
            port = GPIO_4_PORT;
            pin = GPIO_4_PIN;
            break;
#endif
#ifdef GPIO_5_EN
        case GPIO_5:
            GPIO_5_CLKEN();
            port = GPIO_5_PORT;
            pin = GPIO_5_PIN;
            break;
#endif
#ifdef GPIO_6_EN
        case GPIO_6:
            GPIO_6_CLKEN();
            port = GPIO_6_PORT;
            pin = GPIO_6_PIN;
            break;
#endif
#ifdef GPIO_7_EN
        case GPIO_7:
            GPIO_7_CLKEN();
            port = GPIO_7_PORT;
            pin = GPIO_7_PIN;
            break;
#endif
#ifdef GPIO_8_EN
        case GPIO_8:
            GPIO_8_CLKEN();
            port = GPIO_8_PORT;
            pin = GPIO_8_PIN;
            break;
#endif
#ifdef GPIO_9_EN
        case GPIO_9:
            GPIO_9_CLKEN();
            port = GPIO_9_PORT;
            pin = GPIO_9_PIN;
            break;
#endif
#ifdef GPIO_10_EN
        case GPIO_10:
            GPIO_10_CLKEN();
            port = GPIO_10_PORT;
            pin = GPIO_10_PIN;
            break;
#endif
#ifdef GPIO_11_EN
        case GPIO_11:
            GPIO_11_CLKEN();
            port = GPIO_11_PORT;
            pin = GPIO_11_PIN;
            break;
#endif
        case GPIO_UNDEFINED:
        default:
            return -1;
    }

    if (pin < 8) {
        port->CRL &= ~(0xf << (4 * pin));
    }
    else {
        port->CRH &= ~(0xf << (4 * (pin-8)));
    }

    return 0; /* everything alright here */
}

int gpio_init_int(gpio_t dev, gpio_pp_t pullup, gpio_flank_t flank, void (*cb)(void))
{
    int res;
    uint32_t pin;
    uint8_t exti_line;

    (void)pin;

    /* configure pin as input */
    res = gpio_init_in(dev, pullup);
    if (res < 0) {
        return res;
    }

    /* set interrupt priority (its the same for all EXTI interrupts) */
    NVIC_SetPriority(EXTI0_IRQn, GPIO_IRQ_PRIO);
    NVIC_SetPriority(EXTI1_IRQn, GPIO_IRQ_PRIO);
    NVIC_SetPriority(EXTI2_IRQn, GPIO_IRQ_PRIO);
    NVIC_SetPriority(EXTI4_IRQn, GPIO_IRQ_PRIO);

    /* enable clock of the SYSCFG module for EXTI configuration */
    //RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

    /* read pin number, set EXIT channel and enable global interrupt for EXTI channel */
    switch (dev) {
#ifdef GPIO_0_EN
        case GPIO_0:
            pin = GPIO_0_PIN;
            exti_line = GPIO_0_EXTI_LINE;
            GPIO_0_EXTI_CFG();
            NVIC_SetPriority(GPIO_0_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_0_IRQ);
            break;
#endif
#ifdef GPIO_1_EN
        case GPIO_1:
            pin = GPIO_1_PIN;
            exti_line = GPIO_1_EXTI_LINE;
            GPIO_1_EXTI_CFG();
            NVIC_SetPriority(GPIO_1_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_1_IRQ);
            break;
#endif
#ifdef GPIO_2_EN
        case GPIO_2:
            pin = GPIO_2_PIN;
            exti_line = GPIO_2_EXTI_LINE;
            GPIO_2_EXTI_CFG();
            NVIC_SetPriority(GPIO_2_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_2_IRQ);
            break;
#endif
#ifdef GPIO_3_EN
        case GPIO_3:
            pin = GPIO_3_PIN;
            exti_line = GPIO_3_EXTI_LINE;
            GPIO_3_EXTI_CFG();
            NVIC_SetPriority(GPIO_3_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_3_IRQ);
            break;
#endif
#ifdef GPIO_4_EN
        case GPIO_4:
            pin = GPIO_4_PIN;
            exti_line = GPIO_4_EXTI_LINE;
            GPIO_4_EXTI_CFG();
            NVIC_SetPriority(GPIO_4_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_4_IRQ);
            break;
#endif
#ifdef GPIO_5_EN
        case GPIO_5:
            pin = GPIO_5_PIN;
            exti_line = GPIO_5_EXTI_LINE;
            GPIO_5_EXTI_CFG();
            NVIC_SetPriority(GPIO_5_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_5_IRQ);
            break;
#endif
#ifdef GPIO_6_EN
        case GPIO_6:
            pin = GPIO_6_PIN;
            exti_line = GPIO_6_EXTI_LINE;
            GPIO_6_EXTI_CFG();
            NVIC_SetPriority(GPIO_6_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_6_IRQ);
            break;
#endif
#ifdef GPIO_7_EN
        case GPIO_7:
            pin = GPIO_7_PIN;
            exti_line = GPIO_7_EXTI_LINE;
            GPIO_7_EXTI_CFG();
            NVIC_SetPriority(GPIO_7_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_7_IRQ);
            break;
#endif
#ifdef GPIO_8_EN
        case GPIO_8:
            pin = GPIO_8_PIN;
            exti_line = GPIO_8_EXTI_LINE;
            GPIO_8_EXTI_CFG();
            NVIC_SetPriority(GPIO_8_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_8_IRQ);
            break;
#endif
#ifdef GPIO_9_EN
        case GPIO_9:
            pin = GPIO_9_PIN;
            exti_line = GPIO_9_EXTI_LINE;
            GPIO_9_EXTI_CFG();
            NVIC_SetPriority(GPIO_9_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_9_IRQ);
            break;
#endif
#ifdef GPIO_10_EN
        case GPIO_10:
            pin = GPIO_10_PIN;
            exti_line = GPIO_10_EXTI_LINE;
            GPIO_10_EXTI_CFG();
            NVIC_SetPriority(GPIO_10_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_10_IRQ);
            break;
#endif
#ifdef GPIO_11_EN
        case GPIO_11:
            pin = GPIO_11_PIN;
            exti_line = GPIO_11_EXTI_LINE;
            GPIO_11_EXTI_CFG();
            NVIC_SetPriority(GPIO_11_IRQ, GPIO_IRQ_PRIO);
            NVIC_EnableIRQ(GPIO_11_IRQ);
            break;
#endif
        case GPIO_UNDEFINED:
        default:
            return -1;
    }

    /* set callback */
    config[dev].cb = cb;

    /* configure the event that triggers an interrupt */
    switch (flank) {
        case GPIO_RISING:
            EXTI->RTSR |= (1 << exti_line);
            EXTI->FTSR &= ~(1 << exti_line);
            break;
        case GPIO_FALLING:
            EXTI->RTSR &= ~(1 << exti_line);
            EXTI->FTSR |= (1 << exti_line);
            break;
        case GPIO_BOTH:
            EXTI->RTSR |= (1 << exti_line);
            EXTI->FTSR |= (1 << exti_line);
            break;
    }

    /* clear event mask */
    EXTI->EMR &= ~(1 << exti_line);
    /* unmask the pins interrupt channel */
    EXTI->IMR |= (1 << exti_line);

    return 0;
}

int gpio_read(gpio_t dev)
{
    GPIO_TypeDef *port;
    uint32_t pin;

    switch (dev) {
#ifdef GPIO_0_EN
        case GPIO_0:
            port = GPIO_0_PORT;
            pin = GPIO_0_PIN;
            break;
#endif
#ifdef GPIO_1_EN
        case GPIO_1:
            port = GPIO_1_PORT;
            pin = GPIO_1_PIN;
            break;
#endif
#ifdef GPIO_2_EN
        case GPIO_2:
            port = GPIO_2_PORT;
            pin = GPIO_2_PIN;
            break;
#endif
#ifdef GPIO_3_EN
        case GPIO_3:
            port = GPIO_3_PORT;
            pin = GPIO_3_PIN;
            break;
#endif
#ifdef GPIO_4_EN
        case GPIO_4:
            port = GPIO_4_PORT;
            pin = GPIO_4_PIN;
            break;
#endif
#ifdef GPIO_5_EN
        case GPIO_5:
            port = GPIO_5_PORT;
            pin = GPIO_5_PIN;
            break;
#endif
#ifdef GPIO_6_EN
        case GPIO_6:
            port = GPIO_6_PORT;
            pin = GPIO_6_PIN;
            break;
#endif
#ifdef GPIO_7_EN
        case GPIO_7:
            port = GPIO_7_PORT;
            pin = GPIO_7_PIN;
            break;
#endif
#ifdef GPIO_8_EN
        case GPIO_8:
            port = GPIO_8_PORT;
            pin = GPIO_8_PIN;
            break;
#endif
#ifdef GPIO_9_EN
        case GPIO_9:
            port = GPIO_9_PORT;
            pin = GPIO_9_PIN;
            break;
#endif
#ifdef GPIO_10_EN
        case GPIO_10:
            port = GPIO_10_PORT;
            pin = GPIO_10_PIN;
            break;
#endif
#ifdef GPIO_11_EN
        case GPIO_11:
            port = GPIO_11_PORT;
            pin = GPIO_11_PIN;
            break;
#endif
        case GPIO_UNDEFINED:
        default:
            return -1;
    }

    if (pin < 8) {
        if (port->CRL & (0x3 << (pin * 4))) {  /* if configured as output */
            return port->ODR & (1 << pin);          /* read output data register */
        } else {
            return port->IDR & (1 << pin);          /* else read input data register */
        }
    }
    else {
        if (port->CRH & (0x3 << ((pin-8) * 4))) {  /* if configured as output */
            return port->ODR & (1 << pin);          /* read output data register */
        } else {
            return port->IDR & (1 << pin);          /* else read input data register */
        }
    }
}

int gpio_set(gpio_t dev)
{
    switch (dev) {
#ifdef GPIO_0_EN
        case GPIO_0:
            GPIO_0_PORT->ODR |= (1 << GPIO_0_PIN);
            break;
#endif
#ifdef GPIO_1_EN
        case GPIO_1:
            GPIO_1_PORT->ODR |= (1 << GPIO_1_PIN);
            break;
#endif
#ifdef GPIO_2_EN
        case GPIO_2:
            GPIO_2_PORT->ODR |= (1 << GPIO_2_PIN);
            break;
#endif
#ifdef GPIO_3_EN
        case GPIO_3:
            GPIO_3_PORT->ODR |= (1 << GPIO_3_PIN);
            break;
#endif
#ifdef GPIO_4_EN
        case GPIO_4:
            GPIO_4_PORT->ODR |= (1 << GPIO_4_PIN);
            break;
#endif
#ifdef GPIO_5_EN
        case GPIO_5:
            GPIO_5_PORT->ODR |= (1 << GPIO_5_PIN);
            break;
#endif
#ifdef GPIO_6_EN
        case GPIO_6:
            GPIO_6_PORT->ODR |= (1 << GPIO_6_PIN);
            break;
#endif
#ifdef GPIO_7_EN
        case GPIO_7:
            GPIO_7_PORT->ODR |= (1 << GPIO_7_PIN);
            break;
#endif
#ifdef GPIO_8_EN
        case GPIO_8:
            GPIO_8_PORT->ODR |= (1 << GPIO_8_PIN);
            break;
#endif
#ifdef GPIO_9_EN
        case GPIO_9:
            GPIO_9_PORT->ODR |= (1 << GPIO_9_PIN);
            break;
#endif
#ifdef GPIO_10_EN
        case GPIO_10:
            GPIO_10_PORT->ODR |= (1 << GPIO_10_PIN);
            break;
#endif
#ifdef GPIO_11_EN
        case GPIO_11:
            GPIO_11_PORT->ODR |= (1 << GPIO_11_PIN);
            break;
#endif
        case GPIO_UNDEFINED:
        default:
            return -1;
    }

    return 0;
}

int gpio_clear(gpio_t dev)
{
    switch (dev) {
#ifdef GPIO_0_EN
        case GPIO_0:
            GPIO_0_PORT->ODR &= ~(1 << GPIO_0_PIN);
            break;
#endif
#ifdef GPIO_1_EN
        case GPIO_1:
            GPIO_1_PORT->ODR &= ~(1 << GPIO_1_PIN);
            break;
#endif
#ifdef GPIO_2_EN
        case GPIO_2:
            GPIO_2_PORT->ODR &= ~(1 << GPIO_2_PIN);
            break;
#endif
#ifdef GPIO_3_EN
        case GPIO_3:
            GPIO_3_PORT->ODR &= ~(1 << GPIO_3_PIN);
            break;
#endif
#ifdef GPIO_4_EN
        case GPIO_4:
            GPIO_4_PORT->ODR &= ~(1 << GPIO_4_PIN);
            break;
#endif
#ifdef GPIO_5_EN
        case GPIO_5:
            GPIO_5_PORT->ODR &= ~(1 << GPIO_5_PIN);
            break;
#endif
#ifdef GPIO_6_EN
        case GPIO_6:
            GPIO_6_PORT->ODR &= ~(1 << GPIO_6_PIN);
            break;
#endif
#ifdef GPIO_7_EN
        case GPIO_7:
            GPIO_7_PORT->ODR &= ~(1 << GPIO_7_PIN);
            break;
#endif
#ifdef GPIO_8_EN
        case GPIO_8:
            GPIO_8_PORT->ODR &= ~(1 << GPIO_8_PIN);
            break;
#endif
#ifdef GPIO_9_EN
        case GPIO_9:
            GPIO_9_PORT->ODR &= ~(1 << GPIO_9_PIN);
            break;
#endif
#ifdef GPIO_10_EN
        case GPIO_10:
            GPIO_10_PORT->ODR &= ~(1 << GPIO_10_PIN);
            break;
#endif
#ifdef GPIO_11_EN
        case GPIO_11:
            GPIO_11_PORT->ODR &= ~(1 << GPIO_11_PIN);
            break;
#endif
        case GPIO_UNDEFINED:
        default:
            return -1;
    }

    return 0;
}


int gpio_toggle(gpio_t dev)
{
    if (gpio_read(dev)) {
        return gpio_clear(dev);
    } else {
        return gpio_set(dev);
    }
}

int gpio_write(gpio_t dev, int value)
{
    if (value) {
        return gpio_set(dev);
    } else {
        return gpio_clear(dev);
    }
}

__attribute__((naked)) void isr_exti0_1(void)
{
    ISR_ENTER();
    if (EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR |= EXTI_PR_PR0;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_0].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR1) {
        EXTI->PR |= EXTI_PR_PR1;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_1].cb();
    }
    ISR_EXIT();
}

__attribute__((naked)) void isr_exti2_3(void)
{
    ISR_ENTER();
    if (EXTI->PR & EXTI_PR_PR2) {
        EXTI->PR |= EXTI_PR_PR2;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_2].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR3) {
        EXTI->PR |= EXTI_PR_PR3;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_3].cb();
    }
    ISR_EXIT();
}

__attribute__((naked)) void isr_exti4_15(void)
{
    ISR_ENTER();
    if (EXTI->PR & EXTI_PR_PR4) {
        EXTI->PR |= EXTI_PR_PR4;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_4].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR5) {
        EXTI->PR |= EXTI_PR_PR5;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_5].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR6) {
        EXTI->PR |= EXTI_PR_PR6;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_6].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR7) {
        EXTI->PR |= EXTI_PR_PR7;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_7].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR8) {
        EXTI->PR |= EXTI_PR_PR8;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_8].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR9) {
        EXTI->PR |= EXTI_PR_PR9;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_9].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR10) {
        EXTI->PR |= EXTI_PR_PR10;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_10].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR11) {
        EXTI->PR |= EXTI_PR_PR11;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_11].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR12) {
        EXTI->PR |= EXTI_PR_PR12;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_12].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR13) {
        EXTI->PR |= EXTI_PR_PR13;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_13].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR14) {
        EXTI->PR |= EXTI_PR_PR14;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_14].cb();
    }
    else if (EXTI->PR & EXTI_PR_PR15) {
        EXTI->PR |= EXTI_PR_PR15;        /* clear status bit by writing a 1 to it */
        config[GPIO_IRQ_15].cb();
    }
    ISR_EXIT();
}