# -*- coding: utf-8 -*-
from __future__ import  division
import numpy as np
import matplotlib.pyplot as plt


def loadfile(fileName):
    file=open(fileName)
    node = []
    for line in file:
        node.append(line[:-1])
    return node


def paintRate():
    x0 = range(1, 40001)
    y1 = loadfile("TP_rate.txt")
    y2 = loadfile("Aloha_rate.txt")
    y3 = loadfile("Hedis_rate.txt")
    y4 = loadfile("Hello_rate.txt")
    y5 = loadfile("Searchlight_rate.txt")
    fig = plt.figure(figsize=(6, 4))
    ax = fig.add_subplot(111)
    plt.plot(x0, y1[:40000], 'r-', lw=3, label="TP-Alano")
    plt.plot(x0, y2[:40000], 'c--', lw=3, label="Aloha-like")
    plt.plot(x0, y3[:40000], 'b--', lw=3, label="Hedis")
    plt.plot(x0, y4[:40000], 'y:', lw=3, label="Hello")
    plt.plot(x0, y5[:40000], 'g-.', lw=3, label="Searchlight")

    plt.xlabel("Time ($t_0$)", fontsize='16')
    plt.ylabel("Discovery Rate", fontsize='16')
    plt.legend(loc=4, prop={'size': 10})

    ax.set_xticks([10000, 20000, 30000, 40000])
    # ax.set_xticklabels([1, 2, 3, 4])
    ax.set_xticklabels(['1$\\times 10^4$', '2$\\times 10^4$', '3$\\times 10^4$', '4$\\times 10^4$'])
    # plt.yscale('logit')
    plt.xlim(0, 40000)
    plt.ylim(0, 1)
    plt.savefig('rate_local_uniform.eps', format='eps', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    # paintLatencybar()
    paintRate()