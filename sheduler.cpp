#include<iostream>
#include<string>
using namespace std;

struct t
{
    string tittle;
    string description;
    int deadline_date;
    int deadline_month;
    int deadline_year;
    int deadline_hour;
    int deadline_minute;
};

class Task
{
public:
    static int n;
    int priority;
    t data;
    Task* next;

    Task(int p)
    {
        priority = p;
        next = NULL;
        cout << "Title: ";
        cin.ignore();
        getline(cin, data.tittle);
        cout << "Description: ";
        getline(cin, data.description);
        input_deadline();
    }

    void input_deadline()
    {
        cout << "Enter deadline date\n";
        cout << "Date (dd): ";
        cin >> data.deadline_date;
        cout << "Month (mm): ";
        cin >> data.deadline_month;
        cout << "Year (yyyy): ";
        cin >> data.deadline_year;
        cout << "Enter deadline time\n";
        cout << "Hours (hh): ";
        cin >> data.deadline_hour;
        cout << "Minutes (mm): ";
        cin >> data.deadline_minute;
    }
};

int Task::n = 0;

class PriorityQueue
{
public:
    Task* rear;
    Task* front;

    PriorityQueue()
    {
        rear = NULL;
        front = NULL;
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void addTask(int priority)
    {
        Task* temp = new Task(priority);

        if (isEmpty())
        {
            front = temp;
            rear = temp;
            return;
        }

        if (priority < front->priority)
        {
            temp->next = front;
            front = temp;
            heapifyDown();  // Reorganize the heap after adding a task
            return;
        }

        Task* prev = NULL;
        Task* current = front;

        while (current != NULL && priority >= current->priority)
        {
            prev = current;
            current = current->next;
        }

        temp->next = current;
        prev->next = temp;

        if (current == NULL)
        {
            rear = temp;
        }

        heapifyDown();  // Reorganize the heap after adding a task
    }



    void deleteTask(const string& titleToDelete)
    {
        if (isEmpty())
        {
            cout << "No task to delete!\n";
        }
        else
        {
            Task* temp = front;
            Task* prev = NULL;

            // Traverse the linked list to find the task with the specified title
            while (temp != NULL && temp->data.tittle != titleToDelete)
            {
                prev = temp;
                temp = temp->next;
            }

            if (temp == NULL)
            {
                cout << "Task with title '" << titleToDelete << "' not found.\n";
                return;
            }

            // Adjust pointers to remove the task
            if (prev == NULL)
            {
                // The task to delete is at the front
                front = temp->next;
                if (front == NULL)
                {
                    rear = NULL;
                }
            }
            else
            {
                // The task to delete is in the middle or end
                prev->next = temp->next;
                if (temp->next == NULL)
                {
                    rear = prev;
                }
            }

            // Delete the task
            temp->next = NULL;
            delete temp;

            heapifyDown(); // Reorganize the heap after deleting a task
        }
    }

    void heapifyDown()
    {
        Task* current = front;

        while (current != NULL)
        {
            Task* minChild = current;
            Task* leftChild = current->next;
            Task* rightChild;

		if (leftChild != NULL) {
    		rightChild = leftChild->next;
		} 	
		else {
    	rightChild = NULL;
		}

            // Find the minimum child
            if (leftChild != NULL && leftChild->priority < minChild->priority)
            {
                minChild = leftChild;
            }

            if (rightChild != NULL && rightChild->priority < minChild->priority)
            {
                minChild = rightChild;
            }

            // Swap if necessary
            if (minChild != current)
            {
                swap(current->priority, minChild->priority);
                swap(current->data, minChild->data);
                current = minChild;
            }
            else
            {
                break;  // The heap property is satisfied
            }
        }
    }

    void update(int n)
    {
        int num;
        int count = 0;
        display();
        cout << "Enter the digit of the corresponding task you want to update: " << endl;
        cin >> num;

        Task* temp = front;
        while (count < num) {
            if (temp->next == NULL) {
                cout<<"Task # does not exist !"<<endl;
				return;
            }
            temp = temp->next;
            count++;
        }
        if (n == 1)
        {
            cout << "Enter updated Tittle: ";
            cin.ignore();
            getline(cin, temp->data.tittle);
        }
        else if (n == 2)
        {
            cout << "Enter updated Description: ";
            getline(cin, temp->data.description);
        }
        else if (n == 3)
        {
            temp->input_deadline();
        }
        else
        {
            cout << "Invalid Input!\n";
        }
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "No task to display!\n";
        }
        else
        {
            Task* temp = front;
            int i = 1;
            while (temp)
            {
                cout << "Task #: " << i << endl;
                cout << "Title: " << temp->data.tittle << endl;
                cout << "Description: " << temp->data.description << endl;
                cout << "Priority: " << temp->priority << endl;
                cout << "Deadline: " << temp->data.deadline_date << "-" << temp->data.deadline_month << "-" << temp->data.deadline_year << "\t" << temp->data.deadline_hour << ":" << temp->data.deadline_minute << endl << endl;
                i++;
                temp = temp->next;
            }
        }
    }
};

