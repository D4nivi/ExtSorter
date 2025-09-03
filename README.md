<div align="center">
    <img src="images/cabecera-main-window.png" width="70%" alt="Logo ExtSorter">
</div>

<div align="center">
    <br>
    <img src="https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white" alt="qt badge"/>
    <img src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="c++ badge"/>
</div>

---

***ExtSorter*** es una aplicación de escritorio desarrollada con Qt que permite organizar ficheros de forma sencilla a partir de sus extensiones.

El programa clasifica automáticamente los ficheros en categorías, como imágenes, audio, vídeo, documentos... y mueve todos los ficheros a una carpeta concreta según esta categoria. Tras organizar, el programa ofrece al usuario opciones para manipular estas carpetas de manera segura.

## Índice de contenidos
- [Showcase](#showcase)
- [Requisitos e Instalación](#requisitos-e-instalación)
- [Cómo usar](#cómo-usar)
	- [Casillas](#casillas)
	- [Ver extensiones y categorías](#ver-extensiones-y-categorías)
	- [Añadir/Eliminar nuevas categorías y extensiones](#añadireliminar-nuevas-categorías-y-extensiones)
    - [Preferencias](#preferencias)
- [Próximas características](#próximas-características)
- [Créditos](#créditos)

## Showcase

## Requisitos e Instalación
Actualmente el programa **solo es compatible con Windows**.

Puedes instalar la última versión del programa [aquí](insertar enlace a la release). El programa es un simple ejecutable, por lo que además de ser portable, **no es necesario instalar ninguna dependencia**.

Si decides ejecutar el programa desde el código fuente, necesitarás tener instalado:
- Qt 6 (y sus dependencias)
- Qt Creator

Para ejecutarlo, simplemente abre el proyecto con Qt Creator, constrúyelo y ejecútalo.

## Cómo usar
ExtSorter está pensado para usarse en **carpetas desordenadas** con una gran cantidad de ficheros (como _podrían_ ser el Escritorio o Descargas). La interfaz de ExtSorter hace que el programa sea muy intuitivo de usar, por lo que probablemente no necesites ver este apartado.

Para empezar a usar ExtSorter, selecciona una carpeta con el botón o desde el menú con ``Archivo -> Abrir``.

<div align="center">
    <img src="readme_images/Ventana principal.png" width="40%" alt="Ventana principal">
</div>
<div align="center"><i><b>Ventana principal</b></i></div><br>

Una vez seleccionada la carpeta, se pulsa el botón Comenzar. ExtSorter creará una **carpeta destino** dentro de la carpeta seleccionada, llamada ``ExtSorter`` por defecto, donde se guardarán las carpetas con los ficheros ordenados.

Hay ocasiones en las que el programa puede no continuar:
- La carpeta está vacía o no tiene ficheros (solo carpetas).
- El usuario no tiene permisos para leer/escribir la carpeta seleccionada.

>[!IMPORTANT]
>- El programa no toma en cuenta todas las extensiones de ficheros existentes, por lo que los ficheros cuya extensión no esté contemplada por el programa serán movidos a una carpeta llamada `Otros`.
>
>- Las carpetas llenas son ignoradas. Las carpetas vacías pueden eliminarse si se marca su opción (ver [_Casillas de opciones_](#casillas-de-opciones)).

Una vez se termina, saltará un mensaje avisando de que el proceso finalizó. Cerrar el cuadro hará que el programa pase a la ventana de manipulación de las carpetas (___Ventana Final___).

<div align="center">
    <img src="readme_images/Ventana final.png" width="40%" alt="Ventana Final">
</div>
<div align="center"><i><b>Ventana Final</b></i></div><br>

En esta ventana se puede decidir qué hacer con las carpetas que ha creado el programa; simplemente marca las casillas con las carpetas a manipular y selecciona la acción:
- ``Comprimir``: comprime las carpetas seleccionadas en un fichero `.zip`. El fichero se crea en la **carpeta destino**.
- ``Eliminar``: elimina las carpetas seleccionadas.
- ``Mover``: mueve las carpetas seleccionadas a otra carpeta, seleccionada por el usuario.

Cuando el número de ficheros dentro de las carpetas seleccionadas es muy grande (>= 100) las acciones ``Eliminar`` y ``Mover`` solicitarán una confirmación.

Dentro de esta ventana, también puedes:
- Abrir la carpeta destino (botón ``Abrir carpeta``).
- Salir del programa (botón ``Terminar``)
- Volver a la ventana principal (botón ``Volver``).

### Casillas
Antes de comenzar el programa, puedes seleccionar tres casillas en la ventana principal:
- ``Borrar carpetas vacías``: si se marca, el programa eliminará todas las carpetas vacías que encuentre en la carpeta seleccionada.

- ``Borrar accesos directos``: si se marca, el programa eliminará todos los accesos directos que encuentre en la carpeta seleccionada.

- ``Excluir Otros``: si se marca, el programa ignorará los ficheros cuya extensión no esté contemplada por el programa, en lugar de crear una carpeta para ellos.


### Ver extensiones y categorías
Para ver las categorías y extensiones del programa, ve al menú y dale a ``Ver -> Categorías y extensiones``. Se abrirá una ventana con una tabla con todas las categorías y extensiones.

<div align="center">
    <img src="readme_images/Ventana ver categorías y extensiones.png" width="60%" alt="Ventana Ver Categorías y Extensiones">
</div>
<div align="center"><i><b>Ventana Ver Categorías y Extensiones</b></i></div><br>

El programa cuenta por defecto con 7 categorías y 77 extensiones. En la siguiente tabla se muestran las categorías y extensiones predeterminadas del programa.

| Categoría   | Extensiones |
|-------------|-------------|
| **Audio**      | aac, ac3, aiff, aif, flac, m4a, mp3, ogg, pca, w64, wav, wma |
| **Código**     | asm, c, cc, cpp, cs, go, h, hpp, java, lua, pl, py, rb, rs, swift, ts |
| **Comprimido** | 7z, bz2, gz, rar, tar, zip |
| **Documento**  | accdb, doc, docm, docx, mpp, odg, odp, ods, odt, pdf, ppt, pptx, xls, xlsm, xlsx |
| **Ejecutable** | bat, cmd, exe, msi |
| **Imagen**     | bmp, dib, gif, jfif, jpeg, jpg, pbm, pgm, png, pnm, ppm, qtif, svg, tiff, webp |
| **Proyecto**   | ai, aep, aup3, kra, prproj, psd, veg, xcf |
| **Texto**      | csv, log, md, txt |
| **Video**      | avi, flv, m4v, mkv, mov, mp4, mpg, mxf, webm, wmv |

### Añadir/Eliminar nuevas categorías y extensiones
El programa permite personalizar por completo las categorías y las extensiones que se manejan. Puedes crear nuevas categorías aparte de las que vienen por defecto o eliminarlas y crear tus propias categorías con las extensiones que quieras.

- Para **crear** una nueva categoría, haz click en ``Archivo -> Añadir Categorías``. Se abrirá una ventana en la que podrás elegir el nombre y las extensiones para la nueva categoría.

<div align="center">
    <img src="readme_images/Ventana añadir categorías.png" width="40%" alt="Ventana Añadir categorías">
</div>
<div align="center"><i><b>Ventana Añadir categorías</b></i></div><br>

- Para **eliminar** una categoría, haz click en ``Archivo -> Eliminar Categorías``. Se abrirá una ventana en la que podrás seleccionar la categoría a eliminar o, si se prefiere, eliminarlas todas.

<div align="center">
    <img src="readme_images/Ventana eliminar categorías.png" width="40%" alt="Ventana Eliminar categorías">
</div>
<div align="center"><i><b>Ventana Eliminar categorías</b></i></div><br>

- Para **añadir nuevas extensiones** a una categoría, haz click en ``Archivo -> Añadir Extensiones``. Se abrirá una ventana en la que podrás seleccionar una de las categorías creadas para añadir nuevas extensiones.

<div align="center">
    <img src="readme_images/Ventana añadir extensiones.png" width="40%" alt="Ventana Añadir extensiones">
</div>
<div align="center"><i><b>Ventana Añadir extensiones</b></i></div><br>

- Para **restablecer** las categorías y extensiones a las predeterminadas, puede usarse el botón ``Restablecer`` dentro de las ventanas de ``Añadir Extensiones`` y ``Añadir Categorías``, o directamente desde ``Ver -> Restablecer extensiones``.


>[!NOTE]
>- El nombre de la categoría añadida será el nombre que tendrá la carpeta de dicha categoría. Por tanto, dicho nombre no puede estar entre los [nombres reservados por Windows ni contener caracteres prohibidos](https://learn.microsoft.com/es-es/windows/win32/fileio/naming-a-file#file-and-directory-names).
>
>- Las categorías predeterminadas tienen sus propios emojis que se muestran en la **Ventana Final**. Si se eliminan dichas categorías y se vuelven a crear con el mismo nombre, los emojis se seguirán mostrando.

### Preferencias
La ventana de preferencias puede ser accedida desde `Ver -> Preferencias`.

<div align="center">
    <img src="readme_images/Ventana preferencias.png" width="40%" alt="Ventana preferencias">
</div>
<div align="center"><i><b>Ventana Preferencias</b></i></div><br>

El programa permite modificar ciertos parámetros, que se explican a continuación.

#### General
- `Nombre carpeta destino`: Nombre de la carpeta en la que se guardan las carpetas con los ficheros ordenados.
    - **Valor por defecto**: `ExtSorter`.

- `Mostrar advertencias`: Indica si el usuario quiere recibir advertencias al realizar acciones importantes, como restablecer extensiones/preferencias o manipular cierto número de ficheros.
    - **Valor por defecto**: `Si`.

- `Ficheros mínimos para confirmación`: Número mínimo de ficheros que tienen que ser manipulados en la **Ventana Final** para que se pida una confirmación.
    - Rango desde `0-1000`. Si se ponen `0`, nunca se pedirá confirmación.
    - **Valor por defecto**: `100`.

#### Activar casillas
Estas opciones indican el **comportamiento predeterminado de las casillas** en la Ventana Principal:
- `Manual`: El usuario decide si marcar o no las casillas.
- `Siempre`: Las casillas se quedan marcadas por defecto.
- `Nunca`: Las casillas se quedan desmarcadas por defecto.
    - **Valor por defecto**: `Manual`.

## Próximas características
- [ ] Detección y tratamiento de ficheros duplicados.
- [ ] Compresión de ficheros con quazip.
- [ ] Port a Linux.
- [ ] Crear un desinstalador (para eliminar archivos de configuración).

## Créditos
Los iconos usados en el programa no son míos, fueron creados por otros autores y descargados desde [Flaticon](https://www.flaticon.com). En la siguiente tabla se acreditan a los creadores de cada icono. Los iconos marcados con un `*` fueron ligeramente modificados para la aplicación.

| Autor | Iconos |
|-------|--------|
| [Kerismaker](https://www.flaticon.com/authors/kerismaker) | <a href="https://www.flaticon.com/free-icon/file_13542066"><img src="readme_images/file (kerismaker).png" width="60px"></a> <span style="position:absolute; top:5px; font-size:18px;">*</span></div> |
| [Freepik](https://www.flaticon.com/authors/freepik) | <img src="images/add.png" width="50px"> <img src="images/exit.png" width="50px"> <img src="images/list.png" width="50px"> <img src="images/minus.png" width="50px"> <img src="images/settings.png" width="50px"> <img src="images/warning.png" width="50px"> <img src="readme_images/zip (Freepik).png" width="50px"> <span style="position:absolute; top:5px; right:0px; font-size:18px;">*</span></div>|
| [kmg design](https://www.flaticon.com/authors/kmg-design) | <img src="images/icono-carpeta.png" width="50px"> <img src="images/delete.png" width="50px"> |
| [Anggara](https://www.flaticon.com/authors/anggara) | <img src="images/information.png" width="50px"> <img src="images/question.png" width="50px"> |
| [iconixar](https://www.flaticon.com/authors/iconixar) | <img src="readme_images/move (iconixar).png" width="60px"> <span style="position:absolute; top:5px;  font-size:18px;">*</span></div> |
| [Laisa Islam Ani](https://www.flaticon.es/autores/laisa-islam-ani) | <img src="images/reset.png" width="50px"> |