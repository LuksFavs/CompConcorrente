#%%
import random

saida = open("entrada.txt","w")

a=int(input("tamanho do bloco"))
b=int(input("numero de entradas"))
saida.write(f"{str(b)}\n")

for i in range(int(b/a)):
    for i in range(a):
        x=random.randrange(-50,50)
        saida.write(f"{str(x)}")
        if i!=a-1:
            saida.write(" ")
    saida.write("\n")
for i in range((b%a)):
    x=random.randrange(-50,50)
    saida.write(f"{str(x)}")
    if i!=b%a-1:
        saida.write(" ")
saida.write("\n")
saida.close()
# %%
