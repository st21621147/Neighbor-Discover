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
    x0 = range(1, 50001)
    y1 = loadfile("TP_rate.txt")
    y2 = loadfile("Aloha_rate.txt")
    y3 = loadfile("Hedis_rate.txt")
    y4 = loadfile("Hello_rate.txt")
    y5 = loadfile("searchlight_rate.txt")
    fig = plt.figure(figsize=(6, 4))
    ax = fig.add_subplot(111)
    plt.plot(x0, y1[:50000], 'r-', lw=3, label="TP-Alano")
    plt.plot(x0, y2[:50000], 'c--', lw=3, label="Aloha-like")
    plt.plot(x0, y3[:50000], 'b--', lw=3, label="Hedis")
    plt.plot(x0, y4[:50000], 'y:', lw=3, label="Hello")
    plt.plot(x0, y5[:50000], 'g-.', lw=3, label="Searchlight")

    plt.xlabel("Time ($t_0$)", fontsize='16')
    plt.ylabel("Discovery Rate", fontsize='16')
    plt.legend(loc=5, prop={'size': 10})

    ax.set_xticks([10000, 20000, 30000, 40000, 50000])
    # ax.set_xticklabels([1, 2, 3, 4])
    ax.set_xticklabels(['1$\\times 10^4$', '2$\\times 10^4$', '3$\\times 10^4$', '4$\\times 10^4$', '5$\\times 10^4$'])
    # plt.yscale('logit')
    plt.xlim(0, 50000)
    plt.ylim(0, 1)
    plt.savefig('rate_local_normal.eps', format='eps', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    # paintLatencybar()
    paintRate()