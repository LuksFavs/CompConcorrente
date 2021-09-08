/*
 * Laboratório 6 - Contador de Números pares
 * Aluno: Lucas Favilla Ferreira Alves da Silva
 * DRE: 119156518
 *
 * Programa: Conta a quantidade de números pares de um vetor de tamanho 'TAMANHO' com 'TAM' threads
 * Acabei por tendo que inicializar a classe T antes das Threads porque caso contrario não teria como retornar o valor
 * de aux2 para a main.(ficava dando erro durante a compilação do programa)
 * */

import java.util.Random;

class V{
    Random r = new Random();
    //mude esta variavel para mudar o tamanho do vetor
    static final int TAMANHO = 10000;
    //para fazer o check.
    private int pares = 0;
    private int[] v = new int[TAMANHO];

    //construtor
    public V(){
        for(int i = 0; i<TAMANHO; i++){
            //Tenta não haver números repetidos
            int aux = r.nextInt(TAMANHO*2);
            //para dar o check no final
            if(aux%2==0) this.incr();
            this.v[i] = aux;
        }
    }

    public void incr(){
        this.pares++;
    }
    public int pares(){
        return this.pares;
    }

    public int size(){
        return this.v.length;
    }

    public synchronized int get(int i){
        return this.v[i];
    }
}

class T implements Runnable{
    int id;
    V vec;
    int N;
    private volatile int aux2=0;

    public T(int i,int j, V vec){
        this.id = i;
        this.N = j;
        this.vec = vec;
    }

    public int resultado(){
        return this.aux2;
    }

    public void run(){
        //System.out.println("entrei " + this.id + "\n");
        //check para nao acontecer erro de truncamento em contas nao exatas.
        if((N-1) != this.id){
            for(int i= this.id*(this.vec.size()/N);i<(this.id + 1) * (this.vec.size()/N);i++){
                if(this.vec.get(i)%2 == 0) aux2++;
            }
        }else{
            for(int i= this.id*(this.vec.size()/N);i<this.vec.size();i++){
                if(this.vec.get(i)%2 == 0) aux2++;
            }
        }
        System.out.println("acabei " + this.id + " pares = " + this.resultado() + "\n");
    }

    
}

class Vetor {
    //mude esta variavel para mudar a quantidade de threads
    static final int TAM = 12;
    private static int fin = 0;
    public static void main(String[] args){
        Thread[] threads = new Thread[TAM];
        T[] a = new T[TAM];
        V ve = new V();
        for(int h = 0; h<threads.length; h++){
            a[h] = new T(h, TAM, ve);
        }
        for(int i = 0; i<threads.length; i++){
            threads[i]= new Thread(a[i]);
        }
        for(int j = 0; j<threads.length;j++){
            threads[j].start();
        }
        for(int k = 0; k<threads.length;k++){
            try{
                threads[k].join();
            }catch(InterruptedException e){
                return;
            }
        }
        for(int m=0;m<threads.length;m++){
            fin+=a[m].resultado();
        }
        if(fin == ve.pares()) System.out.println("valor de pares: " + ve.pares() + "\n");
        else System.out.println("ERROR " + fin + " != " + ve.pares() + "\n");
    }

}
