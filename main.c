#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// COLORS
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

struct Process
{
    int pid, arrival, burst, priority;
    int completion, turnaround, waiting;
};

// Copy
void copyProcesses(struct Process src[], struct Process dest[], int n)
{
    for(int i=0;i<n;i++)
        dest[i] = src[i];
}

// 🎬 Animation
void animateExecution(struct Process p[], int n)
{
    int time = 0;

    printf("\nLoading Scheduler...\n");
    sleep(1);

    for(int i=0;i<n;i++)
    {
        for(int t=0; t<p[i].burst; t++)
        {
            system("clear");

            printf(BLUE "===== CPU SCHEDULER =====\n\n" RESET);
            printf("Time: " YELLOW "%d\n\n" RESET, time);
            printf("Running: " GREEN "P%d\n\n" RESET, p[i].pid);

            printf("Execution:\n|");

            for(int j=0;j<i;j++)
                printf(" " GREEN "P%d" RESET " |", p[j].pid);

            printf(" " RED ">P%d<" RESET " |", p[i].pid);

            printf("\n\n");

            fflush(stdout);
            usleep(400000);
            time++;
        }
    }

    printf(GREEN "\n✅ Execution Complete!\n" RESET);
    sleep(1);
}

// Results
void printResults(struct Process p[], int n, float *avgWT, float *avgTAT)
{
    *avgWT = 0;
    *avgTAT = 0;

    printf("\nProcess AT BT CT TAT WT\n");

    for(int i=0;i<n;i++)
    {
        printf("P%d     %d  %d  %d  %d  %d\n",
        p[i].pid, p[i].arrival, p[i].burst,
        p[i].completion, p[i].turnaround, p[i].waiting);

        *avgWT += p[i].waiting;
        *avgTAT += p[i].turnaround;
    }

    *avgWT /= n;
    *avgTAT /= n;

    printf("\nAverage Waiting Time = %.2f", *avgWT);
    printf("\nAverage Turnaround Time = %.2f\n", *avgTAT);
}

// 🌐 Export data
void exportData(struct Process p[], int n)
{
    FILE *fp = fopen("data.txt", "w");

    for(int i=0;i<n;i++)
    {
        fprintf(fp, "P%d %d %d %d\n",
            p[i].pid,
            p[i].arrival,
            p[i].burst,
            p[i].completion);
    }

    fclose(fp);
}

// Gantt
void ganttChart(struct Process p[], int n)
{
    printf("\nGantt Chart:\n|");

    for(int i=0;i<n;i++)
        printf(" P%d |",p[i].pid);

    printf("\n0");

    for(int i=0;i<n;i++)
        printf("   %d",p[i].completion);

    printf("\n");
}

// FCFS
void fcfs(struct Process p[], int n)
{
    int time = 0;

    for(int i=0;i<n;i++)
    {
        if(time < p[i].arrival)
            time = p[i].arrival;

        time += p[i].burst;

        p[i].completion = time;
        p[i].turnaround = time - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }

    printf("\n--- FCFS ---\n");
    ganttChart(p,n);
}

// SJF
void sjf(struct Process p[], int n)
{
    struct Process temp;

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].burst > p[j].burst)
            {
                temp=p[i]; p[i]=p[j]; p[j]=temp;
            }

    int time=0;

    for(int i=0;i<n;i++)
    {
        if(time < p[i].arrival)
            time = p[i].arrival;

        time += p[i].burst;

        p[i].completion=time;
        p[i].turnaround=time - p[i].arrival;
        p[i].waiting=p[i].turnaround - p[i].burst;
    }

    printf("\n--- SJF ---\n");
    ganttChart(p,n);
}

// Priority
void priorityScheduling(struct Process p[], int n)
{
    struct Process temp;

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].priority > p[j].priority)
            {
                temp=p[i]; p[i]=p[j]; p[j]=temp;
            }

    int time=0;

    for(int i=0;i<n;i++)
    {
        if(time < p[i].arrival)
            time = p[i].arrival;

        time += p[i].burst;

        p[i].completion=time;
        p[i].turnaround=time - p[i].arrival;
        p[i].waiting=p[i].turnaround - p[i].burst;
    }

    printf("\n--- Priority ---\n");
    ganttChart(p,n);
}

