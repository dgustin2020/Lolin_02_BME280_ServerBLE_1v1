/*
my_task_gest.h
*/
#ifndef MY_TASK_GEST_H
#define MY_TASK_GEST_H

extern "C" {

class My_Task_Gest{
  public:
    /* crea un elemeto tpo my_task_gest*/
    My_Task_Gest();
    /* inizializza i vari puntatori del task
      idx = indice che punta al primo task della tabella da eseguire, di solito il primo quindi 0
      mask = la maschera da usare per limitare l'indice che punta alla tabella dei task
      tbl_ptr = inidirizzo della tabella degli indirizzi dei vari task
      p_time = pointer ad un orologio globale per eventuali gestioni a tempo
    */
    void init(uint8_t idx, uint8_t mask, FnPtr* tbl_ptr, T_Lclk* p_time);
    
    /* inizializza la gestione e lancia il task passato in idx*/
    bool init_and_run(uint8_t idx, uint8_t mask, FnPtr* tbl_ptr, T_Lclk* p_time);//true: trovata procedura ; false: non trovata procedura
    
    /* passa alla prossima procedura della tabella di funzioni e la chiama*/
    bool next_and_run(void);//true: trovata procedura ; false: non trovata procedura
    
    /* chiama la procedura attulamente indicizzata da _idx*/
    bool run(void);//true: trovata procedura ; false: non trovata procedura
    
    /* incre idx tabella indirizzi procedure*/
    bool next(void);//true se esite procedura, false se non esiste
    
    /* esce con il valore dell'indice tabella attuale*/
    uint8_t get_idx(void);
    
    /* imposta l'indice al vlore passato */
    bool set_idx(uint8_t);

    /* abilita task*/
    void task_enable(void);

    /* disabilita task*/
    void task_disable(void);

    /* recupera lo stato del task */
    bool task_get_state(void);

   /* timer per uso generico */
    T_Lclk  loc_time;// se -1, non impostato

  private:
    uint8_t _idx;
    uint8_t _mask;
    FnPtr*  _tbl_ptr;
    FnPtr   _Pfn;           // dich. puntatore al programma principale
    T_Lclk* _p_time;        // ptr clock per confronto con loc_time (di solito Base_Lclk)
    bool    task_active;    /* abilita/disabilta gestione del task - true = abilitato / false = disabilitato*/


};

}// end C
#endif