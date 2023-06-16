from typing_extensions import Required
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components.uart import UARTComponent

from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

DISPLAY_UART_ID = 'display_uart'
MAINBOARD_UART_ID = 'mainboard_uart'
CONTROLLER_ID = 'controller_id'

philips_series_5400_ns = cg.esphome_ns.namespace('philips_series_5400')
PhilipsSeries5400 = philips_series_5400_ns.class_(
    'PhilipsSeries5400', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PhilipsSeries5400),
    cv.Required(DISPLAY_UART_ID): cv.use_id(UARTComponent),
    cv.Required(MAINBOARD_UART_ID): cv.use_id(UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    display = yield cg.get_variable(config[DISPLAY_UART_ID])
    mainboard = yield cg.get_variable(config[MAINBOARD_UART_ID])
    cg.add(var.register_display_uart(display))
    cg.add(var.register_mainboard_uart(mainboard))

