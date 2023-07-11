### [Read in English here](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/README_EN.md)

# ESP-Philips-5400-Coffee-Machine

<img src="https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Image/Coffee%20machine%20philips%205400.jpg" width=50%>


***

### Выражаю благодарность участникам чата [ESPHome](https://t.me/esphome) и [Умный дом с диваном](https://t.me/smart_home_divan), а также огромное спасибо [Brokly](https://github.com/Brokly) за неоценимый вклад в данный проект, который написал код для управления кофемашиной. 

***


Модификация и удаленное управление кофемашиной Philips 5400. Управление реализовано средствами UART и ESPHome.

> Важно!
Все материалы этого проекта (прошивки, схемы, 3D модели и т.п.) предоставляются "КАК ЕСТЬ". Всё, что вы делаете с вашим оборудованием, вы делаете на свой страх и риск. Автор не несет ответственности за результат и ничего не гарантирует. Модификация кофемашины требует вмешательства, что лишит автоматически вас гарантии на новую кофемашину.

> Рабочие варианты кода для управления кофемашиной можно взять [**здесь**](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/Config/ESPHome) (читайте комментарии в коде)

***

<details>
  <summary>Что умеет делать интеграция?</summary>
  


  
1) Включать и выключать кофемашину
2) Выбрать кофейный напиток
   * Эспрессо
   * Эспрессо лунго
   * Кофе
   * Американо
   * Капучино
   * Латте макиато
   * Кофе с молоком
   * Кофе латте
   * Кафе крема
   * Ристретто
   * Кофе в дорогу
   * Кофе с молоком в дорогу
   * Молочная пена
   * Горячая вода
3) Выбрать крепость кофе
4) Выбрать зерно или молотый
5) Выбрать количестиво порции, в том числе и ExtraShot
6) Выбрать объем кофе
7) Выбрать объем молока
8) Отправлять собственные рецепты кофейных напитков
9) Автоматическая работа отвода воды от носика кофемашины
10) Калибровка сервопривода для отвода воды
11) Сенсоры
    * Поддон
    * Зерно
    * Вода в контейнере
    * Контейнер кофейной гущи
    * Статус питания кофемашины (включена/выключена)
    * Статусы кофемашины (приготовление, наливаем кофе, наливаем молоко, перемалываем зерна и т.д)
    * Сенсор ошибки
    * Сенсор 90 для отображения пакета рецепта кофейных напитков, этот пакет можете использовать для создания своего любимого рецепта кофейного напитка
    * Сенсор выбранного кофейного напитка, обема кофе и молока

</details>

<details>
  <summary>Добавление компонета Philips 5400 в ESPHome</summary>
  
```
external_components:
  - source:
      type: git
      url: https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine

```

При желании можно подключить компонент локально. Скачайте файлы в папку \esphome\components\ 

```
external_components:
  - source:
      type: local
      path: components
    components: [philips_series_5400]

```

</details>


<details>
  <summary>Примеры отправки собственных кофейных напитков</summary>
  

> Это чисто рекомендация и уведомление. Решение принимать вам. У данной модели любые изменения параметра напитка сохраняются в профиль, кроме гостевого профиля. Чтобы не перезаписывать профили, можете выбрать гостевой профиль и посылая рецепты на приготовление, изменения не будут перезаписаны. Когда отправляете любой напиток, на экране не будет отображаться выбранный вами напиток, а будет отображаться тот напиток, который вы выбирали последний раз на панели управления кофемашины. Если не трогали экран, то по умолчанию будет выбираться первый напиток, это эспрессо   

> Меняем только протокол с сообщением 0x90, а именно 0x00, 0x01, 0x00, 0x02, 0x02, 0x00, 0x78, 0x00, 0x00, 0x00 на другое, то что скопируете с сенсора Messge2 90, отсеиваем только первые 3 байта

Вы можете создавать любые рецепты используя таблицу [**протокола кофейных напитков**](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/raw/main/Protocol/Протокол%20кофейных%20напитков%20и%20программы.xlsx) в Excel

```
button:

#Пример кода для отправки кофейного напитка Кофе Max 150ml. Пакет рецепта копируется с сенсора Messge2 90, отсеиваем только первые 3 байта
  - platform: template
    name: "Кофе Max 150ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x00, 0x01, 0x00, 0x02, 0x02, 0x00, 0x78, 0x00, 0x00, 0x00};
            id(philip)->coffee_start(data);
            id(philip)->coffee_start();
          }


#Пример кода для отправки кофейного напитка Американо молотый 90ml. Пакет рецепта копируется с сенсора Messge2 90, отсеиваем только первые 3 байта
  - platform: template
    name: "Американо молотый 90ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x01, 0x03, 0x00, 0x02, 0x02, 0x00, 0x28, 0x00, 0x5A, 0x00};
            id(philip)->coffee_start(data);
            id(philip)->coffee_start();
          }


#Пример кода для отправки кофейного напитка Кофе с молоком 120/120ml. Пакет рецепта копируется с сенсора Messge2 90, отсеиваем только первые 3 байта
  - platform: template
    name: "Кофе с молоком 120/120ml"
    icon: mdi:coffee
    on_press:
      - lambda: |-
          {
            uint8_t data[]={0x02, 0x02, 0x00, 0x02, 0x01, 0x02, 0x78, 0x00, 0x78, 0x00};
            id(philip)->coffee_start(data);
            id(philip)->coffee_start();
          }


```
</details>


