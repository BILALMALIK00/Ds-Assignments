#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include"config.h"
using namespace std;
const int High=1;
const int Medium=2;
const int low=3;

class Node{
	public:
		int data;
		Node*next;
		Node(int d){
			data=d;
			next=NULL;
		}
};
//ticket class
class Ticket {
public:
    int ticketID;
    string customerName;
    int priority; // 1 = High, 2 = Medium, 3 = Low
    string description;
    time_t creationTime;
    string status; // "open" or "closed"
    time_t closeTime;
    bool assign;
    Ticket* next;

    Ticket(int id, string name, int pri, string desc) {
        ticketID = id;
        customerName = name;
        priority = pri;
        description = desc;
        creationTime = time(NULL);
        status = "open";
        closeTime = 0;
        next = NULL;
        assign=false;
        
    }
};
class TicketResolutionLog {
private:
    Node* top;

public:
    TicketResolutionLog() {
        top = NULL;
    }
	Node*getTop(){
		if(top!=NULL){
			return top;
		}
	}
    void push(Ticket* ticket) {
        Node* temp = new Node(ticket->ticketID); // Store ticket ID in the stack
        temp->next = top;
        top = temp;
        cout << "Ticket with ID " << ticket->ticketID << " successfully resolved and added to Ticket Resolution Log." << endl;
    }

    bool isEmpty() {
        return top == NULL;
    }

