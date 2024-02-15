# Software de control de la placa EDU-CIAA-NXP para su uso en el proyecto MAW

## IMPORTANTE

** Se realizaron modificaciones al software con el fin de que este responda correctamente al hardware utilizado al momento de testeo

** El código alojado en este repositorio es el utilizado por la EDU-CIAA exclusivamente

## Sobre el proyecto MAW

** MAW (Motorized Arm WiFi) es un trabajo universitario que se desarrolló durante la cursada de la materia Taller de Proyecto I, de la carrera de Ingeniería en Computación de la Universidad Nacional de La Plata (UNLP). La consigna de la cursada era diseñar un proyecto que utilizase la EDU-CIAA-NXP, con el fin de aprender a trabajar con microcontroladores. El objetivo principal fue el de construir un robot que pudiese desplazarse mediante el uso de 4 motores puestos en la base del mismo. A su vez, como objetivo secundario, se planteó la creación de un brazo montado en el auto, el cual le brindaría la posibilidad de agarrar y sujetar cosas. Además, se incluyó una cámara y conexión WiFi, todo a través del chip ESP32-CAM, para la comunicación desde una PC de forma inalámbrica. El trabajo que cumplía la EDU-CIAA en este proyecto era el de recibir y enviar mensajes al ESP32-CAM a través de la UART, decodificar los mensajes recibidos desde el chip, y realizar diferentes movimientos utilizando los motores del robot. 

** Todo el código desarrollado se basó e implementó sobre el framework "Firmware_v3", creado por el proyecto CIAA (Computadora Industrial Abierta Argentina). 

**La librería MAW fue creada y desarrollada por Guerrico Leonel, Ossola Florencia, y Pérez Balcedo Octavio