<details>
  <summary>Встраиваем ESP</summary>
  

Снимаем панель управления. Снимается не сложно. Чуть тянем на себя, в эти точки вставляем пластиковую лопатку и вынимаем панель

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/0f3e22b8-0776-405b-bf2b-5c5324b051ac)

Нам нужен доступ к 8-ми жильному проводу, который нужно подключить к ESP. 

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/cd569697-1ced-4b16-ac21-827a1ac3e4fa)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/118b8a3c-e5d5-4894-8583-bf670426cbc6)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/fbe10219-ceaf-488d-8d06-9a2ef610ab8a)



> Подключаем по схеме на выбор, через реле или через оптопару. Сервопривод подключаем по желанию, он нужен для работы отвода воды от носика.

### Общая схема подключения без реле и оптопары. Просто подключение ESP32 к кофемашине

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/1a65ecf7-6f34-4c4a-af81-90b342f1b7ab)

***

### Схема с подключением реле 5В (KY-019)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/26c08fe2-4f6e-4091-8a43-2accfa30202f)


***

### Cхема с подключением оптопары PC817C

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/078c2dd7-b1b1-457a-beca-5363308cc4e7)

</details>


<details>
  <summary>Разбираем панель управления</summary>
  

Почему именно я разобрал панель управления и подключил реле, и почему не оптопару? Были попытки найти решение включить кофемашину без разборки панели управления. Пробовал размыкать питание через транзистор, пробовал посылать посылать команды, но это было безуспешно. 

Подключал резистор по такой схеме, но этот способ работает на кофемашине серии 2000/3000, но не подошел для серии 5400. Для справки, есть два проекта для кофемашин серии 2000, это [ESPHome-Philips-Smart-Coffee]( https://github.com/TillFleisch/ESPHome-Philips-Smart-Coffee) и [SmartPhilips2200](https://github.com/chris7topher/SmartPhilips2200)


Снимаем наклейку с чипа и видим маркировку с названием чипа STM32L4R5VIT6

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/a32d6c85-50c0-4cb3-9915-75f51548c80b)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/ef5c6cf3-88ba-4818-af39-2f7e0acacf31)


В документации чипа [STMicroelectronics STM32L4R5VIT6](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Documentation/STMicroelectronics%20STM32L4R5VIT6.pdf) находим наш чип, это STM32L4R5VIT6 LQFP100 (25 ножек с каждой стороны)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/458b99de-6ae4-4e56-91af-290e9a8a22bf)

В документации пишется, что управлять кофемашиной байтами можно после включения, поэтому у меня и не получилось включить ее удаленно. Возможно я ошибаюсь и если будет найдено такое решение, то обязательно внесу поправку

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b12e1f48-acc0-46ef-9765-d01f36280aad)


***

Для имитации работы сенсорной кнопки можно использовать модуль реле одноканальный 5В (KY-019) или оптопару PC817C, которые будут подавать GND на резистор R110, к которому идет дорожка от кнопки питания, тем самым включая и выключая кофемашину. Я не стал рисковать припаивать провод после резистора R110 (номинал резистора 4,7 кОм), так как после резистора идет прямая дорожка к ножке чипа, а лампе к конденсаторам и я не знаю как чип может на это отреагировать, но, если так сделать, то тогда у нас будет возможность включать кофемашину через сенсорную кнопку и удаленно. Это было проверенно на сенсорной лампе, где я припаял провода после резистора и я мог включать и выключать удаленно, а также включать и выключать лампу нажимая на сенсорную кнопку. На кофемашине я не решаюсь это повторить.

Панель управления в разборе

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/c9a994ab-043e-4092-803a-bacb02b91225)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/63bc8542-9252-4bd6-b452-7ee600c4b414)


Дорожка от кнопки питания к резистору R110(номинал резистора 4,7 кОм), после резистора дорожка идет к [диоду шоттки BAS70TW (K73)](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Documentation/Диод%20шоттки%20BAS70TW%20(K73).pdf)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/b14f005a-8cfc-4f2e-bc42-3e10a04805e2)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/9db3f4e8-af1f-4b18-bb6b-6c4651da5c48)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0a10714-c257-4404-b8ac-f260329e28e9)


Припаиваем провод к резистору R110(номинал резистора 4,7 кОм)

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/8644fbca-61ac-40d1-880c-c2900e92bc23)


Делаем отверстие в крышке и выносим провод наружу, который подключим к реле или к оптопаре

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/efb5ec12-5842-4f79-9b0a-1510859a50c1)


  
</details>


<details>
  <summary>Расшифровка протокола</summary>

