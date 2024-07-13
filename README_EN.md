# ESP-Philips-5400-Coffee-Machine

<img src="https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Image/Coffee%20machine%20philips%205400.jpg" width=50%>


Modification and remote control of the Philips 5400 coffee machine. Management is implemented by means of UART and ESPHome

> Important!
All materials of this project (firmware, schematics, 3D models, etc.) are provided "AS IS". Everything you do with your equipment, you do at your own risk. The author is not responsible for the result and does not guarantee anything. Modification of the coffee machine requires intervention, which will automatically deprive you of the warranty for a new coffee machine.

> Working versions of the code for controlling the coffee machine can be taken [**here**](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/Config/ESPHome) (read the comments in the code)


***

Important! To assemble the firmware strictly in ESP Home version 2023.5.0 or 2023.6.0 and no later, as well as specify a specific version of arduino 2.0.9 in the configuration. If you build the firmware in the latest version of ESP Home and use arduino recommended, the coffee machine will not work correctly. We need to rewrite the code. If you want to rewrite the code for the new version of ESPHome and share it, then I will be grateful for your participation. Thank you.

```
esp32:
  board: esp32dev
  framework:
    type: arduino
    version: 2.0.9
```
<details>
  <summary>How do I install any version of the ESPHome addon?</summary>

1) You can download the required version of the ESPHome addon at this [link](https://github.com/esphome/home-assistant-addon)
 * Download the addon ESPHome 2023.5.0 (17.05.2023) [here](https://github.com/esphome/home-assistant-addon/releases/tag/2023.5.0)
 * Download the addon ESPHome 2023.6.0 (22.06.2023) [here](https://github.com/esphome/home-assistant-addon/releases/tag/2023.6.0)
2) Unzip the archive to any location so that it can be copied to Home Assistant
3) Copy the **esphome * * folder from the archive to the **addons**network folder. If the Home Assistant network folders don't work, then install the **Samba share**addon. The **Samba share * * addon is available by default in the addon store, but if the **Samba share** addon is not available for some reason, then enable advanced mode, and then you will see the **Samba share** addon in the addon store, and if this addon is still not available, then you can install the **Samba share * * addon using this [link](https://github.com/home-assistant/addons/tree/master/samba)

 * How do I enable Advanced Mode?   
![image](https://github.com/user-attachments/assets/0efaf883-cce8-4e3d-9ad4-0ae38dad29aa)

 * Where can I download the * * esphome * * file?

In the archive * * home-assistant-addon-xx.xx.xxxx** we are interested in the folder **esphome**
![image](https://github.com/user-attachments/assets/4d96e995-13ae-44bc-8bff-d169037fed94)

Copy the **esphome * * folder to the network folder * * addons**
![image](https://github.com/user-attachments/assets/181f986a-63e0-47c2-bdbe-1d4da5c700e2)
![image](https://github.com/user-attachments/assets/134eb78d-8e72-4233-9981-ad74c2360942)

4) Restart Home Assistant
5) Go to the addon store and look for the local version of the ESPHome addon. We are looking at the version, it should be version 2023.5.0 or 2023.6.0, depending on which archive we downloaded
![image](https://github.com/user-attachments/assets/e2447a8c-62f2-47ae-b113-de76d73b4a92)

8) If the local version of the ESPHome addon is also version 2023.5.0 or 2023.6.0, then set
![image](https://github.com/user-attachments/assets/f57fc227-9d3d-4e6d-9a8e-6907508006b7)
</details>

***

<details>
  <summary>What can integration do?</summary>

  
1) Turn on and off the coffee machine
2) Choose a coffee drink
   * Espresso
   * Espresso lungo
   * Coffee
   * Americano
   * Cappuccino
   * Latte macchiato
   * Coffee with milk
   * Latte coffee
   * Cream Cafe
   * Ristretto
   * Coffee on the road
   * Coffee with milk on the road
   * Milk foam
   * Hot water
3) Choose the strength of coffee
4) Choose grain or ground
5) Choose the number of servings, including ExtraShot
6) Choose the volume of coffee
7) Choose the volume of milk
8) Send your own coffee drink recipes
9) Automatic operation of water removal from the spout of the coffee machine
10) Calibration of the servo for water drainage
11) Sensors
    * Pallet
    * Grain
    * Water in the container
    * Coffee grounds container
    * Coffee machine power status (on/off)
    * Coffee machine statuses (cooking, pouring coffee, pouring milk, grinding grains, etc.)
    * Error sensor
    * Sensor 90 to display the coffee drink recipe package, you can use this package to create your favorite coffee drink recipe
    * Sensor of the selected coffee drink, coffee volume and milk

