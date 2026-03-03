#include<iostream>
#include<fstream>
#include<string>
#include<forward_list>
#include<stdio.h>
#include<stdlib.h>
using namespace std;


class TaskNode {
private:
    string title;
    string description;
    string dueDate;

public:
    TaskNode() {}
    TaskNode(string &title, string &description, string &dueDate) {
        this -> title = title;
        this -> description = description;
        this -> dueDate = dueDate;
    }

    string getTitle() { return title; }
    string getDescription() { return description; }
    string getDueDate() { return dueDate; }

    void printTask() {
        cout << endl;
        cout << "Task title: " << title << endl;
        cout << "Task Description: " << description << endl;
        cout << "Task Due date: " << dueDate << endl;
    }
};

class TaskManager {
private:
    forward_list<TaskNode> tasks;
    static bool isObjectCreated;
    TaskManager() { 
        loadTasks(); 
    }

protected:
    void loadTasks();
    void unloadTasks();

public:
    static TaskManager* getTaskManager() {
        if(isObjectCreated == false) {
            isObjectCreated == true;
               return new TaskManager();
        } else {
            cout << "\nYou can get only one task manager" << endl;
          return nullptr;
        }
    }

    int menu();
    void addTask();
    void deleteTask(string &t);
    void viewAllTasks();
    ~TaskManager();
};

bool TaskManager::isObjectCreated = false;

void TaskManager::loadTasks() {

    tasks.clear();

    ifstream fin("Mytask.txt");

    if(!fin) {   // File does not exist (first run)
        return;
    }

    if(fin.peek() == ifstream::traits_type::eof()) {
        cout << "Task Manager is empty\n";  // print only if file exists but empty
        fin.close();
        return;
    }

    //  File exists and has data
    string t, d, dd;  

    while(getline(fin, t) && getline(fin, d) && getline(fin, dd)) {
        TaskNode temp(t, d, dd);
        tasks.push_front(temp);
    }

    fin.close();
}

void TaskManager::unloadTasks() {
    if(tasks.empty()) {
        remove("Mytask.txt");  // remove file if no task
        return;
    }

    ofstream fout("Mytask.txt");
    for(auto &temp : tasks) {
        fout << temp.getTitle() << endl;
        fout << temp.getDescription() << endl;
        fout << temp.getDueDate() << endl;
    }
     fout.close();
}

int TaskManager::menu() {
    int choice;
    cout << endl;
    cout << "\n1. Add New task: ";
    cout << "\n2. Delete Task: ";
    cout << "\n3. View All Tasks: ";
    cout << "\n4. Exit...";
    cout << "\nEnter your choice: ";
    cin >> choice;

    if(cin.fail()) {
        cin.clear();  
        cin.ignore(1000, '\n'); // clear buffer
        return -1;
    }

    cin.ignore(1000, '\n');  
    return choice;
}

void TaskManager::addTask() {

    while(true) {   

        string t, d, dd;

        cout << "\nEnter task title: "; 
        getline(cin, t);

        cout << "Enter task description: ";
        getline(cin, d);

        cout << "Enter due date: ";
        getline(cin, dd);

        TaskNode newTask(t, d, dd);
        tasks.push_front(newTask);

        cout << "\nTask Added Successfully!\n";

        int wrongCount = 0;

        while(true) {

            string choice;

            cout << "\nWhat would you like to do?\n";
            cout << "1. View All Tasks\n";
            cout << "2. Add Another Task\n";
            cout << "3. Delete Task\n";
            cout << "4. Back to Main Menu\n";
            cout << "Enter your choice: ";

            getline(cin, choice);

            if(choice == "1") {
                viewAllTasks();
                wrongCount = 0;
            }
            else if(choice == "2") {
                break;   // add again
            }
            else if(choice == "3") {
                string temp;
                cout << "\nEnter title to delete: ";
                getline(cin, temp);
                deleteTask(temp);
                wrongCount = 0;
            }
            else if(choice == "4") {
                return;   // back to main menu
            }
            else {
                wrongCount++;
                cout << "\nInvalid Choice! Attempt "
                     << wrongCount << " of 2.\n";

                if(wrongCount >= 2) {
                    cout << "\nReturning to Main Menu...\n";
                    return;
                }
            }
        }
    }
}


void TaskManager::deleteTask(string &t) {
    forward_list <TaskNode>::iterator it, it1;
    bool flag = true;
    it = tasks.begin();
    it1 = tasks.before_begin();

    while(it != tasks.end()) {
        if(it->getTitle() == t) {
            tasks.erase_after(it1);
            cout << "\nTask deleted Succefully" << endl;
            flag = false;
            break;
        }
          it1 = it++;
    }
     if(flag)
       cout <<"\nTask not found" << endl;
}

void TaskManager::viewAllTasks() {
    if(tasks.empty())
       cout << "\nNo Task in the manager";
    else
       for(auto task:tasks)
          task.printTask();
    char x;
    cout << "\nEnter any key to processed";
    cin >> x;
}

TaskManager::~TaskManager() {
    unloadTasks();  // Save task before exit
    isObjectCreated = false;
    cout << "\nTask Manager closed succefuly";
}


int main() {
    TaskManager *tm;
    tm = TaskManager::getTaskManager();

    if(tm == nullptr)
        return 0;

    string temp;

    while(true) {
        
        switch(tm-> menu()) {

            case 1:
                tm-> addTask();
                break;

            case 2:
                cout <<"\nEnter title of the task to delete: ";
                getline(cin, temp);
                tm-> deleteTask(temp);
                break;

            case 3:
                tm->viewAllTasks();
                break;

            case 4:
                delete tm;
                exit(0);

            default:
                cout << "\nInvalid Choice, retry";
        }
         getchar();
    }
}