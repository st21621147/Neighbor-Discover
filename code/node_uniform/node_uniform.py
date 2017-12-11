# -*- coding: utf-8 -*-
from __future__ import  division
import numpy as np
import matplotlib.pyplot as plt


def loadfile(fileName):
    file=open(fileName)
    node = []
    for line in file:
        a=line.strip().split(",")
        node.append(float(a[1]))
    return node


def paintNormalybar():
    label = ("1000", "2000",  "3000", "4000", "5000", "6000", "7000", "8000", "9000")
    labelCall = np.arange(len(label))

    fig = plt.figure(figsize=(7, 4))
    ax = fig.add_subplot(111)

    y1 = loadfile("RDS_latency.txt")
    y2 = loadfile("Aloha_latency.txt")

    a = list(map(lambda x: x[0] - x[1], zip(y2, y1)))
    b = list(map(lambda x: x[0]/x[1], zip(a, y1)))
    print y1
    print y2
    print a
    print b
    # print y2

    plt.bar(labelCall - 0.15, y1, label="Alano", color="r", hatch="X", align="center", width=0.3)
    plt.bar(labelCall + 0.15, y2, label="Aloha-like", color="c", hatch="o", align="center", width=0.3)
    plt.xticks(labelCall, label, fontsize=12)
    plt.ylabel("Latency", fontsize=16)
    plt.xlabel("The Number of Nodes", fontsize=16)
    plt.ylim(0, 150000)
    #
    ax.set_yticks([ 20000, 50000, 100000, 200000, 300000, 400000])
    ax.set_yticklabels(
        [ '2$\\times10^4$', '5$\\times10^4$', '1$\\times10^5$', '2$\\times10^5$', '3$\\times10^5$', '4$\\times10^5$'])
    # plt.yscale('log')
    # ax.set_yticks([25000, 50000, 100000, 500000, 800000])
    # ax.set_yticklabels(
    #     ['2.5$\\times10^4$', '5$\\times10^4$', '1$\\times10^5$', '5$\\times10^5$', '8$\\times10^5$'])
    plt.legend(loc=2, prop={'size': 12})
    # plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=3, mode="expand", borderaxespad=0.)
    plt.savefig('node_uniform.eps', format='eps', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    # paintLatencybar()
    paintNormalybar()