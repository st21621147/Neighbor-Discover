# -*- coding: utf-8 -*-
from __future__ import  division
import numpy as np
import matplotlib.pyplot as plt


def loadfile(fileName):
    file=open(fileName)
    node = []
    for line in file:
        a=line.split(",")
        node.append(a[1])
    return node


def paintRate():
    x0 = np.arange(0.1,0.51,0.05)
    y1 = loadfile("RDS_latency.txt")
    y2 = loadfile("Aloha_latency.txt")
    y3 = loadfile("hedis_latency.txt")
    y4 = loadfile("hello_latency.txt")
    y5 = loadfile("Searchlight_latency.txt")
    fig = plt.figure(figsize=(6, 4))
    ax = fig.add_subplot(111)
    print x0
    print y1
    plt.plot(x0, y1, 'ro-', lw=3, label="RDS-Alano")
    plt.plot(x0, y2, 'cv--', lw=3, label="Aloha-like")
    plt.plot(x0, y3, 'bx--', lw=3, label="Hedis")
    plt.plot(x0, y4, 'y+:', lw=3, label="Hello")
    plt.plot(x0, y5, 'g*-.', lw=3, label="Searchlight")
    plt.xlabel("Duty Cycle ($t_0$)", fontsize='16')
    plt.ylabel("Latency", fontsize='16')
    plt.legend(loc=1, prop={'size': 12})

    # ax.set_xticks([10000, 20000, 30000, 40000, 50000])
    # ax.set_xticklabels([1, 2, 3, 4])
    # ax.set_xticklabels(['1$\\times 10^4$', '2$\\times 10^4$', '3$\\times 10^4$', '4$\\times 10^4$', '5$\\times 10^4$'])
    # plt.yscale('logit')
    plt.xlim(0.1, 0.5)
    plt.yscale('log')
    # plt.ylim(0, 1)
    plt.savefig('dutycycle_normal.eps', format='eps', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    # paintLatencybar()
    paintRate()