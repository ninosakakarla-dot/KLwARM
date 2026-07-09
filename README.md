# ANWR (Android Native Windows Runtime)

## Informe Técnico y Propuesta de Desarrollo

**Versión:** 0.1 (Conceptual)

---

## Resumen Ejecutivo

**ANWR (Android Native Windows Runtime)** es un proyecto cuyo objetivo es crear un runtime nativo para Android ARM64 capaz de ejecutar aplicaciones de Windows (.exe, .dll y otros formatos PE) con el mayor rendimiento posible.

No pretende ser un emulador tradicional, una máquina virtual ni un APK que encapsule otras herramientas. Su filosofía es construir un motor de ejecución optimizado que aproveche directamente el kernel de Android y las bibliotecas nativas del sistema.

El objetivo principal es eliminar la mayor cantidad posible de capas de software que actualmente limitan el rendimiento de soluciones existentes como Winlator.

---

## Filosofía

> "No emular cuando exista una alternativa nativa."

ANWR utilizará funciones nativas de Android siempre que sea posible y solo traducirá o implementará aquello que sea estrictamente necesario.

---

## Objetivos Principales

El proyecto ANWR se centra en alcanzar los siguientes objetivos clave para garantizar una experiencia de usuario óptima y un rendimiento superior:

- Ejecutar aplicaciones Windows de forma eficiente.
- Compatibilidad total con archivos EXE y DLL.
- Soporte robusto para aplicaciones Win32.
- Arquitectura altamente optimizada para procesadores ARM64.
- Alcanzar el máximo rendimiento posible en dispositivos móviles.
- Mantener una baja latencia en todas las operaciones.
- Asegurar un bajo consumo de memoria RAM.
- Uso eficiente de la batería del dispositivo.
- Aprovechamiento completo y directo del hardware de Android.

---

## Qué NO será ANWR

Para entender mejor la visión de ANWR, es importante aclarar lo que el proyecto **no** pretende ser:

- Un emulador tradicional.
- Una máquina virtual completa.
- Una instalación del sistema operativo Windows.
- Un APK que ejecute un entorno Linux completo en segundo plano.
- Un sistema basado en PRoot.

---

## Arquitectura y Funcionamiento

El flujo de ejecución de una aplicación en ANWR está diseñado para ser lo más directo posible, minimizando las capas intermedias:

1. **Aplicación Windows**
2. **PE Loader**
3. **Traductor x86/x64 → ARM64**
4. **Runtime Win32 nativo**
5. **Motor ANWR**
6. **Syscalls Linux / Android**
7. **Kernel Android**
8. **Hardware**

---

## Instalación y Ejecución

El núcleo de ANWR será un ejecutable ELF nativo para Android ARM64, diseñado para ejecutarse directamente sobre Android utilizando sus bibliotecas nativas y comunicándose con el kernel mediante syscalls estándar.

Durante las primeras fases de desarrollo, el motor podrá iniciarse desde Termux, que servirá como entorno de compilación, pruebas y ejecución. El objetivo a largo plazo es que ANWR pueda instalarse como un runtime nativo del sistema, independiente de Termux.

### Formas de Ejecución

#### Fase de Desarrollo
- Ejecución desde Termux.
- Compilación con herramientas nativas (Clang/LLVM, CMake, etc.).
- Depuración desde la terminal.

#### Fase de Producción
- Instalación mediante un instalador propio.
- Ejecución directa del binario ELF.
- Integración opcional con una interfaz gráfica (el motor seguirá siendo independiente del frontend).

### Filosofía de Desarrollo y Termux

Es importante destacar que **Termux no forma parte integral de ANWR**. Termux será únicamente una herramienta de desarrollo y ejecución inicial. El verdadero núcleo del proyecto será un ejecutable nativo para Android que funcionará independientemente del entorno desde el que sea lanzado.

---

## Componentes del Sistema

ANWR está compuesto por varios módulos independientes y altamente especializados:

### 1. PE Loader
Encargado de la lectura de archivos PE, resolución de dependencias DLL, creación del espacio de memoria necesario y el inicio del proceso de la aplicación.

