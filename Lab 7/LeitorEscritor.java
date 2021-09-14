/*
 * Laboratório 7 - Computação concorrente
 * Aluno: Lucas Favilla Ferreira Alves da Silva
 * DRE:119156518
 * Trabalho: usar o padrão leitor/escrito para:
 *           apenas escritores -> colocar o seu valor de ID como variavel.
 *           apenas leitores -> imprimir na tela se esse número é ou não primo.
 *           leitores/escritores -> imprime na tela se par ou impar e modifica a variavel pelo
 *           dobro do valor atual.
 * */
class LE {
  private int leit, escr, lescr;  
  int inteiro;
  
  // Construtor
  LE() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0;//escritor escrevendo (0 ou 1)
     this.lescr = 0;//escritor escrevendo (0 ou 1)
     this.inteiro = 0; //variavel a ser lida e mudada
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      while (this.escr > 0 || this.lescr > 0) {
      //if (this.escr > 0 || this.lescr > 0) {
         //System.out.println ("le.leitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      //System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) {return; }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notifyAll(); //libera escritor (caso exista escritor bloqueado)
     //System.out.println ("le.leitorSaindo("+id+") " + this.leit + " " + this.escr +" "+ this.lescr);
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
      //while((this.leit > 0) || (this.escr > 0) || (this.lescr > 0)) {
      if ((this.leit > 0) || (this.escr > 0) || (this.lescr > 0)) {
         //System.out.println ("le.escritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      //System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) {return; }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
     this.escr--; //registra que o escritor saiu
     this.notifyAll(); //libera leitores e escritores
     //System.out.println ("le.escritorSaindo("+id+")");
  }
  
  // Entrada para lescritores
  public synchronized void EntraLescritor (int id) {
    try { 
      if ((this.leit > 0) || (this.escr > 0) || (this.lescr > 0)) {
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.lescr++; //registra que ha um lescritor escrevendo
      //System.out.println ("le.lescritorEscrevendo("+id+")");
    } catch (InterruptedException e) {return; }
  }
  
  // Saida para lescritores
  public synchronized void SaiLescritor (int id) {
     this.lescr--; //registra que o lescritor saiu
     this.notifyAll(); //libera leitores e escritores
     //System.out.println ("le.lescritorSaindo("+id+")");
  }
}



//Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
  int id; //identificador da thread
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  boolean prim = true;

  // Construtor
  Leitor (int id, LE m) {
    this.id = id;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try {
        for(;;){
            this.monitor.EntraLeitor(this.id);
            for(int i = 2; i<this.monitor.inteiro;i++){
                if( this.monitor.inteiro % i == 0){
                    System.out.println("-Leitor: "+this.id+" | número " + this.monitor.inteiro + " não é primo, divisivel por "+ i);
                    prim = false;
                    break;
                }
            }
            if(prim) System.out.println("-Leitor: "+this.id+" | número " + this.monitor.inteiro + " é primo");
            this.monitor.SaiLeitor(this.id);
        }
      }catch (Exception e) { return; }
  }
}
//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
  int id; //identificador da thread
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritor (int id, LE m) {
    this.id = id;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try {
        for(;;){
            this.monitor.EntraEscritor(this.id);
            this.monitor.inteiro = this.id;
            System.out.println("Escrevi "+ this.id);
            this.monitor.SaiEscritor(this.id);
            for(int i = 0; i < 10000; i++){
            }
        }
    } catch (Exception e) { return; }
  }
}

class Lescritor extends Thread {
  int id; //identificador da thread
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  
  // Construtor
  Lescritor (int id, LE m) {
    this.id = id;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try { 
        for(;;){
            this.monitor.EntraLeitor(this.id);
            if(this.monitor.inteiro %2 == 0) System.out.println("//Lescritor: "+this.id+" | número " + this.monitor.inteiro + " é par");
            else System.out.println("//Lescritor: "+this.id+" | número " + this.monitor.inteiro + " é impar");
            this.monitor.SaiLeitor(this.id);
            this.monitor.EntraLescritor(this.id);
            this.monitor.inteiro = this.monitor.inteiro*2;
            this.monitor.SaiLescritor(this.id);
        }
    } catch (Exception e) { return; }
  }
}

class LeitorEscritor {
  static final int L = 7;
  static final int E = 7;
  static final int LEC = 8;

  public static void main (String[] args) {
    int i;
    LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
    Leitor[] l = new Leitor[L];       // Threads leitores
    Escritor[] e = new Escritor[E];   // Threads escritores
    Lescritor[] les = new Lescritor[LEC]; //thread de escritores/leitores;
    
    //inicia o log de saida
    //System.out.println ("import verificaLE");
    //System.out.println ("le = verificaLE.LE()");
    
    for (i=0; i<E; i++) {
       e[i] = new Escritor(i+1, monitor);
       e[i].start(); 
   
    }
     for (i=0; i<LEC; i++) {
       les[i] = new Lescritor(i+1, monitor);
       les[i].start(); 
    }
    for (i=0; i<L; i++) {
       l[i] = new Leitor(i+1, monitor);
       l[i].start(); 
    } 
    
  }
}
