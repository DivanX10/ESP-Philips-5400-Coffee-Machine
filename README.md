# ESP-Philips-5400-Coffee-Machine

Модификация и удаленное управление кофемашиной Philips 5400. Управление реализовано средствами ESPHome. Проект будет дорабатываться и дополняться новыми сведениями. Вы можете принять участие в развитии проекта, например написать код для имитации счетчика чтобы можно было готовить кофе много раз, а не один раз после включения. Можете переписать компонент Philips 5400 и довести до ума.

> Важно!
Все материалы этого проекта (прошивки, схемы, 3D модели и т.п.) предоставляются "КАК ЕСТЬ". Всё, что вы делаете с вашим оборудованием, вы делаете на свой страх и риск. Автор не несет ответственности за результат и ничего не гарантирует. Модификация кофемашины требует вмешательства, что лишит автоматически вас гарантии на новую кофемашину.


Что на текущий момент реализовано?

1) Разобрал протокол кофейных напитков и сенсоров
2) Реализовал удаленное включение и выключение
3) Написал код для вывода различных сенсоров и приготовления кофейного напитка
4) Отвод воды от носика

Что не реализовано? 
1) Написать код, который бы имитровал работу счетчка и можно было отправлять команду приготовление кофейного напитка много раз. Нужна помощь. Я не знаю как это сделать. Я не являюсь программистом и не имею опыта в реверсинжиниринге, это мой первый опыт и проект. Благодаря поддержки сообщества моего чата [Умный дом с Диваном](https://t.me/smart_home_divan) я смог расшифровать и реализовать удаленное управление кофемашиной


***

Добавление компонета Philips 5400 в ESPHome. 

> Для справки, этот компонет без сенсоров, выключателей, кнопок и прочее, он нужен только для работы кофемашины в режиме UART. Приветствуется ваше участие в доработке или написании полноценного рабочего компонента.

```
external_components:
  - source:
      type: git
      url: https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine


philips_series_5400:
  display_uart: uart_display
  mainboard_uart: uart_mainboard
  id: philip
```

При желании можно подключить компонент локально. Скачайте файлы в папку \esphome\components\ 

```
external_components:
  - source:
      type: local
      path: components
    components: [philips_series_5400]


philips_series_5400:
  display_uart: uart_display
  mainboard_uart: uart_mainboard
  id: philip
```

Варианты кода можете посмотреть [здесь](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/Config) 

Что в коде реализовано?
1) Включить и выключить кофемашину
2) Сенсоры
    * Контейнер гущи переполнен или пустой
    * Контейнер с водой пустой или полон
    * Поддон извлечен или на месте
    * Кофемашина включена или выключен
    * Статусы кофемашины
    * Служебные сенсоры, такие как отображение байтов для приготовления кофейных напитков
    * Сенсор положения сервопривода
      
3) Открыть и закрыть носик вручную. Если не планируется наливать кофе автоматически по утрам 
4) Открывать и закрывать носик автоматически. Если планируется наливать кофе автоматически по утрам
5) Калибровка отвода воды, указываете положение сервы в начальной и конечной позиции
6) Кофейные напитки

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/3407d4f8-8e15-43f8-8747-89d7cd9d7ea0)


***

<details>
  <summary>Снимаем панель управления и встраиваем ESP</summary>
  

Снимаем панель управления. Снимается не сложно. Чуть тянем на себя, в эти точки вставляем пластиковую лопатку и вынимаем панель

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/0f3e22b8-0776-405b-bf2b-5c5324b051ac)

Нам нужен доступ к 8-ми жильному проводу, который нужно подключить к ESP. 

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/cd569697-1ced-4b16-ac21-827a1ac3e4fa)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/118b8a3c-e5d5-4894-8583-bf670426cbc6)



> Подключаем по схеме на выбор, через реле или через оптопару. Сервопривод подключаем по желанию, он нужен для работы отвода воды от носика.

Схема с подключением реле 5В (KY-019)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/a08ea89c-9619-41c4-b76c-21c3b38398b4)


Схема с подключением оптопары PC817C
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/201a46dd-f403-4837-9bf1-bdd2e24656c3)


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
  
Протокол кофейного напитка

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/fb5b52b9-8ee8-4a7c-8ce9-da41e7258a64)


Протокол системных сенсоров

![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/04f8501a-e74d-45f7-926c-012d7e0556a4)

Протокол счетчика кофейных напитков. Это важная часть, без него не будет срабатывать приготовление кофейных напитков удаленно



</details>



<details>
  <summary>Отвод воды носика кофемашины</summary>

Я разработал для кофемашины отвод воды от носика. Зачем это нужно? Когда включаем кофемашину, то при включении запускается прогрев и промывка. Во время промывки большая часть воды проходит внутри, а часть выливается через носик. Что делать, если мы хотим, чтобы утром кофемашина приготовила нам свежее кофе и чтобы в кофе не было воды после промывки? Отвод воды позволяет решить эту проблему. Когда кофемашина включается с помощью умного дома, то отовод воды все еще остается у носика вполть до того момента, когда кофемашина не передейдет в режим "Выбор напитка", только после этого отвод воды прячется во внутрь носика, открывая проход носику.

Готовую модель для распечатки на 3д принтере можете [здесь](https://www.thingiverse.com/thing:6073849) или [скачать с этого проекта](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/tree/main/3D%20Model). Файлы формата FCStd можете открыть в программе FreeCAD. 

Используется сервопривод MG90S Micro Servo 180 градусов. Его вполне достаточно для отвода воды. Питаем от +3,3в от самого ESP, что является большим плюсом. Провод от сервопривода во внутрь протягивается просто, без сверления корпуса. Протискиваем провод во внутрь держателя носика и тянем к внутренней части корпуса к ESP


![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/e0c68b4d-ef07-49d4-b393-7d04fa2275e1)
![image](https://github.com/DivanX10/ESP-Philips-5400-Coffee-Machine/assets/64090632/2c6377c9-9f77-45af-b3f6-386a4772ad36)



 
</details>