</details>


<details>
  <summary>Adding the Philips 5400 component to ESPHome</summary>
  

```
external_components:
  - source:
      type: git
      url: https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine

```

If desired, you can connect the component locally. Download the files to the \esphome\components folder\

```
external_components:
  - source:
      type: local
      path: components
    components: [philips_series_5400]

```

</details>

<details>
  <summary>Examples of sending your own coffee drinks</summary>
  

> This is purely a recommendation and notification. The decision is up to you. For this model, any changes to the drink parameter are saved to the profile, except for the guest profile. In order not to overwrite profiles, you can select a guest profile and sending recipes for cooking, the changes will not be overwritten. When you send any drink, the screen will not display the drink you selected, but the drink that you selected last time on the control panel of the coffee machine will be displayed. If you didn't touch the screen, then the first drink will be selected by default, this is espresso

If you turn on the Message 2 90 sensor, a package of coffee drink will be displayed in it. You can use this for your recipes. Just copy all bytes except the first three (example in the screenshot, highlighted in red)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b4fef99d-b5d1-46cc-bbdc-5ba6d225e16a)

You can create any recipes using the table [**protocol of coffee drinks**](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/raw/main/Protocol/Protocol%20of%20coffee%20drinks%20and%20programs.xlsx) in Excel

```
button:

#Sample code for sending a Max 150ml Coffee drink. The recipe package is copied from the Message2 90 sensor, we filter out only the first 3 bytes
#Drink: Coffee. Grain: Maximum. Coffee: 150 ml.
  - platform: template
    name: "Coffee Max 150ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x00, 0x03, 0x00, 0x02, 0x02, 0x00, 0x96, 0x00, 0x00, 0x00};
            id(philip)->prepare(data);
          }


#Sample code for sending a coffee drink Americano ground 90ml. The recipe package is copied from the Message2 90 sensor, we filter out only the first 3 bytes
  - platform: template
    name: "Americano ground 80ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x01, 0x00, 0x00, 0x02, 0x03, 0x00, 0x28, 0x00, 0x28, 0x00};
            id(philip)->prepare(data);
          }


#Sample code for sending a coffee drink Coffee with milk 150/120ml. The recipe package is copied from the Message2 90 sensor, we filter out only the first 3 bytes
  - platform: template
    name: "Coffee with milk Max 150/120ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x02, 0x03, 0x00, 0x02, 0x01, 0x02, 0x96, 0x00, 0x78, 0x00};
            id(philip)->prepare(data);
          }

```
</details>


<details>
  <summary>Embedding ESP</summary>
  

Remove the control panel. It is not difficult to remove. We pull a little on ourselves, insert a plastic spatula into these points and take out the panel

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/0f3e22b8-0776-405b-bf2b-5c5324b051ac)

We need access to an 8-core wire that needs to be connected to the ESP

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/f433b783-f307-41b9-9d6c-ddccb81b2268)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/118b8a3c-e5d5-4894-8583-bf670426cbc6)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/fbe10219-ceaf-488d-8d06-9a2ef610ab8a)



> We connect according to the scheme of your choice, through a relay or through an optocoupler. We connect the servo at will, it is needed for the operation of water removal from the spout

### General connection diagram without relays and optocouplers. Just connect the ESP32 to the coffee machine

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/6dde0d4b-8f04-4954-a36b-3b61f4ecc263)


***

### Circuit with connection of a 5V relay (KY-019)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/eb48ac76-0167-478c-a466-63a20d8332ee)


***

