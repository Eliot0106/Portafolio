# Implementación de Cinemática Inversa en Brazo Robótico Antropomórfico 3-GDL

[cite_start]Este proyecto demuestra el diseño, análisis matemático e implementación de un sistema de control de cinemática inversa para un brazo robótico de 3 Grados de Libertad (GDL)[cite: 192].

![Demostración del Robot](URL_DE_TU_VIDEO_O_GIF.gif)

---

## 1. Objetivo del Proyecto

El objetivo principal fue implementar un modelo de cinemática inversa que permitiera controlar el actuador final del robot especificando únicamente sus coordenadas cartesianas de destino (X, Y, Z). [cite_start]Esto abstrae la complejidad del control articular, permitiendo al robot interactuar con objetos en su espacio de trabajo de forma intuitiva[cite: 197].

---

## 2. Tecnologías y Componentes

* **Software de Control:** (Ej: Arduino IDE (C++), Python)
* **Microcontrolador:** (Ej: Arduino Nano, ESP32)
* **Actuadores:** (Ej: 3x Servomotores MG996R)
* **Diseño CAD:** SolidWorks
* **Matemáticas:** Análisis Geométrico, Trigonometría (Ley de Cosenos).

---

## 3. Diseño y Análisis: Derivación de la Cinemática Inversa

[cite_start]El control del robot se basa en un análisis geométrico para determinar los ángulos de las articulaciones (`q1`, `q2`, `q3`) necesarios para alcanzar una posición (`X`, `Y`, `Z`)[cite: 199].

### Paso 1: Cálculo del Ángulo de la Base (q1)

El primer ángulo, `q1`, controla la rotación de la base en el plano XY. [cite_start]Se calcula proyectando la posición del actuador final en este plano, formando un triángulo rectángulo con los ejes X e Y[cite: 203, 204, 205].

* Proyección en el plano XY: `$Y_1 = \sqrt{X^2 + Y^2}$`
* Cálculo del ángulo (usando la tangente inversa de dos argumentos): `$q_1 = \text{atan2}(Y, X)$`

*(Nota: `atan2(Y, X)` es más robusto que `tan-1(Y/X)` porque maneja los cuatro cuadrantes correctamente).*

[cite_start]![Diagrama 3D](URL_DE_TU_IMAGEN_DEL_DIAGRAMA_3D.png) [cite: 202]

### Paso 2: Proyección del Brazo en el Plano 2D

Una vez que la base ha girado, el resto del problema se simplifica a un análisis 2D en el plano vertical formado por la proyección `$Y_1$` y el eje `Z`.

[cite_start]Primero, definimos la posición relativa del actuador final (`X, Y, Z`) con respecto a la segunda articulación (la que está al final del eslabón L1)[cite: 207, 208].

* Distancia vertical relativa: `$Z_1 = Z - L_1$`
* Distancia horizontal relativa: `$Y_1$` (calculada en el paso 1)

[cite_start]![Diagrama 2D](URL_DE_TU_IMAGEN_DEL_DIAGRAMA_2D.png) [cite: 207]

### Paso 3: Cálculo de Ángulos del Brazo (q2 y q3)

Usando la vista 2D, formamos dos triángulos. [cite_start]Primero, calculamos la hipotenusa `H` (la distancia directa desde la segunda articulación al actuador final) usando Pitágoras[cite: 209].

* `$H = \sqrt{Y_1^2 + Z_1^2}$`

[cite_start]Luego, usamos la **Ley de Cosenos** para resolver los ángulos internos `a2` y `a3` del triángulo oblicuo formado por los eslabones `L2`, `L3` y la hipotenusa `H`[cite: 210, 211].

* `$a_2 = \cos^{-1}\left(\frac{L_2^2 + H^2 - L_3^2}{2 \cdot L_2 \cdot H}\right)$`
* `$a_3 = \cos^{-1}\left(\frac{L_2^2 + L_3^2 - H^2}{2 \cdot L_2 \cdot L_3}\right)$`

[cite_start]Finalmente, calculamos los ángulos de las articulaciones `q2` y `q3` sumando los ángulos internos[cite: 210, 211, 212].

* El ángulo `a1` es el ángulo de la hipotenusa H: `$a_1 = \text{atan2}(Z_1, Y_1)$`
* El ángulo total `O` es: `$O = a_1 + a_2$`
* **Ángulo q2:** `$q_2 = 180^\circ - O$` (Ajustado según la posición "cero" del servo)
* **Ángulo q3:** `$q_3 = 180^\circ - a_3$` (Ajustado según la posición "cero" del servo)

---
#Referencias
Proyectos JC. (2021, 8 septiembre). Introducción Historia de los robots [Video]. YouTube.
https://www.youtube.com/watch?v=y3guAp-l_SA