### 2. Traductor Híbrido (AOT + JIT)
Utiliza una arquitectura combinada de compilación Ahead-of-Time (AOT) y Just-in-Time (JIT). Sus funciones incluyen la traducción previa del código, mantenimiento de una caché persistente, traducción dinámica solo cuando es necesario y optimización continua. El objetivo es evitar traducir el mismo código más de una vez.

### 3. Runtime Win32
Una implementación propia y nativa para ARM64 de las bibliotecas esenciales de Windows, incluyendo `kernel32`, `ntdll`, `user32`, `gdi32`, `shell32`, `advapi32` y otras DLL necesarias. **No utilizará DLL originales de Windows.**

### 4. Motor Gráfico
El flujo gráfico se optimiza traduciendo las llamadas de DirectX a través de DXVK/VKD3D hacia Vulkan, y finalmente a la GPU de Android. Incluye caché persistente de shaders, compilación previa, baja latencia y uso directo de `ANativeWindow`.

### 5. Motor de Audio
Utiliza la API nativa `AAudio` de Android para garantizar baja latencia, evitar emulaciones innecesarias y mantener compatibilidad con las APIs de audio de Windows.

### 6. Entrada (Input)
Soporte completo para pantalla táctil, ratón, teclado y gamepads.

### 7. Gestión de Memoria
Uso eficiente de recursos del sistema mediante `mmap`, `mprotect`, `futex`, `epoll` y `io_uring` (cuando esté disponible).

### 8. Sistema de Archivos
Acceso directo mediante syscalls, manteniendo la compatibilidad con las rutas de archivos de Windows.

### 9. Red
Implementación de Winsock utilizando directamente la pila TCP/IP nativa de Linux.

---

## Eliminación de Capas y Optimización

ANWR se distingue por eliminar capas redundantes que afectan el rendimiento, tales como PRoot, RootFS de Ubuntu y traducciones innecesarias.

### Integración Directa con Android
El runtime hace uso directo de APIs nativas y modernas de Android:
- Vulkan
- AAudio
- ANativeWindow
- EGL
- Performance Hint API (ADPF)
- Thermal API
- Game Mode API
- Binder (cuando sea apropiado)

### Caché Inteligente
Para maximizar el rendimiento, ANWR almacenará código traducido, shaders, DLL procesadas, configuraciones de optimización e información del perfil de ejecución.

### Perfilador Inteligente
El runtime aprenderá dinámicamente qué funciones utiliza más la aplicación, qué bloques requieren mayor optimización y qué DLL deben precargarse para mejorar los tiempos de carga.

---

## Comparativa: ANWR vs Winlator

| Característica | Winlator | ANWR |
| :--- | :--- | :--- |
| **Arquitectura** | Múltiples capas, componentes independientes. | Runtime unificado, específico para Android ARM64. |
| **Dependencias** | Históricamente depende de PRoot y rootfs Linux. | Sin PRoot como objetivo de diseño. Comunicación directa con el kernel (syscalls). |
| **APIs** | Capas de compatibilidad estándar. | Uso directo de APIs nativas de Android (Vulkan, AAudio, ADPF). |
| **Traducción** | Traducción estándar. | Caché AOT + JIT con optimización basada en perfiles. |
| **Rendimiento** | Optimización general. | Menor latencia, menor consumo de memoria y energético. |

---

## Posibles Integraciones Futuras

Cuando estén disponibles, ANWR podría integrarse con:
- **Shizuku:** Para acceder a determinadas capacidades del sistema sin requerir acceso root.
- **Root opcional:** Para habilitar funciones avanzadas.

*Nota: El runtime no dependerá de estas herramientas para su funcionamiento básico.*

---

## Objetivo Final

El propósito último de ANWR es crear el runtime para Android más eficiente y avanzado para ejecutar aplicaciones Windows. La estrategia no es competir añadiendo más capas de software, sino eliminándolas. No se trata de recrear un PC dentro del teléfono, sino de aprovechar directamente Android, su kernel y su hardware para que las aplicaciones Windows se ejecuten con un rendimiento lo más cercano posible al nativo.

ANWR buscará convertirse en una nueva generación de compatibilidad Windows sobre Android, diseñada desde el inicio para ARM64, con una arquitectura modular, optimizada y preparada para evolucionar durante años.
