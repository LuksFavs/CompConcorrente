tempo médio para 10⁵
sequencial = 0.0006764     base
com 2 threads = 0.0008054  aceleração de 0,83983114, indicando desaceleração
com 4 threads = 0.0011466  aceleração de 0,589918018, indicando desaceleração

tempo médio para 10⁷
sequencial = 0,0212752    base
com 2 threads = 0,0125484 aceleração de 1.695451213
com 4 threads = 0.0090614 aceleração de 2,347893262

tempo médio para 10⁹
sequencial = 2.1287782    base
com 2 threads = 1.2515656 aceleração de 1,700892227
com 4 threads = 0.8652672 aceleração de 2,460255283

Esses dados nos indicam que, para casos pequenos a concorrencia pode nao ser indicada,
ou até mesmo ser contraindicada ja que ela pode acarretar com o efeito contrario ao esperado.
mas vemos que a concorrencia demostra seu uma aceleração significativa para execuções cada vez
maiores, chegando a deixar o código 2,5 vezes mais rapido do que o caso sequencial.