Task* merge(Task* left, Task* right)
{
    Task* result = NULL;

    if (left == NULL)
        return right;
    if (right == NULL)
        return left;

    if (left->data.deadline_year < right->data.deadline_year ||
        (left->data.deadline_year == right->data.deadline_year &&
            (left->data.deadline_month < right->data.deadline_month ||
                (left->data.deadline_month == right->data.deadline_month &&
                    left->data.deadline_date < right->data.deadline_date))))
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }

    return result;
}

// Merge Sort function for sorting linked list based on due date
Task* mergeSort(Task* head)
{
    if (head == NULL || head->next == NULL)
        return head;

    Task* slow = head;
    Task* fast = head->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    Task* left = head;
    Task* right = slow->next;
    slow->next = NULL;

    left = mergeSort(left);
    right = mergeSort(right);

    return merge(left, right);
}

// Display function with sorting by due date using merge sort
void displaySortedByDueDate(PriorityQueue p)
{
    if (p.isEmpty())
    {
        cout << "No task to display!\n";
    }
    else
    {
        Task* sortedList = mergeSort(p.front);

        Task* temp = sortedList;
        int i = 1;
        while (temp)
        {
            cout << "Task #: " << i << endl;
            cout << "Title: " << temp->data.tittle << endl;
            cout << "Description: " << temp->data.description << endl;
            cout << "Priority: " << temp->priority << endl;
            cout << "Deadline: " << temp->data.deadline_date << "-" << temp->data.deadline_month << "-" << temp->data.deadline_year << "\t" << temp->data.deadline_hour << ":" << temp->data.deadline_minute << endl << endl;
            i++;
            temp = temp->next;
        }
    }
}

void menu(PriorityQueue& p)
{
    while (true)
    {
        int n, m;
        string boi;
        cout << "Press\n";
        cout << "\t1) To add task\n";
        cout << "\t2) To update task\n";
        cout << "\t3) To delete task\n";
        cout << "\t4) To display task\n";
        cout << "\t5) To exit\n";
        cin >> n;
        cout << "\n\n";
        switch (n)
        {
        case 1:
            int priority;
            cout << "Enter the priority of the task: ";
            cin >> priority;
            p.addTask(priority);
            cout << "\n\n";
            p.display();
            break;
        case 2:
            if (p.isEmpty())
            {
                cout << "No task to update!\n";
            }
            else
            {
                cout << "Press\n";
                cout << "\t1) To update tittle\n";
                cout << "\t2) To update description\n";
                cout << "\t3) To update deadline\n";
                cout << "\t4) To goto Main Menu\n";
                cin >> m;
                p.update(m);
                cout << "\n\n";
                p.display();
            }
            break;
        case 3:
            if (p.isEmpty())
            {
                cout << "No task to delete!\n";
            }
            else
            {
                cout << "Enter a title:" << endl;
                cin >> boi;
                p.deleteTask(boi);
                cout << "\n\n";
                p.display();
            }
            break;
        case 4:
            displaySortedByDueDate(p);
            break;
        case 5:
            return;
        default:
            cout << "Invalid Input!\n";
        }
        cout << "\n\n";
    }
}

int main()
{
    PriorityQueue pq;
    menu(pq);
    return 0;
}
