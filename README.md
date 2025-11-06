# Portafolio de Proyectos de Eliot Izquierdo Balencia

¡Hola! 👋 Bienvenido a mi portafolio de ingeniería. Soy un apasionado de la robótica, el control automático y el diseño mecánico.

Este repositorio sirve como una colección centralizada de mis proyectos académicos y personales más relevantes, demostrando mis habilidades en la creación de soluciones desde el concepto y el diseño CAD hasta la implementación de hardware (PCB) y software (firmware).

---

## 🚀 Habilidades y Tecnologías

Aquí hay un resumen de las tecnologías y habilidades que encontrarás en mis proyectos:

* **Diseño Electrónico (EDA):** Diseño de PCBs en **KiCad**, selección de componentes (reguladores, drivers, microcontroladores).
* **Diseño Mecánico (CAD):** Diseño de chasis y mecanismos en **SolidWorks**.
* **Simulación de Circuitos:** Análisis de configuraciones (serie, paralelo, mixto) en **Proteus**.
* **Fabricación Digital:** Impresión 3D (FDM) para prototipado rápido de chasis y piezas mecánicas.
* **Programación (Firmware):** C++ (para **Arduino** / **ESP32**), Python.
* **Control y Robótica:** Algoritmos de **Cinemática Inversa (IK)**, Control de Procesos (Térmico), Algoritmos de Navegación (Wall-Follower), control de motores (Puentes H, Servos).
* **Sensores:** Integración de Termopares (SPI), Sensores ToF (I2C), Sensores Analógicos (IR).
* **Análisis y Simulación (FEA):** Experiencia básica en simulación de esfuerzos y análisis estático en **SolidWorks Simulation**.

---

## 📂 Índice de Proyectos

Navega por las carpetas para ver los detalles, el código y la documentación técnica de cada proyecto.

### 1. Proyectos Escolares (School Proyects)
Proyectos desarrollados durante mi formación en el **CETI**, enfocados en la aplicación práctica de la teoría de control e instrumentación.

* **[Horno](./School%20Proyects/Horno)**
    * **Descripción:** Diseño y construcción de un horno eléctrico para calentar alimentos (lasaña) en el menor tiempo posible, aplicando la **Ley de Joule**. Se realizó un análisis de circuitos en Proteus para optimizar la configuración de las resistencias.
    * **Habilidades:** Control de procesos térmicos (Control ON-OFF con histéresis de $\pm5^{\circ}C$) , integración de hardware de potencia (Relés, Termopar Tipo K vía SPI), HMI (LCD I2C) y construcción refractaria.
aa
* **[Maze_Robot](./School%20Proyects/Maze_Robot)**
    * **Descripción:** Robot autónomo diseñado para resolver laberintos. Utiliza un algoritmo de **seguimiento de pared (wall-follower)**  que prioriza giros basado en la data de múltiples sensores.
    * **Habilidades:** Fusión de sensores (Láser ToF VL53L0X para detección frontal , IR Analógicos CNY70 para detección lateral y de suelo), programación de algoritmos de navegación y control de motores PWM.

* **[Robot_3DOF](./School%20Proyects/Robot_3DOF)**
    * **Descripción:** Implementación de un algoritmo de **Cinemática Inversa (IK)** geométrica  para controlar un brazo robótico de 3 Grados de Libertad (GDL) mediante coordenadas cartesianas (X, Y, Z).
    * **Habilidades:** Matemáticas aplicadas (Ley de Cosenos) , programación de algoritmos de control, SolidWorks.

* **[Reactor_Batch](./School%20Proyects/Reactor_Batch)**
    * **Descripción:** Automatización de un reactor químico batch a escala. Incluye un dispensador de pastillas (piñón-cremallera) diseñado en CAD e impreso en 3D, y un sistema de agitación y bombeo controlado por Arduino.
    * **Habilidades:** Control de procesos, diseño mecánico (engranajes), integración de hardware (servos, bombas), HMI (LCD I2C).

* **[Sumo_Rob_10x10 / 20x20](./School%20Proyects/Sumo_Rob_10x10)**
    * **Descripción:** Diseño integral (hardware y software) de un robot de sumo de competencia.Se diseñó un chasis optimizado (centro de gravedad bajo)y una PCB personalizada en KiCad para integrar los sensores (VL53L0X) y los drivers de motor.
    * **Habilidades:** Diseño de PCB (KiCad), diseño de chasis (Impresión 3D), integración de sensores (I2C), gestión de potencia.

### 2. Diseños Mecánicos (Mechanical Design)
Una colección de diseños conceptuales y prácticos realizados en SolidWorks, enfocados en la manufactura y el ensamblaje.

* **[Sumo_Designs](./Mechanical%20Design/Sumo_Designs)**
    * **Descripción:** Conceptos y prototipos de chasis para robots de sumo, optimizados para impresión 3D y bajo centro de gravedad.
* **[UGV Chassis Concept](./Mechanical%20Design/UGV%20Chassis%20Concept)**
    * **Descripción:** Concepto de un chasis para un Vehículo Terrestre No Tripulado (UGV), enfocado en la modularidad y resistencia.
* **[Closet](./Mechanical%20Design/Closet)**
    * **Descripción:** Diseño de mobiliario (clóset) enfocado en la optimización de espacio y ensamblaje.
* **[Practice](./Mechanical%20Design/Practice)**
    * **Descripción:** Archivos de práctica y ejercicios varios de modelado en SolidWorks.

---

## 📫 Contacto
* **Email:** eliotizq@gmail.com
* **LinkedIn:** www.linkedin.com/in/eliot-izquierdo-85326a372
