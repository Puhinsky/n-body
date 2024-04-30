# n-body

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