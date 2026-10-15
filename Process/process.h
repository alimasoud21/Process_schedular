enums class ProcessStates{NEW,READY,RUNNING,BLOCKED,TERMINATEd}

class Process{
    private:
        int arrival_time;
        int CPU_time;
        int remaining_time;
        //int deleline;

        // Metric attributes
        int FirstExecutionTime;
        int CompletionTime;
        int ResponceTime;
        int TurnaroundTime;
        int Waiting time;

        ProcessStates states = NEW;

    public:
        


};