    int pop() {
        if (isEmpty()) {
            cout << "Stack Underflow: No Ticket to pop." << endl;
            return -1;
        } else {
            int ticketID = top->data;
            Node* curr = top;
            top = top->next;
            delete curr;
            cout << "Ticket ID " << ticketID << " popped from resolution log." << endl;
            return ticketID;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "No resolution logs available." << endl;
            return;
        }
        Node* temp = top;
        cout << "Resolution Log (Most Recent First): ";
        while (temp != NULL) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

class PendingTicketQueue {
private:
    Ticket* front;
    Ticket* rear;

public:
    PendingTicketQueue() {
        front = NULL;
        rear = NULL;
    }

    void enqueue(Ticket* ticket) {
        if (rear == NULL) {
            front = rear = ticket;
        } else {
            rear->next = ticket;
            rear = ticket;
        }
        ticket->next = NULL;
        cout << "Ticket with ID " << ticket->ticketID << " added to pending queue." << endl;
    }

    Ticket* dequeue() {
        if (front == NULL) {
            cout << "Queue Underflow: No tickets in pending queue." << endl;
            return NULL;
        } else {
            Ticket* temp = front;
            front = front->next;
            if (front == NULL) {
                rear = NULL;
            }
            cout << "Ticket with ID " << temp->ticketID << " removed from pending queue." << endl;
            return temp; // Return the dequeued ticket
        }
    }

    bool isEmpty() {
        return front == NULL;
    }

    void display() {
        if (isEmpty()) {
            cout << "No tickets in pending queue." << endl;
            return;
        }
        Ticket* current = front;
        cout << "Pending Tickets: ";
        while (current != NULL) {
            cout << current->ticketID << " ";
            current = current->next;
        }
        cout << endl;
    }
};
//class to manage tickets and includes all functions required
class Ticket_Management {
private:
    Ticket* head;
    Ticket* tail;
     PendingTicketQueue p1; 
    TicketResolutionLog r1;


public:
    Ticket_Management() {
        head = NULL;
        tail = NULL;
    }
     Ticket* get_head() {
        return head;
    }


    void add_ticket(int id, string name, int priority, string desc) {
        Ticket* temp = new Ticket(id, name, priority, desc);
        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = temp;
        }
        cout << "Ticket added successfully!" << endl;
        p1.enqueue(temp);
    }
 	void resolveTicket(int id) {
        Ticket* current = head;
        while (current != NULL) {
            if (current->ticketID == id && current->status == "open") {
                current->status = "closed";
                current->closeTime = time(NULL);
               r1.push(current); 
                cout << "Ticket ID " << id << " has been resolved." << endl;
                return;
            }
            current = current->next;
        }
        cout << "Ticket not found or already resolved!" << endl;
    }
    void displayPendingQueue() {
        p1.display();
    }
    
    void displayResolutionLog() {
        r1.display();
    }
    void removeticket(int id) {
        if (head == NULL) {
            cout << "The list is empty. No ticket to remove." << endl;
            return;
        }

        Ticket* current = head;
        Ticket* previous = NULL;
        while (current != NULL && current->ticketID != id) {
            previous = current;
            current = current->next;
        }
        if (current == NULL) {
            cout << "Ticket not found!" << endl;
            return;
        }
        if (previous == NULL) {
            head = current->next;
            if (head == NULL) {
                tail = NULL;
            }
        } else {
            previous->next = current->next;
            if (current == tail) {
                tail = previous;
            }
        }
        delete current;
        cout << "Ticket removed successfully!" << endl;
    }
    Ticket* mergeSortedLists(Ticket* left, Ticket* right, int criteria) {
    if (!left) return right;
    if (!right) return left;
    Ticket* result = NULL;
    bool condition = false;
    if (criteria == 1) condition = left->priority <= right->priority;
    else if (criteria == 2) condition = left->creationTime <= right->creationTime;
    else if (criteria == 3) condition = left->customerName <= right->customerName;
	
    if (condition) {
        result = left;
        result->next = mergeSortedLists(left->next, right, criteria);
    } else {
        result = right;
        result->next = mergeSortedLists(left, right->next, criteria);
    }

    return result;
}
//helper function
Ticket* findMiddle(Ticket* head) {
    if (!head) return head;
    Ticket* slow = head;
    Ticket* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
//mergesort time complexity O(log n)
Ticket* mergeSortTickets(Ticket* head, int criteria) {
    if (!head || !head->next) return head;

    Ticket* middle = findMiddle(head);
    Ticket* nextToMiddle = middle->next;
    middle->next = NULL;

    Ticket* left = mergeSortTickets(head, criteria);
    Ticket* right = mergeSortTickets(nextToMiddle, criteria);

    return mergeSortedLists(left, right, criteria);
}

    void sortTicketsMerge(int criteria) {
    head = mergeSortTickets(head, criteria);
    cout << "Tickets sorted using Merge Sort." << endl;
}

	//bubble sort time complexity O (n^2)
    void bubbleSort(int criteria) {
    if (!head) return;

    bool swapped;
    do {
        swapped = false;
        Ticket* current = head;

        while (current && current->next) {
            bool condition = false;
            if (criteria == 1) { // Sort by priority
                condition = (current->priority > current->next->priority);
            } else if (criteria == 2) { // Sort by creation time
                condition = (current->creationTime > current->next->creationTime);
            } else if (criteria == 3) { // Sort by customer name
                condition = (current->customerName > current->next->customerName);
            }else if (criteria == 4) { // Sort by customer name
                condition = (current->ticketID > current->next->ticketID);
            }

            if (condition) {
                // Swap the data of current and next nodes
                swap(current->ticketID, current->next->ticketID);
                swap(current->customerName, current->next->customerName);
                swap(current->priority, current->next->priority);
                swap(current->description, current->next->description);
                swap(current->creationTime, current->next->creationTime);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}
	//Selection sort time complexity O (n^2)
void selectionSort(int criteria) {
    if (head == NULL || head->next == NULL) return;

    Ticket* current = head;
    while (current) {
        Ticket* minNode = current;
        Ticket* temp = current->next;
        while (temp) {
            bool condition = false;
            if (criteria == 1) {
                condition = (temp->priority < minNode->priority);
            } else if (criteria == 2) {
                condition = (temp->creationTime < minNode->creationTime);
            } else if (criteria == 3) {
                condition = (temp->customerName < minNode->customerName);
            }

            if (condition) {
                minNode = temp;
            }
            temp = temp->next;
        }
        if (minNode != current) {
            swap(current->ticketID, minNode->ticketID);
            swap(current->customerName, minNode->customerName);
            swap(current->priority, minNode->priority);
            swap(current->description, minNode->description);
            swap(current->creationTime, minNode->creationTime);
        }
        current = current->next;
    }
}
	//insertionsort time complexity O (n^2)
void insertionSort(int criteria) {
    if (head == NULL || head->next == NULL) return;

    Ticket* sorted = NULL;
    Ticket* current = head;

    while (current) {
        Ticket* nextNode = current->next;

        if (sorted == NULL || (criteria == 1 && current->priority <= sorted->priority) ||
            (criteria == 2 && current->creationTime <= sorted->creationTime) ||
            (criteria == 3 && current->customerName <= sorted->customerName)) {
            current->next = sorted;
            sorted = current;
        } else {
            Ticket* temp = sorted;
            while (temp->next && ((criteria == 1 && temp->next->priority < current->priority) ||
                (criteria == 2 && temp->next->creationTime < current->creationTime) ||
                (criteria == 3 && temp->next->customerName < current->customerName))) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = nextNode;
    }
    head = sorted;
}
	Ticket *partition(Ticket *head, Ticket *tail, int criteria) {
    Ticket *pivot = head;      // The pivot is the head
    Ticket *curr = head->next; // Start checking from the second element
    Ticket *prev = head;       // Last element smaller than pivot

    while (curr != tail->next) {
        if ((criteria == 1 && curr->priority < pivot->priority) ||
            (criteria == 2 && curr->creationTime < pivot->creationTime) ||
            (criteria == 3 && curr->customerName < pivot->customerName) ||
            (criteria == 4 && curr->ticketID < pivot->ticketID)) {
            // Move prev forward and swap fields with curr
            prev = prev->next;
            swap(prev->ticketID, curr->ticketID);
            swap(prev->customerName, curr->customerName);
            swap(prev->priority, curr->priority);
            swap(prev->creationTime, curr->creationTime);
            swap(prev->status, curr->status);
            swap(prev->assign, curr->assign);
            swap(prev->description, curr->description);
            swap(prev->closeTime, curr->closeTime);
        }
        curr = curr->next;
    }

    // Finally, swap the pivot with the last smaller element
    swap(head->ticketID, prev->ticketID);
    swap(head->customerName, prev->customerName);
    swap(head->priority, prev->priority);
    swap(head->creationTime, prev->creationTime);
    swap(head->status, prev->status);
    swap(head->assign, prev->assign);
    swap(head->description, prev->description);
    swap(head->closeTime, prev->closeTime);

    return prev; // Return the pivot's final position
}

void solveSort(Ticket *head, Ticket *tail, int criteria) {
    // single element or null
    if (head == NULL || head == tail)
        return;

    Ticket *pivot = partition(head, tail, criteria);
    solveSort(head, pivot, criteria);
    solveSort(pivot->next, tail, criteria);
}
Ticket* getTail(Ticket* head) {
    if (head == NULL) {
        return NULL; 
    }
    
    Ticket* current = head;
    while (current->next != NULL) {
        current = current->next; 
    }
    
    return current; 
}

Ticket *quickSort(Ticket *head, int criteria) {
    // single element or null
    if (head == NULL || head->next == NULL)
        return head;
    Ticket *tail = getTail(head);
    solveSort(head, tail, criteria);
    return head;
} 
    Ticket* getMiddle(Ticket* left, Ticket* right) {
    if (left == NULL) return NULL;

    Ticket* slow = left;
    Ticket* fast = left->next;

    while (fast != right) {
        fast = fast->next;
        if (fast != right) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    return slow; 
}

Ticket* binarySearchByName(Ticket* head,string name) {
    Ticket* left = head;
    Ticket* right = getTail(head); 

    while (left != right) {
        Ticket* mid = getMiddle(left, right); 
        if (mid->customerName == name) {
            return mid; 
        }
        if (mid->customerName < name) {
            left = mid->next;
        } else {
            right = mid; 
        }
    }
    return NULL; 
}

    Ticket* binarySearchById(Ticket* head, int id) {
    Ticket* left = head;
    Ticket* right = getTail(head); 

    while (left != right) {
        Ticket* mid = getMiddle(left, right); 
        if (mid->ticketID == id) {
            return mid; 
        }
        if (mid->ticketID < id) {
            left = mid->next; 
        } else {
            right = mid;
        }
    }
    return NULL; 
}

Ticket* search_ticket(int id) {
        Ticket* current = head;
        while (current != NULL) {
            if (current->ticketID == id) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }
    Ticket* search_ticket(string name) {
        Ticket* current = head;
        while (current != NULL) {
            if (current->customerName == name) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void displayTickets() {
        if (head == NULL) {
            cout << "No tickets available." << endl;
            return;
        }
       Ticket *temp = head;
        while (temp != NULL)
        {
            cout << "Customer Name: " << temp->customerName << "\n";
            cout << "Priority: " << temp->priority << "\n";
            cout << "Service Request: " << temp->description << "\n";
            cout << "Creation Time: " << ctime(&temp->creationTime);
            cout << "Ticket Id: " << temp->ticketID << endl;
            if (temp->status == "open")
            {
                cout << "Ticket is open" << endl;
            }
            else
            {
                cout << "Ticket is closed" << endl;
            }
            if (temp->assign == true)
            {
               cout << "Ticket Assigned to  agent" << endl;
            }
            else
            {
                cout << "Ticket is not assigned" << endl;
            }
            cout << "-------------------------\n";

            temp = temp->next;
        }
    }
    TicketResolutionLog& getResolutionLog() {
        return r1;
    }

    ~Ticket_Management() {
        Ticket* current = head;
        while (current != NULL) {
            Ticket* next = current->next;
            delete current;
            current = next;
        }
    }
};
class Agent {
public:
    int agentID;
    string name;
    bool availability;
    int* assigned_tickets;
    int ticketscount;
    string status;

    Agent(int ag, string n) {
        agentID = ag;
        name = n;
        availability = true;
        ticketscount = 0;
        assigned_tickets = new int[5];  // Can handle up to 5 tickets
        status = "Available";
        for (int i = 0; i < 5; i++) {
            assigned_tickets[i] = -1;
        }
    }

    void add_ticket(int ticket_id) {
        if (ticketscount < 5) {
            assigned_tickets[ticketscount++] = ticket_id;
            if (ticketscount == 5) {
                availability = false;
                status = "Unavailable";
            }
        } else {
            cout << "Agent is already at full capacity (5 tickets)" << endl;
        }
    }
int get_ticket_count(){
	return ticketscount;
}
    void remove_ticket(int ticketID) {
        for (int i = 0; i < ticketscount; i++) {
            if (assigned_tickets[i] == ticketID) {
                for (int j = i; j < ticketscount - 1; j++) {
                    assigned_tickets[j] = assigned_tickets[j + 1];
                }
                ticketscount--;
                if (ticketscount < 5) {
                    availability = true;
                    status = "Available";
                }
                break;
            }
        }
    }

    void displayAgentInfo() {
        cout << "Agent ID: " << agentID << "\nName: " << name
             << "\nStatus: " << status << "\nAssigned Tickets: ";
        if (ticketscount == 0) {
            cout << "None";
        } else {
            for (int i = 0; i < ticketscount; i++) {
                cout << assigned_tickets[i] << " ";
            }
        }
        cout << endl;
    }

    ~Agent() {
        delete[] assigned_tickets;
    }
};

// Agent Management class
class Agent_Management {
public:
    Agent** agents;
    int capacity;
    int count;

    Agent_Management(int cap) {
        capacity = cap;
        agents = new Agent*[capacity];
        count = 0;
    }

    void addAgent(Agent* a1) {
        if (count < capacity) {
            agents[count++] = a1;
            cout << "Agent Added Successfully" << endl;
        } else {
            cout << "No Capacity Available" << endl;
        }
    }

void assign_ticket_to_agent(Ticket* ticket) {
    if (!ticket || ticket->assign){
    cout<<"Tickets Already Assigned"<<endl;	
	} 

    int index = -1;
    int min= INT_MAX;
    if (ticket->priority == High) {
        for (int i = 0; i < count; i++) {
            if (agents[i]->availability) {
                int currentTicketCount = agents[i]->get_ticket_count();
                if (currentTicketCount < min) {
                    min = currentTicketCount;
                    index = i;
                }
            }
        }
    }
    if (index == -1 && ticket->priority == Medium) {
        for (int i = 0; i < count; i++) {
            if (agents[i]->availability) {
                int currentTicketCount = agents[i]->get_ticket_count();
                if (currentTicketCount < min) {
                    min = currentTicketCount;
                 index = i;
                }
            }
        }
    }
    if (index == -1 && ticket->priority == low) {
        for (int i = 0; i < count; i++) {
            if (agents[i]->availability) {
                int currentTicketCount = agents[i]->get_ticket_count();
                if (currentTicketCount < min) {
                    min = currentTicketCount;
                    index = i;
                }
            }
        }
    }
    if (index != -1) {
        agents[index]->add_ticket(ticket->ticketID);
        cout << "Ticket " << ticket->ticketID << " assigned to Agent " << agents[index]->name << endl;
        ticket->assign=true;
      int maxcapacity = 5; 
        if (agents[index]->get_ticket_count() >= maxcapacity) {
            agents[index]->availability = false; 
            agents[index]->status="unavailable";
            cout << "Agent " << agents[index]->name << " is now marked as unavailable." << endl;
        }
    } else {
        cout << "All agents are at full capacity or no suitable agents available!" << endl;
    }
}

    void displayAllAgents() {
        if (count == 0) {
            cout << "No agents available!" << endl;
            return;
        }

        for (int i = 0; i < count; i++) {
            agents[i]->displayAgentInfo();
        }
    }

    ~Agent_Management() {
        for (int i = 0; i < count; i++) {
            delete agents[i];
        }
        delete[] agents;
    }
};
//for reporting and analysis
class Reporting {
public:
    void listOpenTickets(Ticket *head, bool sortByPriority) {
        int capacity = 10;
        int size = 0; 
        Ticket** openTickets = new Ticket*[capacity]; 
        Ticket *current = head;
        while (current != NULL) {
            if (current->status == "open") {
                if (size == capacity) {
                    capacity *= 2;
                    Ticket** newOpenTickets = new Ticket*[capacity];
                    for (int i = 0; i < size; i++) {
                        newOpenTickets[i] = openTickets[i];
                    }
                    delete[] openTickets;
                    openTickets = newOpenTickets;
                }
                openTickets[size++] = current;
            }
            current = current->next;
        }

        // Sort the open tickets based on priority or creation time
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if ((sortByPriority && openTickets[j]->priority > openTickets[j + 1]->priority) ||
                    (!sortByPriority && openTickets[j]->creationTime > openTickets[j + 1]->creationTime)) {
                    Ticket *temp = openTickets[j];
                    openTickets[j] = openTickets[j + 1];
                    openTickets[j + 1] = temp;
                }
            }
        }

        // Display sorted open tickets
        std::cout << "Open Tickets (sorted by " 
                  << (sortByPriority ? "Priority" : "Creation Time") << "):\n";
        for (int i = 0; i < size; i++) {
            std::cout << "Ticket ID: " << openTickets[i]->ticketID
                      << ", Priority: " << openTickets[i]->priority
                      << ", Creation Time: " << openTickets[i]->creationTime << "\n";
        }

        // Clean up dynamic memory
        delete[] openTickets;
    }

    // Agent Ticket Load
    void displayAgentTicketLoad(Agent **agents, int agentCount) {
        int capacity = 10;
        int size = 0;
        Agent **agentList = new Agent*[capacity];

        for (int i = 0; i < agentCount; ++i) {
            if (size == capacity) {
                capacity *= 2;
                Agent **newList = new Agent*[capacity];
                for (int j = 0; j < size; j++) {
                    newList[j] = agentList[j];
                }
                delete[] agentList;
                agentList = newList;
            }
            agentList[size++] = agents[i];
        }

        // Sort agents by number of tickets assigned
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (agentList[j]->get_ticket_count() > agentList[j + 1]->get_ticket_count()) {
                    Agent *temp = agentList[j];
                    agentList[j] = agentList[j + 1];
                    agentList[j + 1] = temp;
                }
            }
        }

        // Display Agent Load
        std::cout << "Agent Ticket Load:\n";
        for (int i = 0; i < size; i++) {
            std::cout << "Agent ID: " << agentList[i]->agentID 
                      << ", Tickets Assigned: " << agentList[i]->get_ticket_count() << "\n";
        }

        delete[] agentList;  
    }

    // Ticket Resolution History - using Stack
    void displayTicketResolutionHistory(TicketResolutionLog &log) {
       cout << "Ticket Resolution History:\n";
        Node* current = log.getTop();  
        
        while (current != NULL) {
            cout << "TicketID: " << current->data << "\n";
            current = current->next;  
        }
    }
};
int main() {
    Ticket_Management ticketManagement;
    Agent_Management agentManagement(10);  
    Reporting reporting;

    // Sample agents to start with
    agentManagement.addAgent(new Agent(1, "Ashok"));
    agentManagement.addAgent(new Agent(2, "Bikal"));
    agentManagement.addAgent(new Agent(3, "Cherry"));

    int choice;
    do {
        cout << "\n************ One-Stop Management System ************\n";
        cout << "1. Add Ticket\n";
        cout << "2. Remove Ticket\n";
        cout << "3. Search Ticket by Customer Name\n";
        cout << "4. Search Ticket by Ticket ID\n";
        cout << "5. Display All Tickets\n";
        cout << "6. Sort Tickets\n";
        cout << "7. Assign Ticket to Agent\n";
        cout << "8. Resolve Ticket\n";
        cout << "9. View Pending Queue\n";
        cout << "10. View Ticket Resolution Log\n";
        cout << "11. List Open Tickets\n";
        cout << "12. Display Agent Ticket Load\n";
        cout << "13. Display Ticket Resolution History\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id, priority;
                string name, description;
                cout << "Enter Ticket ID: ";
                cin >> id;
                cout << "Enter Customer Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Priority (1=High, 2=Medium, 3=Low): ";
                cin >> priority;
                cout << "Enter Description: ";
                cin.ignore();
                getline(cin, description);
                ticketManagement.add_ticket(id, name, priority, description);
                break;
            }
            case 2: {
                int id;
                cout << "Enter Ticket ID to remove: ";
                cin >> id;
                ticketManagement.removeticket(id);
                break;
            }
            case 3: { // Search by Customer Name
    string name;
    cout << "Enter Customer Name to search: ";
    cin.ignore();
    getline(cin, name);
    
    Ticket* result = NULL;

    // Count the number of tickets
    int ticketCount = 0;
    Ticket* temp = ticketManagement.get_head();
    while (temp) {
        ticketCount++;
        temp = temp->next;
    }

    // Choose search method based on threshold
    if (ticketCount < threshold) {
        result = ticketManagement.search_ticket(name); // Linear search by name
        cout << "Using Linear Search.\n";
    } else {
        // Ensure tickets are sorted by name before binary search
        ticketManagement.selectionSort(3); // Ensure tickets are sorted based on the name
        result = ticketManagement.binarySearchByName(ticketManagement.get_head(), name); // Binary search by name
        cout << "Using Binary Search.\n";
    }

    if (result) {
        cout << "Ticket found:\n";
        cout << "Ticket ID: " << result->ticketID << ", Priority: " << result->priority 
             << ", Status: " << result->status << "\n";
    } else {
        cout << "Ticket not found.\n";
    }
    break;
}
case 4: { // Search by Ticket ID
    int id;
    cout << "Enter Ticket ID to search: ";
    cin >> id;
    
    Ticket* result = NULL;

    // Count the number of tickets
    int ticketCount = 0;
    Ticket* temp = ticketManagement.get_head();
    while (temp) {
        ticketCount++;
        temp = temp->next;
    }

    // Choose search method based on threshold
    if (ticketCount < threshold) {
        result = ticketManagement.search_ticket(id); // Linear search by ID
        cout << "Using Linear Search.\n";
    } else {
        // Ensure tickets are sorted by ID before binary search
        ticketManagement.selectionSort(4); // Ensure ticket are sorted based on the ID
        result = ticketManagement.binarySearchById(ticketManagement.get_head(), id); // Binary search by ID
        cout << "Using Binary Search.\n";
    }

    if (result) {
        cout << "Ticket found:\n";
        cout << "Ticket ID: " << result->ticketID << ", Priority: " << result->priority 
             << ", Status: " << result->status << "\n";
    } else {
        cout << "Ticket not found.\n";
    }
    break;
}

            case 5: {
                ticketManagement.displayTickets();
                break;
            }
            case 6: {
                int criteria;
                cout << "Select Sorting Criteria:\n";
                cout << "1. Priority\n2. Creation Time\n3. Customer Name\n";
                cout << "Enter choice: ";
                cin >> criteria;

                int ticketCount = 0;
                Ticket* temp = ticketManagement.get_head();
                while (temp) {
                    ticketCount++;
                    temp = temp->next;
                }

                if (ticketCount < threshold) {
                    if (elementarySortingAlgorithm == BUBBLE_SORT)
                        ticketManagement.bubbleSort(criteria);
                    else if (elementarySortingAlgorithm == INSERTION_SORT)
                        ticketManagement.insertionSort(criteria);
                    else if (elementarySortingAlgorithm == SELECTION_SORT)
                        ticketManagement.selectionSort(criteria);
                    cout << "Tickets sorted with elementary sort.\n";
                } else {
                    if (advancedSortingAlgorithm == MERGE_SORT)
                        ticketManagement.sortTicketsMerge(criteria);
                    else if (advancedSortingAlgorithm == QUICK_SORT)
                        ticketManagement.quickSort(ticketManagement.get_head(), criteria);
                    cout << "Tickets sorted with advanced sort.\n";
                }
                break;
            }
            case 7: {
                int ticketID;
                cout << "Enter Ticket ID to assign: ";
                cin >> ticketID;
                Ticket* ticket = ticketManagement.search_ticket(ticketID);
                if (ticket && !ticket->assign) {
                    agentManagement.assign_ticket_to_agent(ticket);
                } else {
                    cout << "Ticket not found or already assigned.\n";
                }
                break;
            }
            case 8: {
                int ticketID;
                cout << "Enter Ticket ID to resolve: ";
                cin >> ticketID;
                ticketManagement.resolveTicket(ticketID);
                break;
            }
            case 9: {
                ticketManagement.displayPendingQueue();
                break;
            }
            case 10: {
                ticketManagement.displayResolutionLog();
                break;
            }
            case 11: {
                bool sortByPriority;
                cout << "Sort by Priority? (1 for Yes, 0 for No): ";
                cin >> sortByPriority;
                reporting.listOpenTickets(ticketManagement.get_head(), sortByPriority);
                break;
            }
            case 12: {
                reporting.displayAgentTicketLoad(agentManagement.agents, agentManagement.count);
                break;
            }
            case 13: {
                reporting.displayTicketResolutionHistory(ticketManagement.getResolutionLog());
                break;
            }
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
