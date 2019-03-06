#!/usr/bin/python

from matplotlib.pyplot import *
import numpy as np

x = np.linspace(-np.pi, np.pi, 100, endpoint=True)
y_sin = np.sin(x)
y_cos = np.cos(x)
plot(x, y_sin)
plot(x, y_cos)

title("Function $\sin$ and $\cos$")

xlim = (-3, 3)
ylim = (-2, 2)

xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi], [r'$-\pi$', r'$-\pi/2$', r'$0$', r'$+\pi/2$', r'$+\pi$'])
yticks([-2, -1, 0, 1, 2], [r'$-2$', r'$-1$', r'$0$', r'$1$', r'$2$'])

grid()
show()