// Round Robin
void roundRobin(struct Process p[], int n, int quantum)
{
    int remaining[n], time=0, done;

    for(int i=0;i<n;i++)
        remaining[i]=p[i].burst;

    printf("\n--- Round Robin ---\n");
    printf("\nGantt Chart:\n|");

    do{
        done=1;

        for(int i=0;i<n;i++)
        {
            if(remaining[i]>0)
            {
                done=0;
                printf(" P%d |",p[i].pid);

                if(remaining[i]>quantum)
                {
                    time+=quantum;
                    remaining[i]-=quantum;
                }
                else
                {
                    time+=remaining[i];
                    p[i].completion=time;

                    p[i].turnaround=time - p[i].arrival;
                    p[i].waiting=p[i].turnaround - p[i].burst;

                    remaining[i]=0;
                }
            }
        }
    }while(!done);

    printf("\n0   ");
    for(int i=0;i<n;i++)
        printf("%d   ",p[i].completion);
    printf("\n");
}

// Comparison
void compareAlgorithms(struct Process original[], int n)
{
    struct Process p1[n], p2[n], p3[n], p4[n];
    float wt1,tat1, wt2,tat2, wt3,tat3, wt4,tat4;

    copyProcesses(original,p1,n);
    copyProcesses(original,p2,n);
    copyProcesses(original,p3,n);
    copyProcesses(original,p4,n);

    fcfs(p1,n); animateExecution(p1,n); printResults(p1,n,&wt1,&tat1);
    sjf(p2,n); animateExecution(p2,n); printResults(p2,n,&wt2,&tat2);
    priorityScheduling(p3,n); animateExecution(p3,n); printResults(p3,n,&wt3,&tat3);

    int q;
    printf("\nEnter Time Quantum for RR: ");
    scanf("%d",&q);

    roundRobin(p4,n,q); animateExecution(p4,n); printResults(p4,n,&wt4,&tat4);

    printf("\n===== FINAL COMPARISON =====\n");
    printf("Algorithm     Avg WT     Avg TAT\n");

    printf("FCFS          %.2f       %.2f\n",wt1,tat1);
    printf("SJF           %.2f       %.2f\n",wt2,tat2);
    printf("Priority      %.2f       %.2f\n",wt3,tat3);
    printf("Round Robin   %.2f       %.2f\n",wt4,tat4);

    float min=wt1;
    char best[20]="FCFS";

    if(wt2<min){min=wt2; strcpy(best,"SJF");}
    if(wt3<min){min=wt3; strcpy(best,"Priority");}
    if(wt4<min){min=wt4; strcpy(best,"Round Robin");}

    printf(GREEN "\n🔥 Best Algorithm: %s\n" RESET,best);
}

// MAIN
int main()
{
    int n,choice;

    printf(BLUE "CPU Scheduling Tool\n" RESET);

    printf("\nEnter number of processes: ");
    scanf("%d",&n);

    struct Process p[n];

    for(int i=0;i<n;i++)
    {
        p[i].pid=i+1;

        printf("\nProcess P%d\n",i+1);
        printf("Arrival Time: "); scanf("%d",&p[i].arrival);
        printf("Burst Time: "); scanf("%d",&p[i].burst);
        printf("Priority: "); scanf("%d",&p[i].priority);
    }

    printf("\n==== MENU ====\n");
    printf("1. Run Single Algorithm\n");
    printf("2. Compare All Algorithms 🔥\n");
    printf("Enter choice: ");
    scanf("%d",&choice);

    float wt,tat;

    if(choice==1)
    {
        int ch,q;

        printf("\n1.FCFS  2.SJF  3.Priority  4.Round Robin\n");
        printf("Choose: ");
        scanf("%d",&ch);

        switch(ch)
        {
            case 1: fcfs(p,n); break;
            case 2: sjf(p,n); break;
            case 3: priorityScheduling(p,n); break;
            case 4:
                printf("Enter Quantum: ");
                scanf("%d",&q);
                roundRobin(p,n,q);
                break;
        }

        animateExecution(p,n);
        printResults(p,n,&wt,&tat);
        exportData(p,n);
    }
    else if(choice==2)
    {
        compareAlgorithms(p,n);
        exportData(p,n);
    }

    return 0;
}
