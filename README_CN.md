# ESP-飞利浦5400咖啡机

<img src="https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Image/Coffee%20machine%20philips%205400.jpg" width=50%>


对飞利浦5400咖啡机进行改装和远程控制。通过UART和ESPHome实现控制

> 重要提示！
本项目的所有材料（固件、原理图、3D模型等）均按"原样"提供。您对自己设备所做的一切操作，风险自负。作者不对结果负责，也不做任何保证。改装咖啡机需要进行干预，这将自动使您失去新咖啡机的保修资格。

> 控制咖啡机的代码工作版本可从此处获取 [**这里**](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/Config/ESPHome)（请阅读代码中的注释）


***

重要提示！要严格使用ESP Home 2023.5.0或2023.6.0版本及更早版本编译固件，并在配置中指定特定版本的arduino 2.0.9。如果使用最新版本的ESP Home并使用推荐的arduino版本，咖啡机将无法正常工作。我们需要重写代码。如果您想为新版本的ESPHome重写代码并分享，我将非常感谢您的参与。谢谢。

```
esp32:
  board: esp32dev
  framework:
    type: arduino
    version: 2.0.9
```
<details>
  <summary>如何安装任何版本的ESPHome插件？</summary>

1) 您可以从此链接下载所需版本的ESPHome插件 [link](https://github.com/esphome/home-assistant-addon)
 * 下载ESPHome 2023.5.0插件（2023年5月17日）[here](https://github.com/esphome/home-assistant-addon/releases/tag/2023.5.0)
 * 下载ESPHome 2023.6.0插件（2023年6月22日）[here](https://github.com/esphome/home-assistant-addon/releases/tag/2023.6.0)
2) 将存档解压缩到任何位置，以便可以复制到Home Assistant
3) 将存档中的**esphome**文件夹复制到**addons**网络文件夹。如果Home Assistant网络文件夹不起作用，则安装**Samba share**插件。**Samba share**插件默认在插件商店中可用，但如果由于某种原因**Samba share**插件不可用，则启用高级模式，然后您将在插件商店中看到**Samba share**插件，如果此插件仍然不可用，则可以使用此链接安装**Samba share**插件 [link](https://github.com/home-assistant/addons/tree/master/samba)

 * 如何启用高级模式？  
![image](https://github.com/user-attachments/assets/6a9cbe3c-e43a-407b-843d-d4babc960446)


 * 在哪里可以下载**esphome**文件？

在**home-assistant-addon-xx.xx.xxxx**存档中，我们感兴趣的是**esphome**文件夹
![image](https://github.com/user-attachments/assets/4d96e995-13ae-44bc-8bff-d169037fed94)

将**esphome**文件夹复制到网络文件夹**addons**
![image](https://github.com/user-attachments/assets/181f986a-63e0-47c2-bdbe-1d4da5c700e2)
![image](https://github.com/user-attachments/assets/134eb78d-8e72-4233-9981-ad74c2360942)

4) 重启Home Assistant
5) 进入插件商店，查找本地版本的ESPHome插件。查看版本，应为2023.5.0或2023.6.0版本，具体取决于我们下载的存档
![image](https://github.com/user-attachments/assets/e2447a8c-62f2-47ae-b113-de76d73b4a92)

8) 如果ESPHome插件的本地版本也是2023.5.0或2023.6.0版本，则进行设置
![image](https://github.com/user-attachments/assets/f57fc227-9d3d-4e6d-9a8e-6907508006b7)
</details>

***

<details>
  <summary>集成可以做什么？</summary>

  
1) 打开和关闭咖啡机
2) 选择咖啡饮品
   * 浓缩咖啡
   * 长浓缩咖啡
   * 咖啡
   * 美式咖啡
   * 卡布奇诺
   * 拿铁玛奇朵
   * 牛奶咖啡
   * 拿铁咖啡
   * 奶油咖啡
   * 芮斯崔朵
   * 随行咖啡
   * 随行牛奶咖啡
   * 牛奶泡沫
   * 热水
3) 选择咖啡浓度
4) 选择咖啡豆或研磨咖啡
5) 选择份数，包括ExtraShot
6) 选择咖啡量
7) 选择牛奶量
8) 发送自定义咖啡饮品配方
9) 自动操作咖啡机出水口排水
10) 排水伺服校准
11) 传感器
    * 托盘
    * 咖啡豆
    * 水箱中的水
    * 咖啡渣容器
    * 咖啡机电源状态（开/关）
    * 咖啡机状态（烹饪中、倒咖啡、倒牛奶、研磨咖啡豆等）
    * 错误传感器
    * 90号传感器显示咖啡饮品配方包，可使用此包创建您喜爱的咖啡饮品配方
    * 所选咖啡饮品、咖啡量和牛奶量的传感器

</details>


<details>
  <summary>将飞利浦5400组件添加到ESPHome</summary>
  
```
external_components:
  - source:
      type: git
      url: https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine

```

如果需要，可以本地连接组件。将文件下载到\\esphome\\components文件夹\\

