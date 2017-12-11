# -*- coding: utf-8 -*-
from __future__ import  division
import numpy as np
import matplotlib.pyplot as plt

def uniform():
    fig = plt.figure(figsize=(4, 4))
    N = 100
    x = np.random.uniform(-10,10,size=N)
    y = np.random.uniform(-10,10,size=N)
    x1 = np.array([0])
    y1 = np.array([0])
    plt.xticks([])
    plt.yticks([])
    ax = plt.subplot()

    ax.scatter(x, y, s=5, alpha=0.6)
    ax.scatter(x1, y1, s=50, c='Red', marker='D', alpha=0.7)
    plt.savefig('uniform.eps', format='eps', bbox_inches='tight')
    plt.show()

def normal():
    fig = plt.figure(figsize=(4, 4))
    N=150
    mu, sigma = 0, 4  # 均值与标准差
    x = np.random.normal(mu, sigma, N)
    y = np.random.normal(mu, sigma, N)
    x1 = np.array([0])
    y1 = np.array([0])
    # plt.xlim(-10, 10)
    # plt.ylim(-10, 10)
    plt.xticks([])
    plt.yticks([])
    ax = plt.subplot()
    ax.scatter(x, y, s=5, alpha=0.6)
    ax.scatter(x1, y1, s=50, c='Red', marker='D',  alpha=0.7)
    plt.savefig('normal.eps', format='eps', bbox_inches='tight')
    plt.show()

if __name__ == '__main__':
    uniform()
    normal()
