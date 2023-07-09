import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor, text_sensor, number, select, button
from esphome.automation import maybe_simple_id
from esphome.const import CONF_ID
from esphome.const import (
    CONF_ID,
    CONF_NUMBER,
    CONF_CURRENT,
    CONF_VALUE,
    CONF_INITIAL_VALUE,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_STEP,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_ACCURACY_DECIMALS,
    CONF_RESTORE_VALUE,
    CONF_ICON,
)
DEPENDENCIES = ['uart','sensor']
AUTO_LOAD = ['sensor','text_sensor','number','select','button']

philips_series_5400_ns = cg.esphome_ns.namespace('philips_series_5400')
PhilipsSeries5400 = philips_series_5400_ns.class_('PhilipsSeries5400', cg.Component)
PhilipsSeries5400Select = philips_series_5400_ns.class_('PhilipsSeries5400Select', select.Select)
PhilipsSeries5400Number = philips_series_5400_ns.class_('PhilipsSeries5400Number', number.Number, cg.Component)
PhilipsSeries5400Button = philips_series_5400_ns.class_('PhilipsSeries5400Button', button.Button, cg.Component)

DISPLAY_UART_ID = 'display_uart'
MAINBOARD_UART_ID = 'mainboard_uart'
STATE_SENSOR = 'state_sensor'
MESS_B0 = "B0_msg"
MESS_B5 = "B5_msg"
MESS_BA = "BA_msg"
MESS_90 = "90_msg"
MESS_91 = "91_msg"
MESS_93 = "93_msg"
TEXT_PRODUCT =  "Display_product"
COFFEE_VOL = "Coffee_volume"
MILK_VOL = "Milk_volume"
UNIT_MIL = "ml"
STERENGTH = "Strength_select"
CUPS = "Cups_select"
PRODUCT = "Product_select"
START_KEY = "Start_key"
TEST_KEY = "Test_key"
ICON_CURRENT_STATE = "mdi:state-machine"
ICON_COFFEE_VOL = "mdi:coffee"
ICON_MILK_VOL = "mdi:coffee-outline"
ICON_PRODUCT = "mdi:coffee-maker-check"
ICON_STERENGTH = "mdi:spoon-sugar"
ICON_CUPS = "mdi:beaker-plus"
ICON_START_KEY = "mdi:coffee-to-go"
ICON_TEXT_PRODUCT = "mdi:list-status"

OPTIONS_STERENGTH = [
    "Soft",
    "Medium",
    "Strong",
    "Ground"
]

OPTIONS_PRODUCT = [
    "Espresso",
    "Coffee to go",
    "Luingo",
    "Black Coffee",
    "Сaffe Crema",
    "Ristretto",
    "Americano",
    "Coffee With Milk",
    "Latte",
    "Milk Coffee to go",
    "Latte Macchiato",
    "Сappuccino",
    "Milk foam",
    "Hot water"
]