```
external_components:
  - source:
      type: local
      path: components
    components: [philips_series_5400]

```

</details>

<details>
  <summary>发送自定义咖啡饮品的示例</summary>
  
> 这纯粹是建议和通知。决定权在您。对于此型号，除访客配置文件外，对饮品参数的任何更改都将保存到配置文件中。为了不覆盖配置文件，可以选择访客配置文件并发送烹饪配方，更改不会被覆盖。发送任何饮品时，屏幕不会显示您选择的饮品，而是显示您上次在咖啡机控制面板上选择的饮品。如果您没有触摸屏幕，则默认选择第一种饮品，即浓缩咖啡

如果启用Message 2 90传感器，其中将显示咖啡饮品的配方包，您可以使用此包创建自己喜爱的咖啡饮品配方

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b4fef99d-b5d1-46cc-bbdc-5ba6d225e16a)

您可以使用Excel中的[**咖啡饮品协议**](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/raw/main/Protocol/Protocol%20of%20coffee%20drinks%20and%20programs.xlsx)表创建任何配方

```
button:

#发送最大150ml咖啡饮品的示例代码。配方包从Message2 90传感器复制，我们只过滤掉前3个字节
#饮品：咖啡。咖啡豆：最大。咖啡：150 ml。
  - platform: template
    name: "Coffee Max 150ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x00, 0x03, 0x00, 0x02, 0x02, 0x00, 0x96, 0x00, 0x00, 0x00};
            id(philip)->prepare(data);
          }


#发送美式研磨咖啡80ml的示例代码。配方包从Message2 90传感器复制，我们只过滤掉前3个字节
  - platform: template
    name: "Americano ground 80ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x01, 0x00, 0x00, 0x02, 0x03, 0x00, 0x28, 0x00, 0x28, 0x00};
            id(philip)->prepare(data);
          }


#发送牛奶咖啡150/120ml的示例代码。配方包从Message2 90传感器复制，我们只过滤掉前3个字节
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
  <summary>嵌入ESP</summary>
  
取下控制面板。取下并不难。稍微向自己拉一下，将塑料刮刀插入这些点，然后取出面板

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/0f3e22b8-0776-405b-bf2b-5c5324b051ac)

我们需要接触到8芯线，需要将其连接到ESP

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/f433b783-f307-41b9-9d6c-ddccb81b2268)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/118b8a3c-e5d5-4894-8583-bf670426cbc6)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/fbe10219-ceaf-488d-8d06-9a2ef610ab8a)



> 我们根据您选择的方案进行连接，通过继电器或通过光耦合器。伺服电机根据需要连接，它用于从出水口排水

### 不带继电器和光耦合器的通用连接图。只需将ESP32连接到咖啡机

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/6dde0d4b-8f04-4954-a36b-3b61f4ecc263)


***

### 带5V继电器（KY-019）的连接电路

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/eb48ac76-0167-478c-a466-63a20d8332ee)


***

### PC817C光耦合器连接图

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/6c79a9bd-295a-4396-9e55-329af4cb2479)


</details>


<details>
  <summary>拆卸控制面板</summary>
  
为什么我偏偏要拆卸控制面板并连接继电器，而不是光耦合器？曾尝试找到无需拆卸控制面板即可打开咖啡机的解决方案。我尝试通过晶体管断开电源，尝试发送命令，但都没有成功。

我按照此方案连接了电阻，但这种方法在2000/3000系列咖啡机上有效，但在5400系列上无效。仅供参考，有两个针对2000系列咖啡机的项目，分别是[ESPHome-Philips-Smart-Coffee](https://github.com/TillFleisch/ESPHome-Philips-Smart-Coffee)和[SmartPhilips2200](https://github.com/chris7topher/SmartPhilips2200)

我们取下芯片上的贴纸，看到芯片名称标记STM32L4R5VIT6

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/a32d6c85-50c0-4cb3-9915-75f51548c80b)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/ef5c6cf3-88ba-4818-af39-2f7e0acacf31)

在芯片文档[STMicroelectronics STM32L4R5VIT6](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Documentation/STMicroelectronics%20STM32L4R5VIT6.pdf)中，我们找到了我们的芯片，它是STM32L4R5VIT6 LQFP100（每侧25个引脚）

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/458b99de-6ae4-4e56-91af-290e9a8a22bf)

文档中说，开机后可以用字节控制咖啡机，所以我无法远程开机。也许我错了，如果找到这样的解决方案，我一定会进行修正。

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b12e1f48-acc0-46ef-9765-d01f36280aad)


***

要模拟触摸按钮的操作，可以使用单通道5V继电器模块（KY-019）或光耦合器PC817C，它们将GND提供给电阻R110，电源按钮的线路连接到该电阻，从而打开和关闭咖啡机。我不敢在电阻R110（电阻的标称值为4.7 kOhm）之后焊接导线，因为电阻之后有直接通往芯片引脚的线路，以及通往电容器的指示灯，我不知道芯片对此会有什么反应，但如果是这样，那么我们将能够通过触摸按钮和远程打开咖啡机。这在触摸灯上进行了测试，我在电阻后焊接了导线，可以远程打开和关闭它，也可以通过按触摸按钮打开和关闭灯。我不敢在咖啡机上重复此操作。

解析中的控制面板

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/c9a994ab-043e-4092-803a-bacb02b91225)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/63bc8542-9252-4bd6-b452-7ee600c4b414)


从电源按钮到电阻R110（电阻的标称值为4.7 kOhm）的线路，电阻之后的线路通向[肖特基二极管BAS70TW (K73)](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Documentation/Диод%20шоттки%20BAS70TW%20(K73).pdf)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b14f005a-8cfc-4f2e-bc42-3e10a04805e2)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/9db3f4e8-af1f-4b18-bb6b-6c4651da5c48)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0a10714-c257-4404-b8ac-f260329e28e9)


将导线焊接到电阻R110（电阻的标称值为4.7 kOhm）

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/8644fbca-61ac-40d1-880c-c2900e92bc23)


在盖子上打一个孔，将导线引出，连接到继电器或光耦合器

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/8ad93af5-c652-428f-bab1-76f07a676e03)


</details>


<details>
  <summary>协议解密</summary>

> 我设法解密了部分协议，如果您发现新内容，请告诉我，我会更新信息。创建issues并详细描述协议。

协议功能简要说明

* **AA:AA:AA:90** - 咖啡饮品配方
* **AA:AA:AA:91** - 咖啡饮品制备计数器
* **AA:AA:AA:93** - 咖啡饮品制备计数器
* **AA:AA:AA:B0** - 系统传感器
* **AA:AA:AA:B5** - 制备咖啡饮品的咖啡和牛奶量计数器
* **AA:AA:AA:BA** - 无法解密
* **AA:AA:AA:BB** - 无法解密
* **AA:AA:AA:FF** - 无法解密
* **AA:AA:AA:FE** - 咖啡机开关

***

**AA:AA:AA:FE** - 此包AA:AA:AA:FE:00:00:C8:87:1B:40:55出现两次，在最开始，它在打开咖啡机和关闭时首先出现


**AA:AA:AA:90** - 咖啡饮品配方

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/fc43f9f1-3155-4813-84ad-c3978fed41cc)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/bc3260f5-ba94-49b4-8448-e8c7f73873e2)


**AA:AA:AA:91** - 咖啡饮品计数器协议。它只是记录所制备的咖啡饮品，并且在关闭咖啡机时重置记录。我们制作了一杯咖啡，计数器工作并设置为例如1，然后我们再制作一杯咖啡，计数器再次工作并设置为2。为了远程开始制备咖啡饮品，需要运行计数器中的+1。如果不考虑此计数器，远程制备咖啡饮品将无法工作

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/f72f3394-b41d-47f6-8d43-6e5266021999)


**AA:AA:AA:B0** - 系统传感器协议：咖啡渣容器、水、托盘、咖啡豆存在、咖啡制备状态、奶泡块状态

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/ea0f03eb-0cfd-4d86-b0ca-d164f7efcb1b)


**AA:AA:AA:B5** - 咖啡饮品计数器协议。以秒为单位计算倒入的咖啡和牛奶量

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/06e32b5e-5455-46e5-9f86-b6d76ef755a6)


</details>


<details>
  <summary>咖啡机出水口排水</summary>

我开发了咖啡机出水口的排水装置。为什么这是必要的？当我们打开咖啡机时，开机时会开始加热和冲洗。冲洗过程中，大部分水从内部流过，部分通过出水口流出。如果我们希望咖啡机在早上为我们制作新鲜咖啡，而冲洗后咖啡中没有水，该怎么办？排水可以解决这个问题。当通过智能家居打开咖啡机时，出水口的供水仍然保持，直到咖啡机切换到"饮品选择"模式，只有在此之后，出水口内部的排水口隐藏，打开通往出水口的通道。

您可以在[这里](https://www.thingiverse.com/thing:6073849)找到用于3D打印机的现成模型，或[从此项目下载](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/3D%20Model)。FCStd文件可以在FreeCAD程序中打开。

使用MG90S Micro Servo 180度舵机。它足以排水。由ESP本身的+3.3V供电，这是一个很大的优势。舵机的导线简单地从内部拉入，无需钻外壳。我们将导线拉入出水口支架内部，并将其拉到ESP的外壳内部

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0c68b4d-ef07-49d4-b393-7d04fa2275e1)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/2c6377c9-9f77-45af-b3f6-386a4772ad36)


</details>

<details>
  <summary>Home Assistant中的控制面板</summary>
  
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0d4b538-72ec-44f3-9131-2e53ded775b5)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/9ee4dd36-76c5-4716-9ea5-f99f4018eeed)


从[这里](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Config/Home%20Assistant/coffee_machine_philips_5400_en.yaml)复制代码，然后创建卡片

卡片代码
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