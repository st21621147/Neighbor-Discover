# -*- coding: utf-8 -*-
from __future__ import  division
import numpy as np
import matplotlib.pyplot as plt

def paintLatencybar():
    label = ("Alano", "Aloha-like",  "Hello", "Hedis", "Searchlight")
    labelCall = np.arange(len(label))

    fig = plt.figure(figsize=(7, 4))
    ax = fig.add_subplot(111)

    plt.bar(labelCall - 0.15, [10683, 16416.6, 37987.1, 67640.2, 67607.2], label="Global", color="r", hatch="X", align="center", width=0.3)
    plt.bar(labelCall + 0.15, [8126.96, 15155.9, 18064.5, 68509.7, 67963.4], label="Local", color="c", hatch="o", align="center", width=0.3)
    plt.xticks(labelCall, label, fontsize=12)
    plt.ylabel("Latency", fontsize=16)
    # plt.ylim(0, 1.03)
    # ax.set_yticks([10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000])
    # ax.set_yticklabels(['1$\\times10^4$', '2$\\times10^4$', '3$\\times10^4$', '4$\\times10^4$', '5$\\times10^4$', '6$\\times10^4$', '7$\\times10^4$', '8$\\times10^4$'])
    plt.ylim(0, 40000)
    ax.set_yticks([10000, 20000, 30000, 40000])
    ax.set_yticklabels(['1$\\times10^4$', '2$\\times10^4$', '3$\\times10^4$', '4$\\times10^4$'])
    plt.legend(loc=2, prop={'size': 12})
    # plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=3, mode="expand", borderaxespad=0.)
    plt.savefig('latency_uniform.eps', format='eps', bbox_inches='tight')
    plt.show()

def paintNormalybar():
    label = ("Alano", "Aloha-like",  "Hello", "Hedis", "Searchlight")
    labelCall = np.arange(len(label))

    fig = plt.figure(figsize=(7, 4))
    ax = fig.add_subplot(111)

    plt.bar(labelCall - 0.15, [28714.1, 37715.2, 484167, 734180, 733401], label="Global", color="r", hatch="X", align="center", width=0.3)
    plt.bar(labelCall + 0.15, [22046.8, 32175.8, 171188, 734293, 734614], label="Local", color="c", hatch="o", align="center", width=0.3)
    plt.xticks(labelCall, label, fontsize=12)
    plt.ylabel("Latency", fontsize=16)
    plt.ylim(0, 80000)
    #
    ax.set_yticks([ 20000, 40000, 60000, 80000])
    ax.set_yticklabels(
        [ '2$\\times10^4$',  '4$\\times10^4$', '6$\\times10^4$', '8$\\times10^4$'])
    # plt.yscale('log')
    # ax.set_yticks([25000, 50000, 100000, 500000, 800000])
    # ax.set_yticklabels(
    #     ['2.5$\\times10^4$', '5$\\times10^4$', '1$\\times10^5$', '5$\\times10^5$', '8$\\times10^5$'])
    plt.legend(loc=2, prop={'size': 12})
    # plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=3, mode="expand", borderaxespad=0.)
    plt.savefig('latency_normal.eps', format='eps', bbox_inches='tight')
    plt.show()


def loadfile(fileName):
    file=open(fileName)
    node = []
    for line in file:
        node.append(line[:-1])
    return node



def paintRate():
    x0 = range(1, 40001)
    y1 = loadfile("RDS_rate.txt")
    y2 = loadfile("Aloha_rate.txt")
    y3 = loadfile("Hedis_rate.txt")
    y4 = loadfile("Hello_rate.txt")
    y5 = loadfile("Searchlight_rate.txt")
    fig = plt.figure(figsize=(6, 4))
    ax = fig.add_subplot(111)
    plt.plot(x0, y1[:40000], 'r-', lw=3, label="RDS-Alano")
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

    plt.xlim(0, 40000)
    plt.ylim(0, 1)
    plt.savefig('rate_uniform.eps', format='eps', bbox_inches='tight')
    plt.show()


if __name__ == '__main__':
    paintLatencybar()
    # paintNormalybar()
    # paintRate()