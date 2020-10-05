# Аппроксимация функций двух переменных
### Краткое описание программы
Программа представляет собой реализацию аппроксимации функций двух переменных методом конечных элементов. Программа написана на языке C++, графический интерфейс реализован при помощи средств *Qt5* и *OpenGL*. 

Программа является многопоточной. Один из потоков всегда выделяется под отрисовку графики и работает независимо от других, в то время как количество потоков, задействованных под вычисление аппроксимации, может варьироваться и является параметром запуска программы. Многопоточность реализована на базе библиотеки *pthread*.
### Область, на которой строится аппроксимация
Аппроксимация строится на выпуклом четырехугольнике (выпуклость дополнительно проверяется в программе) с вырезом на стороне в виде подобного ему четырехугольника. Более подробная информация о задании области содержится в файле **[Setting the area.pdf](Setting%20the%20area.pdf)**, там же содержится информация о запуске программы. Пример задания области можно увидеть в файле **[area.txt](area.txt)**.
### Функционал программы

Программа строит 3 вида графиков (вид графика меняется по циклу нажатием кнопки *Change graph* на верхней панели управления):
1. График непосредственно функции

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/func.png" width="700" align="middle"> 

2. График аппроксимирующей функции

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/approx.png" width="700" allign="middle">

3. График невязки 

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/residual.png" width="700" allign="middle">

Аппроксимируемые функции меняются по циклу нажатием соответствующей кнопки *Change function* на верхней панели упраления. Всего в программе прописаны 8 функций, но этот список может дополняться (см. **[functions.cpp](functions.cpp)**). 

Цвет графика менятся пропорционально значению отображаемой функции: более темным цветам соответствуют меньшие значения, более светлым - большие значения.

Изображение графика может быть построено как с помощью перспективной, так и с помощью ортогональной проекций. Вид проекции меняется по циклу нажатием соответствующей кнопки *Change projection* на нижней панели управления:
* Перспективная проекция

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/perspective.png" width="700" allign="middle">

* Ортогональная проекция 

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/ortho.png" width="700" allign="middle">

Масштаб картинки может быть увеличен/уменьшен кнопками *Increase scale/Reduce scale* на верхней панели управления.

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/scale.png" width="700" allign="middle">


На область аппроксимации может быть нанесена сетка, по которой строится триангуляция. Эта возможность актвируется нажатием кнопки *Draw a split grid* на нижней панели управления.

<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/grid.png" width="700" allign="middle">

Помимо вышеперечисленных возможностей, можно:
* Вращать картинку по/против часовой стрелки нажатием кнопок *Rotate clockwise/Rotate counterclockwise*
* Менять точку, вокруг которой осуществляется вращение картинки кнопкой *Change point of view*
* Наносить координатную сетку кнопкой *Draw a coordinate grid*
* Имитировать погрешность вычислений кнопками *Add delta/Sustract delta*
* Увеличивать/уменьшать плотность триангуляции области, тем самым увеличивая/уменьшая точность аппроксимации, кнопками *Increase n/Reduce n*
* Завершить программу кнопкой *Exit*

К всем кнопкам на панелях управления привязаны шорткаты


