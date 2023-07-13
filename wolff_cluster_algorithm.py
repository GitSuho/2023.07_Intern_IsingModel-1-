import random, math
import numpy as np

L = 100
N = L* L
nbr = {i : ((i // L) * L + (i + 1) % L, (i + L) % N, (i // L) * L + (i - 1) % L, (i - L) % N) for i in range(N)}


T_steps = np.linspace(2.5, 0.5, 20)
S = [random.choice([1, -1]) for k in range(N)]


for T in T_steps:
    S_mean = 0
    p = 1.0 - math.exp(-2.0 / T)
    nsteps = 10000
    for step in range(nsteps):
        k = random.randint(0, N - 1)
        Pocket, Cluster = [k], [k]
        while Pocket != []:
            j = random.choice(Pocket)
            for l in nbr[j]:
                if S[l] == S[j] and l not in Cluster and random.uniform(0.0, 1.0) < p:
                    Pocket.append(l)
                    Cluster.append(l)
            Pocket.remove(j)
        for j in Cluster:
            S[j] *= -1
        S_mean = sum(S) / N
    print(f'T : {T} , <m> = {S_mean/nsteps}')
