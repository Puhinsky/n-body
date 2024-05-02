# n-body

## Theory

$$
f_1=f(x_n,y_n),
$$

$$
f_2=f(x_n + \frac{2}{3} h, y_n+\frac{2}{3} h\cdot f_1)
$$

$$
f_3=f(x_n + \frac{2}{3} h, y_n-\frac{h}{3} \cdot f_1 + h\cdot f_2)
$$

$$
y_{n+1}=y_n+\frac{h}{4}(f_1+2f_2+f_3)
$$

[Метод Хойна/Рунге-Кутта, 3 порядок и не только](https://old.mipt.ru/upload/medialibrary/87d/rk.pdf)

[N-Body на пайтоне](https://prappleizer.github.io/Tutorials/RK4/RK4_Tutorial.html)

[Тут про общую энергию](https://www.math.arizona.edu/~dwang/485home-10/N-body.pdf)

## Report

### Шаг 0: Анализ

Изначально компилятор векторизовал цикл с расчетом потенциальной энергии и раскрутил цикл с расчетом кинетической. Также цикл для расчета усорений был предикатно оптимизирован. Автоматическия оптимизация остальных циклов не удалась.

![](img/default_opt_report.png)

Summary:

![](img/default_summary.png)

Roofline:

![](img/default_roofline.png)

Hotspots:

![](img/default_hotspots.png)

Memory Access Pattern Анализ выявил неэффективный доступ к памяти:

![](img/default_map.png)

### Шаг 1: Векторизация

Средние значения на 10 запусков:

|SSE (default)|AVX|AVX2|
|---|---|---|
|13508.2 мс|13240.0 мс|13585.2 мс|

|No tune|Tune for Haswell|
|---|---|
|13240.0 мс|13039.9 мс|

Цикл для расчета кинетической энергии компилятор посчитал невыгодным для simd-фикации

|SIMD|Unroll|
|---|---|
|13046.6 ms|13033.2 ms|

 Были добавлены выравнивание ```alignas(64)``` и оптимизация ```__restrict```

![](img/vectorization_roofline.png)

 ### Шаг 2: Оптимизация доступа к памяти

 Было добавлен массив для временного храниния ускорений.

 |Forward write|Temp write|
 |---|---|
 |13033.2 ms|6733.5 ms|

 ### Шаг 3: Добавление многопоточности

![](img/parallel_suitability.png)

|non mp|generation mp|pos and vel mp|accel mp|energy mp|
|---|---|---|---|---|
|6450.4 ms|6521.9 ms|7211.2 ms|4160.9 ms|4970.5 ms|

generation mp – unroll

pos and vel mp – simd

accel mp – parallel

energy mp – parallel

|atomic|reduction|
|---|---|
|1717.5 ms|948.6 ms|

Total time: 948.6 ms

Utilization report:

![](img/parallel_utilization.png)

Bottom-up report:

![](img/parallel_bottom-up.png)

Data race check report:

![](img/parallel_data_race.png)

Memory leaks check report:

![](img/parallel_memory_leaks.png)

Final roofline:
![](img/parallel_roofline.png)


Timing (10 000 объектов):

* default:  13508.2 мс

* vectorization: 13046.6 ms

* opt memory access pattern: 6733.5 ms

* parallel: 948.6 ms