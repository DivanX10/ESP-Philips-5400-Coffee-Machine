import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

philips_series_5400_ns = cg.esphome_ns.namespace('philips_series_5400')

PhilipsSeries5400 = philips_series_5400_ns.class_('PhilipsSeries5400', cg.Component)

DISPLAY_UART_ID = 'display_uart'
MAINBOARD_UART_ID = 'mainboard_uart'

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PhilipsSeries5400),
    cv.Required(DISPLAY_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(MAINBOARD_UART_ID): cv.use_id(uart.UARTComponent),
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    display = await cg.get_variable(config[DISPLAY_UART_ID])
    cg.add(var.register_display_uart(display))
    mainboard = await cg.get_variable(config[MAINBOARD_UART_ID])
    cg.add(var.register_mainboard_uart(mainboard))
