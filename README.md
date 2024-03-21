# Tesis sobre Monitoreo de Material Particulado Fino

	Este proyecto de tesis desarrolla un sistema de monitoreo de material particulado fino utilizando el sensor Sensirion SPS30 junto con una placa de desarrollo Nucleo STM32F429 y un módulo RTC (Reloj en Tiempo Real), entre otras tecnologías. El sistema es capaz de medir concentraciones de partículas PM en el ambiente y registrar los datos con marcas de tiempo precisas, proporcionando una herramienta valiosa para estudios ambientales y de calidad del aire.

## Objetivos

	La tesis se propuso los siguientes objetivos principales:

    Diseñar y desarrollar un sistema integrado para la monitorización en tiempo real de partículas PM, utilizando tecnología de punta en sensores y microcontroladores.
    Implementar una solución eficiente para la recopilación y almacenamiento de datos ambientales, con la capacidad de realizar un seguimiento temporal a través del uso de un módulo RTC.
    Analizar los datos recogidos para identificar patrones o eventos de contaminación significativos.

## Tecnologías y Herramientas Utilizadas

    Sensor de Partículas PM: Sensirion SPS30, capaz de medir la calidad del aire detectando concentraciones de partículas finas.
    Plataforma de Desarrollo: Nucleo STM32F429, un potente microcontrolador para procesamiento y gestión de datos.
    Módulo RTC: Para el registro preciso de la fecha y hora de cada medición.
    Software y Programación: Desarrollo realizado en el entorno de STM32CubeIDE, programación en C/C++ para la gestión de sensores y datos.

## Resultados

El sistema desarrollado demostró ser eficaz en la monitorización precisa y continua de la calidad del aire en términos de material particulado. Se implementó con éxito una interfaz para la visualización de datos en tiempo real y se estableció un protocolo para el almacenamiento y análisis posterior de los datos recogidos.

## Estructura del Repositorio

    src/: Contiene todo el código fuente necesario para la implementación del sistema de monitoreo.
    docs/: Documentación técnica, incluyendo esquemas de circuitos y detalles de implementación.
    data/: Ejemplos de datos recogidos durante las pruebas del sistema.

Instalación y Configuración

Para instalar y configurar el sistema de monitoreo en tu propio entorno, sigue los pasos detallados en la sección de documentación. Se requiere un entorno de desarrollo STM32CubeIDE y las bibliotecas específicas para el sensor SPS30 y el módulo RTC.
Contribuciones

Las contribuciones a este proyecto son bienvenidas. Si tienes sugerencias de mejora, correcciones o quieres adaptar el sistema para otro tipo de análisis ambiental, no dudes en abrir un issue o enviar un pull request.
Contacto

Para preguntas específicas sobre el proyecto o colaboraciones, puedes contactar al autor a través de [tu correo electrónico o medio de contacto preferido].
