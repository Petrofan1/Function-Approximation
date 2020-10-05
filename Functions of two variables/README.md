# Аппроксимация функций двух переменных
### Краткое описание программы
Программа представляет собой реализацию аппроксимации функций двух переменных методом конечных элементов. Программа написана на языке C++, графический интерфейс реализован при помощи средств Qt5 и OpenGL.
### Функционал программы
Программа строит 3 вида графиков:
1. График непосредственно функции
<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/approx.png" width="700" align="middle">
2. График аппроксимирующей функции
<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/approx.png" width="700" allign="middle">
3. График невязки 
<img src="https://github.com/Petrofan1/Function-approximation/raw/master/Functions%20of%20two%20variables/images/residual.png" width="700" allign="middle">
Аппроксимируемые функции меняются по циклу нажатием соответствующей кнопки в панели упраления или с помощью шортката. Всего в программе прописаны 8 функций (см. **[functions.cpp](functions.cpp)**.