### PC817C optocoupler connection diagram

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/6c79a9bd-295a-4396-9e55-329af4cb2479)


</details>


<details>
  <summary>Disassembling the control panel</summary>
  

Why exactly did I disassemble the control panel and connect the relay, and why not the optocoupler? There were attempts to find a solution to turn on the coffee machine without disassembling the control panel. I tried to disconnect the power supply through the transistor, tried to send commands, but it was unsuccessful.

I connected the resistor according to this scheme, but this method works on a 2000/3000 series coffee machine, but it did not work for the 5400 series. For reference, there are two projects for 2000 series coffee machines, these are [ESPHome-Philips-Smart-Coffee](https://github.com/TillFleisch/ESPHome-Philips-Smart-Coffee) and [SmartPhilips2200](https://github.com/chris7topher/SmartPhilips2200)

We remove the sticker from the chip and see the marking with the name of the chip STM32L4R5VIT6

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/a32d6c85-50c0-4cb3-9915-75f51548c80b)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/ef5c6cf3-88ba-4818-af39-2f7e0acacf31)

In the chip documentation [STMicroelectronics STM32L4R5VIT6](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Documentation/STMicroelectronics%20STM32L4R5VIT6.pdf) we find our chip, it is STM32L4R5VIT6 LQFP100 (25 legs on each side)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/458b99de-6ae4-4e56-91af-290e9a8a22bf)

The documentation says that you can control the coffee machine with bytes after turning it on, so I couldn't turn it on remotely. Maybe I'm wrong and if such a solution is found, I will definitely make an amendment.

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b12e1f48-acc0-46ef-9765-d01f36280aad)


***

To simulate the operation of the touch button, you can use a single-channel 5V relay module (KY-019) or an optocoupler PC817C, which will supply GND to the resistor R110, to which there is a track from the power button, thereby turning the coffee machine on and off. I did not risk soldering the wire after the resistor R110 (the nominal value of the resistor is 4.7 kOhm), since after the resistor there is a direct path to the chip leg, and the lamp to the capacitors, and I do not know how the chip can react to this, but if so, then we will be able to turn on the coffee machine through the touch button and remotely. This was tested on a touch lamp, where I soldered the wires after the resistor and I could turn it on and off remotely, as well as turn the lamp on and off by pressing the touch button. I don't dare repeat it on the coffee machine.

Control panel in parsing

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/c9a994ab-043e-4092-803a-bacb02b91225)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/63bc8542-9252-4bd6-b452-7ee600c4b414)


The track from the power button to the resistor R110 (the nominal value of the resistor is 4.7 kOhm), after the resistor the track goes to [Schottky diode BAS70TW (K73)](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Documentation/Диод%20шоттки%20BAS70TW%20(K73).pdf)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b14f005a-8cfc-4f2e-bc42-3e10a04805e2)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/9db3f4e8-af1f-4b18-bb6b-6c4651da5c48)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0a10714-c257-4404-b8ac-f260329e28e9)


Solder the wire to the resistor R110 (the nominal value of the resistor is 4.7 kOhm)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/8644fbca-61ac-40d1-880c-c2900e92bc23)


We make a hole in the lid and take the wire out, which we connect to the relay or to the optocoupler

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/8ad93af5-c652-428f-bab1-76f07a676e03)



</details>


<details>
  <summary>Decryption of the protocol</summary>

> I managed to decrypt some of the protocols, if you find something new, let me know about it and I will update the information. Create an issues and describe in detail about the protocol.

Brief description of the protocol function

* **AA:AA:AA:90** - coffee drink recipes
* **AA:AA:AA:91** - coffee beverage preparation counter
* **AA:AA:AA:93** - coffee beverage preparation counter
* **AA:AA:AA:B0** - system sensors
* **AA:AA:AA:B5** - coffee and milk volume counter for making coffee drinks
* **AA:AA:AA:BA** - couldn't decipher
* **AA:AA:AA:BB** - couldn't decipher
* **AA:AA:AA:FF** - couldn't decipher
* **AA:AA:AA:FE** - turning the coffee machine on and off

***