> Мне удалось расшифровать часть протоколов, если вы найдете что-то новое, сообщите мне об этом обновлю информацию. Создайте ишью и опишите подробно про протокол. 

Краткое описание функции протоколов

* **AA:AA:AA:90** - рецепты кофейных напитков
* **AA:AA:AA:91** - счетчик приготовления кофейных напитков
* **AA:AA:AA:93** - счетчик приготовления кофейных напитков
* **AA:AA:AA:B0** - системные сенсоры
* **AA:AA:AA:B5** - счетчик объема кофе и молока для приготовления кофейных напитков
* **AA:AA:AA:BA** - не смог расшифровать
* **AA:AA:AA:BB** - не смог расшифровать
* **AA:AA:AA:FF** - данные с ЖК экрана
* **AA:AA:AA:FE** - включение и выключение кофемашины

***

**AA:AA:AA:FE** - этот пакет AA:AA:AA:FE:00:00:C8:87:1B:40:55 встречается два раза, в самом начале, он идет первым, когда включаем кофемашину и при выключении


**AA:AA:AA:90** - протокол кофейных напитков

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/7b29c0fb-bd64-4ac5-ada4-545cee055631)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/14cb5080-d1c7-4e7a-bd56-0d2b7406c032)


**AA:AA:AA:91** - протокол счетчика кофейных напитков. Это просто учет сделанных кофейных напитков и учет сбрасывается, когда кофемашина выключается. Сделали одну чашку кофе, сработал счетчик и выставил допустим 1, потом сделали еще чашку кофе, снова сработал счетчик и выставил 2. Чтобы удаленно запускать приготовление кофейных напимтков, нужно запускать на +1 к тому, что в счетчике. Без учета этого счетчика не будет срабатывать приготовление кофейных напитков удаленно

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/1a834ee8-3264-4f91-9807-771a8789348d)

**AA:AA:AA:B0** - протокол системных сенсоров: контейнер кофейной гущи, воды, поддона, анличие зерна, статусы приготовления кофе, статусы заварного блока

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/04255199-1344-4441-a08b-1f0ea3ae3cf9)

**AA:AA:AA:B5** - протокол счетчика кофейных напитков. Считает в секундах объем налитого кофе и молока

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/db3e16f0-f321-4857-acc7-f7ade6bc8b76)


</details>


<details>
  <summary>Отвод воды носика кофемашины</summary>

Я разработал для кофемашины отвод воды от носика. Зачем это нужно? Когда включаем кофемашину, то при включении запускается прогрев и промывка. Во время промывки большая часть воды проходит внутри, а часть выливается через носик. Что делать, если мы хотим, чтобы утром кофемашина приготовила нам свежее кофе и чтобы в кофе не было воды после промывки? Отвод воды позволяет решить эту проблему. Когда кофемашина включается с помощью умного дома, то отовод воды все еще остается у носика вполть до того момента, когда кофемашина не передейдет в режим "Выбор напитка", только после этого отвод воды прячется во внутрь носика, открывая проход носику.

Готовую модель для распечатки на 3д принтере можете [здесь](https://www.thingiverse.com/thing:6073849) или [скачать с этого проекта](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/3D%20Model). Файлы формата FCStd можете открыть в программе FreeCAD. 

Используется сервопривод MG90S Micro Servo 180 градусов. Его вполне достаточно для отвода воды. Питаем от +3,3в от самого ESP, что является большим плюсом. Провод от сервопривода во внутрь протягивается просто, без сверления корпуса. Протискиваем провод во внутрь держателя носика и тянем к внутренней части корпуса к ESP


![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0c68b4d-ef07-49d4-b393-7d04fa2275e1)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/2c6377c9-9f77-45af-b3f6-386a4772ad36)

</details>

<details>
  <summary>Панель управления в Home Assistant</summary>
  

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/6253a91c-9479-494c-ad8e-605d8406baed)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/3526ea87-a62f-4d7d-8639-a5bcad70a0b5)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/ae9311fd-a209-48bc-9537-510c5df05789)


Скопируйте код [отсюда](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/blob/main/Config/Home%20Assistant/coffee_machine_philips_5400_ru.yaml), а после создайте карточку

Код карточки
```
entity: sensor.coffee_machine_power_status
type: custom:multiple-entity-row
name: Статус
icon: mdi:coffee-maker
state_color: true
show_state: false
secondary_info:
  attribute: Making coffee
entities:
  - entity: group.kitchen_coffee_machine_info_and_menu
    name: Меню
    state_color: true
    icon: mdi:information-outline
    styles:
      height: 60px
      width: 50px
  - entity: group.kitchen_coffee_machine_coffee_drinks
    name: Напитки
    state_color: true
    icon: mdi:coffee
    styles:
      height: 60px
      width: 50px
  - entity: switch.coffee_philips_5400_valve_open_closed
    name: Клапан
    type: switch
    tap_action:
      action: toggle
    icon: mdi:coffee-maker-outline
    state_color: true
    styles:
      height: 60px
      width: 50px
  - entity: binary_sensor.coffee_philips_5400_power_status
    name: Питание
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




</details>
