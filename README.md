# Gamepad bluetooth con ESP32 (XIAO ESP32-C3)
En este repositorio encuentras los archivos necesarios para construir un gamepad bluetooth retro basado en el microcontrolador ESP32, especificamente la versión de seeed studio [XIAO ESP32C3](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/).

En este [vídeo](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) encontrarás el paso a paso de la fabricación del gamepad.

## Materiales
La lista completa de materiales se puede ver [aquí](https://docs.google.com/spreadsheets/d/1ZOEywxfTi1w-vu1b5H4TEMHqUDYsI4aYrmyZVoHWpyY/edit?usp=sharing)

## Instalacion
- Primero es necesario instalar las tarjetas ESP32 de Espressif Systems por medio del gestor de tarjetas.
- Descargas la ultima versión de este repositorio como .zip.
- En el IDE de Arduino ir a "Sketch" -> "Incluir Librería" -> "Agregar librería .ZIP" a y seleccionar el archivo descargado.
- En el IDE de Arduino ir a "Herramientas" -> "Gestionar librerías..." -> Filtrar por "NimBLE-Arduino" del autor h2zero e instalar.
- Ahora ya puedes ir a "Archivo" -> "Ejemplos" -> "joycon" y seleccionar el ejemplo para empezar.
