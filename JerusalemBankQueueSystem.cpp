#include <iostream>
#include <queue>
#include<time.h>

using namespace std;

class CustomerTask
{
public:
    int taskID;
    string taskName;
    int arrivalTime;
    int waitTime;
    int startTime;
    int periodTime;
    string priority;

    CustomerTask()
    {
        taskID = 0;
        arrivalTime = 0;
        waitTime = 0;
        startTime = 0;
        periodTime = 0;
        priority = "Normal";
    }
};

void enqueueTask(queue<CustomerTask> &BankQueue)
{
    CustomerTask task;
    cout << "Enter the Task ID: ";
    cin >> task.taskID;
    cout << "Enter the Task Name: ";
    cin >> task.taskName;
    cout << "Enter Arrival Time (Bank Opens from 8 AM till 2 PM): ";
    cin >> task.arrivalTime;

    if (!(task.arrivalTime >= 480 && task.arrivalTime <= 840))
    {
        cout << "Error: Arrival time must be between 8 AM and 2 PM. Task not added...!" << endl;
        return;
    }
    cout << "Enter Period Time (minutes needed to finish the task): ";
    cin >> task.periodTime;
    cout << "Enter Priority (Normal, Important, or VIP): ";
    cin >> task.priority;

    if (task.priority == "Important")
        task.arrivalTime = task.arrivalTime - 2;
    else if (task.priority == "VIP")
        task.arrivalTime = task.arrivalTime - 5;
    task.waitTime = 0;
    task.startTime = task.arrivalTime + task.waitTime;
    task.waitTime = task.startTime - task.arrivalTime;

    BankQueue.push(task);
}
void enqueueMultipleTasks(queue<CustomerTask> &BankQueue, int n)
{
    static const string taskOptions[] = {"Credit", "Debit", "Query", "Open an Account", "Close an Account", "Loan Application", "Foreign Currency Exchange", "ATM Issue Report", "Credit Card Application", "Safe Deposit Access"};
    static const int taskOptionsSize = sizeof(taskOptions) / sizeof(string);
    CustomerTask *tempTasks = new CustomerTask[n];
    for (int i = 0; i < n; i++)
    {
        CustomerTask task;
        task.taskID = i + 1;
        task.arrivalTime = rand() % 361 + 480;
        task.periodTime = rand() % 17 + 4;
        task.waitTime = 0;
        task.taskName = taskOptions[rand() % taskOptionsSize];

        int priorityNum = rand() % 3;
        if (priorityNum == 0)
            task.priority = "Normal";
        else if (priorityNum == 1)
            task.priority = "Important";
        else if (priorityNum == 2)
            task.priority = "VIP";

        if (task.priority == "Important")
            task.arrivalTime = task.arrivalTime - 2;
        else if (task.priority == "VIP")
            task.arrivalTime = task.arrivalTime - 5;

        tempTasks[i] = task;
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (tempTasks[j].arrivalTime > tempTasks[j + 1].arrivalTime)
            {
                CustomerTask temp = tempTasks[j];
                tempTasks[j] = tempTasks[j + 1];
                tempTasks[j + 1] = temp;
            }
        }
    }

    int lastTaskEndTime = 0;
    for (int i = 0; i < n; i++)
    {
        tempTasks[i].taskID = i + 1;
        if (tempTasks[i].arrivalTime < lastTaskEndTime)
            tempTasks[i].startTime = lastTaskEndTime;
        else
            tempTasks[i].startTime = tempTasks[i].arrivalTime;

        tempTasks[i].waitTime = tempTasks[i].startTime - tempTasks[i].arrivalTime;
        lastTaskEndTime = tempTasks[i].startTime + tempTasks[i].periodTime;

        BankQueue.push(tempTasks[i]);
    }

    delete[] tempTasks;
}

void reorderTasks(queue<CustomerTask> &BankQueue)
{
    for (int i = 0; i < BankQueue.size(); i++)
    {
        CustomerTask frontTask = BankQueue.front();
        BankQueue.pop();
        if (frontTask.waitTime >= 5)
            frontTask.waitTime -= 5;
        else
            frontTask.waitTime = 0;
        BankQueue.push(frontTask);
    }
}
string timeFormatter(int minutes)
{
    int h = minutes / 60;
    int min = minutes % 60;
    string period = h >= 12 ? "PM" : "AM";
    h = h % 12;
    if (h == 0)
        h = 12;
    string newTime;
    if (h < 10)
        newTime += "0";
    newTime += to_string(h) + ":";
    if (min < 10)
        newTime += "0";
    newTime += to_string(min) + " " + period;

    return newTime;
}

void displayTaskDetails(CustomerTask task)
{
    cout << "Task ID: " << task.taskID
         << ", Task Name: " << task.taskName
         << ", Arrival Time: " << timeFormatter(task.arrivalTime)
         << ", Wait Time: " << task.waitTime << " minutes"
         << ", Start Time: " << timeFormatter(task.startTime)
         << ", Period Time: " << task.periodTime << " minutes"
         << ", Priority: " << task.priority
         << endl;
}

int computeTotalWaitingTime(queue<CustomerTask> &BankQueue)
{
    int totalWait = 0;
    queue<CustomerTask> tempQueue = BankQueue;
    while (!tempQueue.empty())
    {
        totalWait += tempQueue.front().waitTime;
        tempQueue.pop();
    }

    return totalWait;
}