OPTIONS_CUPS = [
    "One cup",
    "Two cups",
    "ExtraShot",
]

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PhilipsSeries5400),
    cv.Required(DISPLAY_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(MAINBOARD_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_RESTORE_VALUE, default=False): cv.boolean,
    cv.Optional(STATE_SENSOR): sensor.sensor_schema(icon=ICON_CURRENT_STATE),
    cv.Optional(MESS_B0): text_sensor.text_sensor_schema(),
    cv.Optional(MESS_B5): text_sensor.text_sensor_schema(),
    cv.Optional(MESS_BA): text_sensor.text_sensor_schema(),
    cv.Optional(MESS_90): text_sensor.text_sensor_schema(),
    cv.Optional(MESS_91): text_sensor.text_sensor_schema(),
    cv.Optional(MESS_93): text_sensor.text_sensor_schema(),
    cv.Optional(TEXT_PRODUCT): text_sensor.text_sensor_schema(icon=ICON_TEXT_PRODUCT),
    #  объем кофе Required
    cv.Optional(COFFEE_VOL): number.NUMBER_SCHEMA.extend(
      {
        cv.GenerateID(): cv.declare_id(PhilipsSeries5400Number),
        cv.Optional(CONF_VALUE, default=100): cv.positive_int,
        cv.Optional(CONF_MAX_VALUE, default=500): cv.positive_int,
        cv.Optional(CONF_MIN_VALUE, default=40): cv.positive_int,
        cv.Optional(CONF_STEP, default=10): cv.positive_int,
        cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_MIL): str, 
        cv.Optional(CONF_ACCURACY_DECIMALS, default=0): cv.positive_int,
        cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
        cv.Optional(CONF_ICON, default=ICON_COFFEE_VOL): cv.icon,
      }
    ),
    #  объем молока
    cv.Optional(MILK_VOL): number.NUMBER_SCHEMA.extend(
      {
        cv.GenerateID(): cv.declare_id(PhilipsSeries5400Number),
        cv.Optional(CONF_VALUE, default=100):cv.positive_int,
        cv.Optional(CONF_MAX_VALUE, default=500): cv.positive_int,
        cv.Optional(CONF_MIN_VALUE, default=40): cv.positive_int,
        cv.Optional(CONF_STEP, default=10): cv.positive_int,
        cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_MIL): str, 
        cv.Optional(CONF_ACCURACY_DECIMALS, default=0): cv.positive_int,
        cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
        cv.Optional(CONF_ICON, default=ICON_MILK_VOL): cv.icon,
      }
    ),
    # крепость/помол
    cv.Optional(STERENGTH): select.SELECT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PhilipsSeries5400Select),
            cv.Optional(CONF_ICON, default=ICON_STERENGTH): cv.icon,
        }
    ),
    # количество чашек
    cv.Optional(CUPS): select.SELECT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PhilipsSeries5400Select),
            cv.Optional(CONF_ICON, default=ICON_CUPS): cv.icon,
        }
    ),
    # напиток
    cv.Optional(PRODUCT): select.SELECT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PhilipsSeries5400Select),
            cv.Optional(CONF_ICON, default=ICON_PRODUCT): cv.icon,
        }
    ),
    # кнопка запуска приготовления
    cv.Optional(START_KEY): button.BUTTON_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PhilipsSeries5400Button),
            cv.Optional(CONF_ICON, default=ICON_START_KEY): cv.icon,
        }
    ),
    # кнопка запуска приготовления
    cv.Optional(TEST_KEY): button.BUTTON_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(PhilipsSeries5400Button),
        }
    ),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    display = await cg.get_variable(config[DISPLAY_UART_ID])
    cg.add(var.register_display_uart(display))
    mainboard = await cg.get_variable(config[MAINBOARD_UART_ID])
    cg.add(var.register_mainboard_uart(mainboard))
    if STATE_SENSOR in config:
        sens = await sensor.new_sensor(config[STATE_SENSOR])
        cg.add(var.register_work_sensor(sens))
    if MESS_B0 in config:
        sens = await text_sensor.new_text_sensor(config[MESS_B0])
        cg.add(var.set_sens_b0(sens))
    if MESS_B5 in config:
        sens = await text_sensor.new_text_sensor(config[MESS_B5])
        cg.add(var.set_sens_b5(sens))
    if MESS_BA in config:
        sens = await text_sensor.new_text_sensor(config[MESS_BA])
        cg.add(var.set_sens_ba(sens))
    if MESS_90 in config:
        sens = await text_sensor.new_text_sensor(config[MESS_90])
        cg.add(var.set_sens_90(sens))
    if MESS_91 in config:
        sens = await text_sensor.new_text_sensor(config[MESS_91])
        cg.add(var.set_sens_91(sens))
    if MESS_93 in config:
        sens = await text_sensor.new_text_sensor(config[MESS_93])
        cg.add(var.set_sens_93(sens))
    if TEXT_PRODUCT in config:
        sens = await text_sensor.new_text_sensor(config[TEXT_PRODUCT])
        cg.add(var.set_sens_product(sens))
    if COFFEE_VOL in config:
        sens = await number.new_number(config[COFFEE_VOL],min_value=config[COFFEE_VOL][CONF_MIN_VALUE], max_value=config[COFFEE_VOL][CONF_MAX_VALUE], step=config[COFFEE_VOL][CONF_STEP])
        cg.add(var.set_vol_number(sens))
    if MILK_VOL in config:
        #sens = await number.new_number(config[MILK_VOL],min_value=40, max_value=350, step=10)
        sens = await number.new_number(config[MILK_VOL],min_value=config[MILK_VOL][CONF_MIN_VALUE], max_value=config[MILK_VOL][CONF_MAX_VALUE], step=config[MILK_VOL][CONF_STEP])
        cg.add(var.set_milk_number(sens))
    if STERENGTH in config:
        sel = await select.new_select(config[STERENGTH],options=OPTIONS_STERENGTH)
        cg.add(var.set_grind_select(sel))
    if CUPS in config:
        sel = await select.new_select(config[CUPS], options=OPTIONS_CUPS)
        cg.add(var.set_cups_select(sel))
    if PRODUCT in config:
        sel = await select.new_select(config[PRODUCT], options=OPTIONS_PRODUCT)
        cg.add(var.set_product_select(sel))
    if START_KEY in config:
        but = await button.new_button(config[START_KEY])
        cg.add(var.set_start_button(but))
    if TEST_KEY in config:
        but = await button.new_button(config[TEST_KEY])
        cg.add(var.set_test_button(but))
    if CONF_RESTORE_VALUE in config:
        cg.add(var.set_store_settings(config[CONF_RESTORE_VALUE]))
        