**AA:AA:AA:FE** - this package AA:AA:AA:FE:00:00:C8:87:1B:40:55 occurs twice, at the very beginning, it goes first when turning on the coffee machine and when turning off


**AA:AA:AA:90** - coffee drink recipes

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/fc43f9f1-3155-4813-84ad-c3978fed41cc)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/bc3260f5-ba94-49b4-8448-e8c7f73873e2)


**AA:AA:AA:91** - protocol of the coffee drinks counter. It's just a record of the coffee drinks made and the record is reset when the coffee machine turns off. We made one cup of coffee, the counter worked and set for example 1, then we made another cup of coffee, the counter worked again and set 2. In order to remotely start the preparation of coffee drinks, you need to run at +1 to what is in the counter. Without taking into account this counter, the preparation of coffee drinks remotely will not work

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/f72f3394-b41d-47f6-8d43-6e5266021999)


**AA:AA:AA:B0** - protocol of system sensors: container of coffee grounds, water, tray, presence of grain, coffee preparation statuses, custard block statuses

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/ea0f03eb-0cfd-4d86-b0ca-d164f7efcb1b)


**AA:AA:AA:B5** - protocol of the coffee drinks counter. Counts the volume of poured coffee and milk in seconds

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/06e32b5e-5455-46e5-9f86-b6d76ef755a6)



</details>


<details>
  <summary>Water drain of the coffee machine spout</summary>

I have developed a water outlet from the spout for the coffee machine. Why is this necessary? When we turn on the coffee machine, then when it is turned on, the heating and flushing starts. During flushing, most of the water passes inside, and some is poured out through the spout. What should we do if we want the coffee machine to make us fresh coffee in the morning and that there is no water in the coffee after washing? Water drainage allows you to solve this problem. When the coffee machine is turned on with the help of a smart home, the water supply still remains at the spout until the moment when the coffee machine switches to the "Drink selection" mode, only after that the water outlet hides inside the spout, opening the passage to the spout.

You can find a ready-made model for printing on a 3d printer [here](https://www.thingiverse.com/thing:6073849) or [download from this project](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/3D%20Model). You can open FCStd files in the FreeCAD program.

The MG90S Micro Servo 180 degree servo is used. It is quite enough to drain water. Powered by +3.3V from the ESP itself, which is a big plus. The wire from the servo is pulled inside simply, without drilling the housing. We pull the wire into the inside of the spout holder and pull it to the inside of the case to the ESP


![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0c68b4d-ef07-49d4-b393-7d04fa2275e1)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/2c6377c9-9f77-45af-b3f6-386a4772ad36)



</details>

<details>
  <summary>Control Panel in Home Assistant</summary>
  

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0d4b538-72ec-44f3-9131-2e53ded775b5)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/9ee4dd36-76c5-4716-9ea5-f99f4018eeed)


Copy the code [from here](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Config/Home%20Assistant/coffee_machine_philips_5400_en.yaml) , and then create a card

Card code
```
entity: sensor.coffee_machine_power_status
type: custom:multiple-entity-row
name: Status
icon: mdi:coffee-maker
state_color: true
show_state: false
secondary_info:
  attribute: Making coffee
entities:
  - entity: group.kitchen_coffee_machine_info_and_menu
    name: Menu
    state_color: true
    icon: mdi:information-outline
    styles:
      height: 60px
      width: 50px
  - entity: group.kitchen_coffee_machine_coffee_drinks
    name: Drinks
    state_color: true
    icon: mdi:coffee
    styles:
      height: 60px
      width: 50px
  - entity: switch.coffee_philips_5400_valve_open_closed
    name: Valve
    type: switch
    tap_action:
      action: toggle
    icon: mdi:coffee-maker-outline
    state_color: true
    styles:
      height: 60px
      width: 50px
  - entity: binary_sensor.coffee_philips_5400_power_status
    name: Power
    icon: mdi:gesture-tap-button
    state_color: true
    tap_action:
      action: call-service
      service: script.kitchen_coffee_machine_power
    styles:
      height: 60px
      width: 50px

```
</details>