int computeTotalCompletionTime(queue<CustomerTask> BankQueue)
{
    int totalTime = 0;
    while (!BankQueue.empty())
    {
        CustomerTask task = BankQueue.front();
        totalTime += task.waitTime + task.periodTime;
        BankQueue.pop();
    }
    return totalTime;
}

int countIncompleteTasks(queue<CustomerTask> BankQueue)
{
    int notCompleted = 0;
    while (!BankQueue.empty())
    {
        CustomerTask task = BankQueue.front();
        if (task.startTime + task.periodTime > 840)
            notCompleted++;
        BankQueue.pop();
    }
    return notCompleted;
}

void showCompletedTaskDetails(queue<CustomerTask> &tasks)
{
    if (tasks.empty())
    {
        cout << "No completed tasks to show." << endl;
        return;
    }

    queue<CustomerTask> tempQueue;
    cout << "Completed Task Details:" << endl;
    while (!tasks.empty())
    {
        CustomerTask task = tasks.front();
        tasks.pop();
        if (task.startTime <= 840)
        {
            cout << "Task ID: " << task.taskID << ", ";
            cout << "Task Name: " << task.taskName << ", ";
            cout << "Task Arrival Time: " << timeFormatter(task.arrivalTime) << ", ";
            cout << "Task Start Time: " << timeFormatter(task.startTime) << ", ";
            cout << "Task Period Time: " << task.periodTime << ", ";
            cout << "Task Priority: " << task.priority << endl;
        }
        tempQueue.push(task);
    }

    while (!tempQueue.empty())
    {
        tasks.push(tempQueue.front());
        tempQueue.pop();
    }
}

void showPendingTaskDetails(queue<CustomerTask> &tasks)
{
    if (tasks.empty())
    {
        cout << "There are no tasks in the queue." << endl;
        return;
    }

    queue<CustomerTask> tempQueue = tasks;
    bool hasPendingTasks = false;

    cout << "Information of Tasks Arrived After 2 PM:" << endl;
    while (!tempQueue.empty())
    {
        CustomerTask currentTask = tempQueue.front();
        tempQueue.pop();
        if (currentTask.startTime >= 840)
        {
            hasPendingTasks = true;
            cout << "Task ID: " << currentTask.taskID << endl;
            cout << "Task Name: " << currentTask.taskName << endl;
            cout << "Task Arrival Time: " << timeFormatter(currentTask.arrivalTime) << endl;
            cout << "Task Period Time: " << currentTask.periodTime << " minutes" << endl;
            cout << "Task expected start time: " << timeFormatter(currentTask.startTime) << endl;
            cout << "Task Priority: " << currentTask.priority << endl;
            cout << "------------------------------------------" << endl;
        }
    }

    if (!hasPendingTasks)
        cout << "No tasks arrived after 2 PM." << endl;
}

int main()
{
    srand(time(NULL));
    queue<CustomerTask> bankQueue;
    int option;
    do
    {
        cout << "Jerusalem Bank System" << endl;
        cout << "1. Enqueue Task\n";
        cout << "2. Enqueue Multiple Tasks\n";
        cout << "3. Reorder Tasks\n";
        cout << "4. Display Task Details\n";
        cout << "5. Compute Total Waiting Time\n";
        cout << "6. Compute Total Completion Time\n";
        cout << "7. Count Incomplete Tasks\n";
        cout << "8. Show Completed Task Details\n";
        cout << "9. Show Pending Task Details\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> option;

        switch (option)
        {
        case 1:
            enqueueTask(bankQueue);
            cout << "**********************\n";
            break;
        case 2:
            int count;
            cout << "Enter the number of tasks to enqueue: ";
            cin >> count;
            enqueueMultipleTasks(bankQueue, count);
            cout << "**********************\n";
            break;
        case 3:
            reorderTasks(bankQueue);
            cout << "**********************\n";
            break;
        case 4:
            if (bankQueue.empty())
                cout << "no task have been arrived yet." << endl;
            else
            {
                queue<CustomerTask> tempQueue = bankQueue;
                cout << "Task Details:" << endl;
                while (!tempQueue.empty())
                {
                    displayTaskDetails(tempQueue.front());
                    tempQueue.pop();
                }
            }
            cout << "**********************\n";
            break;
        case 5:
            cout << "Total Waiting Time: " << computeTotalWaitingTime(bankQueue) << " Minutes" << endl;
            cout << "**********************\n";
            break;
        case 6:
            cout << "Total Completion Time: " << computeTotalCompletionTime(bankQueue) << " Minutes" << endl;
            cout << "**********************\n";
            break;
        case 7:
            cout << "Incomplete Tasks: " << countIncompleteTasks(bankQueue) << endl;
            cout << "**********************\n";
            break;
        case 8:
            cout << "Printing the information of all served task:" << endl;
            showCompletedTaskDetails(bankQueue);
            cout << "**********************\n";
            break;

        case 9:
            cout << "Printing the information of all tasks not yet served" << endl;
            showPendingTaskDetails(bankQueue);
            cout << "**********************\n";
            break;
        }
    } while (option != 10);
    cout << "**********************\n";
    cout << "BANK SIMULATION END" << endl;
    return 0;
}